#pragma once
#include "command.hpp"
#include "device.hpp"
#include "sync.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class IQueue {
public:
  virtual ~IQueue() = default;
  virtual operator VkQueue() const = 0;
};

class GraphicQueue : public IQueue {
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
  VkObjectRef<IDevice> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;
  Type::vec<SubmitInfo> mSubmitInfos;

private:
  Type::vec<VkSubmitInfo> createSubmitInfos();

public:
  GraphicQueue(VkObjectRef<IDevice> device);
  ~GraphicQueue() override { this->dispose(); }

  void queue(Type::vec<VkObjectRef<ICommandBuffer>> commandBuffers,
             Type::vec<Type::PipelineStage> waitStages = {},
             Type::vec<VkObjectRef<ISemaphore>> waitSemaphores = {},
             Type::vec<VkObjectRef<ISemaphore>> signalSemaphores = {});
  void submit(VkObjectRef<IFence> fence = nullptr);
  void dispose();

  operator VkQueue() const override { return mQueue; }
};

class PresentQueue : public IQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(PresentQueue);

private:
  VkObjectRef<IDevice> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;

public:
  PresentQueue(VkObjectRef<IDevice> device);
  ~PresentQueue() override { this->dispose(); }

  void present(VkObjectRef<ISwapchain> swapchain,
               Type::vec<Type::u32> imageIndices,
               VkObjectRef<ISemaphore> waitSemaphore = nullptr);
  void dispose();

  operator VkQueue() const override { return mQueue; }
};
} // namespace Terreate::Core
