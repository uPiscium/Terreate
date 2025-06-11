#pragma once
#include "command.hpp"
#include "device.hpp"
#include "sync.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class GraphicQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(GraphicQueue);

private:
  struct SubmitInfo {
    Type::vec<VkCommandBuffer> commandBuffers;
    Type::vec<VkPipelineStageFlags> waitStages;
    Type::vec<VkSemaphore> waitSemaphores;
    Type::vec<VkSemaphore> signalSemaphores;
  };

private:
  VkObjectRef<Device> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;
  Type::vec<SubmitInfo> mSubmitInfos;

private:
  Type::vec<VkSubmitInfo> createSubmitInfos();

public:
  GraphicQueue(VkObjectRef<Device> device);
  ~GraphicQueue() { this->dispose(); }

  VkObjectRef<Device> getDevice() const { return mDevice; }

  void queue(Type::vec<VkObjectRef<ICommandBuffer>> commandBuffers,
             Type::vec<Type::PipelineStage> waitStages = {},
             Type::vec<VkObjectRef<Semaphore>> waitSemaphores = {},
             Type::vec<VkObjectRef<Semaphore>> signalSemaphores = {});
  void submit(VkObjectRef<Fence> fence = nullptr);
  void dispose();

  operator VkQueue() const { return mQueue; }
};

class PresentQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(PresentQueue);

private:
  VkObjectRef<Device> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;

public:
  PresentQueue(VkObjectRef<Device> device);
  ~PresentQueue() { this->dispose(); }

  VkObjectRef<Device> getDevice() const { return mDevice; }

  void present(VkObjectRef<Swapchain> swapchain,
               Type::vec<Type::u32> imageIndices,
               VkObjectRef<Semaphore> waitSemaphore = nullptr);
  void dispose();

  operator VkQueue() const { return mQueue; }
};
} // namespace Terreate::Core
