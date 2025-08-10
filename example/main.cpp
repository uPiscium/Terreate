// Minimal multi-window Vulkan+SDL3 demo (single render thread).
// Build: requires Vulkan SDK and SDL3 dev. See README instructions below.
//
// Note: This is a minimal, straightforward implementation intended for
// prototyping and correctness. It uses CPU waits for simplicity (not optimal).

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

struct WindowContext {
  SDL_Window *sdlWindow = nullptr;
  vk::SurfaceKHR surface;
  vk::SwapchainKHR swapchain;
  vk::Format format{};
  vk::Extent2D extent{};
  std::vector<vk::Image> images;
  std::vector<vk::ImageView> imageViews;
  std::vector<vk::Framebuffer> framebuffers;
  std::vector<vk::CommandBuffer> cmdBuffers;
  vk::Fence acquireFence;
  vk::Fence renderFence;
  vk::Semaphore presentSemaphore;
  vk::ClearColorValue clearColor;
};

static uint32_t chooseQueueFamilySupporting(vk::PhysicalDevice pd,
                                            vk::SurfaceKHR surface) {
  auto props = pd.getQueueFamilyProperties();
  for (uint32_t i = 0; i < props.size(); ++i) {
    if (props[i].queueFlags & vk::QueueFlagBits::eGraphics) {
      VkBool32 present = VK_FALSE;
      // check present support for the surface
      if (pd.getSurfaceSupportKHR(i, surface))
        return i;
    }
  }
  throw std::runtime_error(
      "No queue family supports graphics+present for the surface");
}

int main() {
  try {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                  SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_CAMERA |
                  SDL_INIT_SENSOR | SDL_INIT_AUDIO)) {
      throw std::runtime_error(std::string("SDL_Init failed: ") +
                               SDL_GetError());
    }

    // create two windows
    SDL_Window *winA = SDL_CreateWindow("View A", 640, 480, SDL_WINDOW_VULKAN);
    SDL_Window *winB =
        SDL_CreateWindow("Inspector", 320, 240, SDL_WINDOW_VULKAN);
    if (!winA || !winB)
      throw std::runtime_error("SDL_CreateWindow failed");

    // get instance extensions required by SDL (SDL3 API: returns array pointer)
    Uint32 extCount = 0;
    const char *const *sdlExts = SDL_Vulkan_GetInstanceExtensions(&extCount);
    if (!sdlExts || extCount == 0) {
      throw std::runtime_error("SDL_Vulkan_GetInstanceExtensions failed");
    }
    std::vector<const char *> instanceExts(sdlExts, sdlExts + extCount);

    // app info
    vk::ApplicationInfo appInfo("GamelibB_MultiWin",
                                VK_MAKE_API_VERSION(0, 1, 0, 0));
    vk::InstanceCreateInfo icinfo({}, &appInfo);
    // enable the SDL-required extensions
    icinfo.enabledExtensionCount = static_cast<uint32_t>(instanceExts.size());
    icinfo.ppEnabledExtensionNames = instanceExts.data();

    // create instance (Vulkan-Hpp)
    vk::UniqueInstance instance = vk::createInstanceUnique(icinfo);

    // create surfaces for windows (SDL helper)
    VkSurfaceKHR rawSurfaceA = VK_NULL_HANDLE;
    VkSurfaceKHR rawSurfaceB = VK_NULL_HANDLE;
    if (!SDL_Vulkan_CreateSurface(winA, instance.get(), nullptr, &rawSurfaceA))
      throw std::runtime_error("SDL_Vulkan_CreateSurface failed for winA");
    if (!SDL_Vulkan_CreateSurface(winB, instance.get(), nullptr, &rawSurfaceB))
      throw std::runtime_error("SDL_Vulkan_CreateSurface failed for winB");
    vk::SurfaceKHR surfaceA(rawSurfaceA);
    vk::SurfaceKHR surfaceB(rawSurfaceB);

    // pick physical device that can present to both surfaces (try each physical
    // device)
    auto physList = instance->enumeratePhysicalDevices();
    if (physList.empty())
      throw std::runtime_error("No physical devices found");
    vk::PhysicalDevice chosenPD;
    uint32_t chosenQFam = UINT32_MAX;
    for (auto &pd : physList) {
      auto props = pd.getQueueFamilyProperties();
      bool found = false;
      for (uint32_t qi = 0; qi < props.size(); ++qi) {
        if ((props[qi].queueFlags & vk::QueueFlagBits::eGraphics) ==
            vk::QueueFlags())
          continue;
        // check present support for both surfaces
        bool supA = pd.getSurfaceSupportKHR(qi, surfaceA);
        bool supB = pd.getSurfaceSupportKHR(qi, surfaceB);
        if (supA && supB) {
          chosenPD = pd;
          chosenQFam = qi;
          found = true;
          break;
        }
      }
      if (found)
        break;
    }
    if (!chosenPD) {
      // fallback: pick first device and first graphics queue (may or may not
      // present on both surfaces)
      chosenPD = physList[0];
      auto props = chosenPD.getQueueFamilyProperties();
      bool found = false;
      for (uint32_t qi = 0; qi < props.size(); ++qi) {
        if (props[qi].queueFlags & vk::QueueFlagBits::eGraphics) {
          chosenQFam = qi;
          found = true;
          break;
        }
      }
      if (!found)
        throw std::runtime_error("No graphics queue found on physical device");
      std::cerr << "Warning: did not find a queue family that reports present "
                   "support for both surfaces; using first graphics queue (may "
                   "fail on some platforms).\n";
    }

    // create logical device with swapchain extension enabled
    const char *deviceExts[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    float qPriority = 1.0f;
    vk::DeviceQueueCreateInfo qci({}, chosenQFam, 1, &qPriority);
    vk::DeviceCreateInfo dci({}, 1, &qci, 0, nullptr, 1,
                             deviceExts, // device extension count + names
                             nullptr);
    vk::UniqueDevice device = chosenPD.createDeviceUnique(dci);
    vk::Queue graphicsQueue = device->getQueue(chosenQFam, 0);

    // command pool for the graphics queue
    vk::UniqueCommandPool cmdPool =
        device->createCommandPoolUnique(vk::CommandPoolCreateInfo(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer, chosenQFam));

    // helper lambda: create swapchain & resources for a surface
    auto createForSurface =
        [&](SDL_Window *win, vk::SurfaceKHR surface,
            vk::ClearColorValue clearColor) -> WindowContext {
      WindowContext ctx;
      ctx.sdlWindow = win;
      ctx.surface = surface;
      ctx.clearColor = clearColor;

      // surface capabilities / formats
      auto caps = chosenPD.getSurfaceCapabilitiesKHR(surface);
      auto formats = chosenPD.getSurfaceFormatsKHR(surface);
      if (formats.empty())
        throw std::runtime_error("No surface formats");
      vk::SurfaceFormatKHR chosenFormat = formats[0];
      for (auto &f : formats) {
        if (f.format == vk::Format::eB8G8R8A8Unorm) {
          chosenFormat = f;
          break;
        }
      }
      ctx.format = chosenFormat.format;

      // choose extent
      vk::Extent2D extent;
      if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        extent = caps.currentExtent;
      } else {
        int w, h;
        SDL_GetWindowSize(win, &w, &h);
        extent.width = static_cast<uint32_t>(w);
        extent.height = static_cast<uint32_t>(h);
        extent.width =
            std::max(caps.minImageExtent.width,
                     std::min(caps.maxImageExtent.width, extent.width));
        extent.height =
            std::max(caps.minImageExtent.height,
                     std::min(caps.maxImageExtent.height, extent.height));
      }
      ctx.extent = extent;

      // present modes: prefer FIFO (always supported)
      vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

      // image count
      uint32_t imageCount = caps.minImageCount + 1;
      if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount)
        imageCount = caps.maxImageCount;

      // create swapchain
      vk::SwapchainCreateInfoKHR scinfo(
          {}, surface, imageCount, chosenFormat.format, chosenFormat.colorSpace,
          extent, 1, vk::ImageUsageFlagBits::eColorAttachment,
          vk::SharingMode::eExclusive, 0, nullptr, caps.currentTransform,
          vk::CompositeAlphaFlagBitsKHR::eOpaque, presentMode, VK_TRUE,
          VK_NULL_HANDLE);
      ctx.swapchain = device->createSwapchainKHR(scinfo);

      // images & imageviews
      ctx.images = device->getSwapchainImagesKHR(ctx.swapchain);
      ctx.imageViews.reserve(ctx.images.size());
      for (auto &img : ctx.images) {
        vk::ImageViewCreateInfo ivci(
            {}, img, vk::ImageViewType::e2D, ctx.format, vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0,
                                      1));
        ctx.imageViews.push_back(device->createImageView(ivci));
      }

      // renderpass (simple clear -> present)
      vk::AttachmentDescription attach(
          {}, ctx.format, vk::SampleCountFlagBits::e1,
          vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
          vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
          vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
      vk::AttachmentReference colorRef(
          0, vk::ImageLayout::eColorAttachmentOptimal);
      vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0,
                                     nullptr, 1, &colorRef);
      vk::RenderPassCreateInfo rpci({}, 1, &attach, 1, &subpass);
      vk::UniqueRenderPass renderPass = device->createRenderPassUnique(rpci);

      // framebuffers
      ctx.framebuffers.reserve(ctx.imageViews.size());
      for (auto &iv : ctx.imageViews) {
        vk::FramebufferCreateInfo fbci({}, *renderPass, 1, &iv,
                                       ctx.extent.width, ctx.extent.height, 1);
        ctx.framebuffers.push_back(device->createFramebuffer(fbci));
      }

      // command buffers (one per image)
      vk::CommandBufferAllocateInfo cbai(
          *cmdPool, vk::CommandBufferLevel::ePrimary,
          static_cast<uint32_t>(ctx.imageViews.size()));
      ctx.cmdBuffers = device->allocateCommandBuffers(cbai);

      // record command buffers (clear only)
      for (size_t i = 0; i < ctx.cmdBuffers.size(); ++i) {
        auto &cb = ctx.cmdBuffers[i];
        vk::CommandBufferBeginInfo binfo(
            vk::CommandBufferUsageFlagBits::eSimultaneousUse);
        cb.begin(binfo);
        vk::ClearValue clearVal;
        clearVal.color = ctx.clearColor;
        vk::RenderPassBeginInfo rpbi(*renderPass, ctx.framebuffers[i],
                                     vk::Rect2D({0, 0}, ctx.extent), 1,
                                     &clearVal);
        cb.beginRenderPass(rpbi, vk::SubpassContents::eInline);
        cb.endRenderPass();
        cb.end();
      }

      // simple fences (one pair reused)
      ctx.acquireFence = device->createFence(vk::FenceCreateInfo());
      ctx.renderFence = device->createFence(vk::FenceCreateInfo());
      ctx.presentSemaphore = device->createSemaphore(vk::SemaphoreCreateInfo());

      return ctx;
    };

    // create contexts for two windows with different clear colors
    WindowContext ctxA = createForSurface(
        winA, surfaceA,
        vk::ClearColorValue(std::array<float, 4>{0.2f, 0.4f, 0.8f, 1.0f}));
    WindowContext ctxB = createForSurface(
        winB, surfaceB,
        vk::ClearColorValue(std::array<float, 4>{0.8f, 0.4f, 0.2f, 1.0f}));

    // main loop (single render thread). simple event handling.
    bool running = true;
    SDL_Event e;
    while (running) {
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
          running = false;
          break;
        }
        if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
          running = false;
          break;
        }
      }

      // render window A then B sequentially (very simple synchronous method)
      auto renderOne = [&](WindowContext &ctx) {
        // acquire
        uint32_t imgIndex = 0;
        auto result = device->acquireNextImageKHR(ctx.swapchain, UINT64_MAX,
                                                  VK_NULL_HANDLE,
                                                  ctx.acquireFence, &imgIndex);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to acquire swapchain image");
        }
        result =
            device->waitForFences(1, &ctx.acquireFence, VK_TRUE, UINT64_MAX);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to wait for acquire fence");
        }

        result = device->resetFences(1, &ctx.acquireFence);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to reset acquire fence");
        }

        // submit recorded command buffer for this image
        vk::SubmitInfo submit({}, {}, {}, 1, &ctx.cmdBuffers[imgIndex]);
        graphicsQueue.submit(submit, ctx.renderFence);

        // wait for rendering to finish
        result =
            device->waitForFences(1, &ctx.renderFence, VK_TRUE, UINT64_MAX);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to wait for render fence");
        }

        result = device->resetFences(1, &ctx.renderFence);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to reset render fence");
        }

        // present
        vk::PresentInfoKHR present(1, &ctx.presentSemaphore, 1, &ctx.swapchain,
                                   &imgIndex, {});
        result = graphicsQueue.presentKHR(present);
        if (result != vk::Result::eSuccess) {
          throw std::runtime_error("Failed to present swapchain image");
        }
      };

      renderOne(ctxA);
      renderOne(ctxB);
    }

    // cleanup (destroy created Vulkan objects)
    // destroy framebuffers, imageViews, swapchains, surfaces manually
    // (device/instance must exist)
    auto cleanupCtx = [&](WindowContext &ctx) {
      for (auto &fb : ctx.framebuffers)
        device->destroyFramebuffer(fb);
      for (auto &iv : ctx.imageViews)
        device->destroyImageView(iv);
      device->destroySwapchainKHR(ctx.swapchain);
      device->destroyFence(ctx.acquireFence);
      device->destroyFence(ctx.renderFence);
      // surface will be destroyed on instance after this function returns
    };
    cleanupCtx(ctxA);
    cleanupCtx(ctxB);

    // destroy surfaces
    instance->destroySurfaceKHR(surfaceA);
    instance->destroySurfaceKHR(surfaceB);

    // destroy windows and quit SDL
    SDL_DestroyWindow(winA);
    SDL_DestroyWindow(winB);
    SDL_Quit();

    std::cout << "Exited cleanly.\n";
  } catch (const std::exception &ex) {
    std::cerr << "Fatal: " << ex.what() << "\n";
    SDL_Quit();
    return -1;
  }
  return 0;
}
