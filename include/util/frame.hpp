#pragma once
#include "../core/command.hpp"
#include "../core/context.hpp"
#include "../core/framebuffer.hpp"
#include "../core/pipeline.hpp"
#include "../core/queue.hpp"
#include "../core/swapchain.hpp"
#include "../core/sync.hpp"
#include "../core/vkobj.hpp"

namespace Terreate::Util {
class Frame {
private:
  Core::VkObjectRef<Core::ICommandBuffer> mCommandBuffer;
  Core::VkObjectRef<Core::ISwapchain> mSwapchain;
  Core::VkObjectRef<Core::ISemaphore> mImageAvailableSemaphore;
  Core::VkObjectRef<Core::ISemaphore> mRenderFinishedSemaphore;
  Core::VkObjectRef<Core::IFence> mFence;

  Core::VkObjectRef<Core::IFramebuffer> mFramebuffer;
  Core::VkObjectRef<Core::GraphicQueue> mGraphicQueue;
  Core::VkObjectRef<Core::PresentQueue> mPresentQueue;
  Core::VkObjectRef<Core::IPipeline> mPipeline;

  Type::vec<float> mViewportOffset = {0, 0};
  Type::vec<float> mViewportSize = {0, 0};
  Type::vec<float> mViewportDepth = {0, 0};
  Type::vec<Type::u32> mScissorOffset = {0, 0};
  Type::vec<Type::u32> mScissorSize = {0, 0};
  Type::u32 mCurrentImageIndex = 0;

public:
  Frame(Core::Context *context,
        Core::VkObjectRef<Core::ICommandPool> commandPool,
        Core::VkObjectRef<Core::IFramebuffer> framebuffer,
        Core::VkObjectRef<Core::ISwapchain> swapchain,
        Core::VkObjectRef<Core::GraphicQueue> graphic,
        Core::VkObjectRef<Core::PresentQueue> present,
        Core::VkObjectRef<Core::IPipeline> pipeline);
  ~Frame() {}

  void begin(Type::vec<float> clearColor = {0.0f, 0.0f, 0.0f, 1.0f});
  void setViewport(Type::vec<float> offset, Type::vec<float> size,
                   Type::vec<float> depth);
  void setScissor(Type::vec<Type::u32> offset, Type::vec<Type::u32> size);
  void draw(Type::u32 vertexCount, Type::u32 instanceCount,
            Type::u32 firstVertex = 0, Type::u32 firstInstance = 0) {
    mCommandBuffer->drawBuffer(vertexCount, instanceCount, firstVertex,
                               firstInstance);
  }
  void end();
};
} // namespace Terreate::Util
