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

class IGraphicQueue : public IQueue {
public:
  virtual void
  queue(Type::vec<VkObjectRef<ICommandBuffer>> commandBuffers,
        Type::vec<Type::PipelineStage> waitStages = {},
        Type::vec<VkObjectRef<ISemaphore>> waitSemaphores = {},
        Type::vec<VkObjectRef<ISemaphore>> signalSemaphores = {}) = 0;
  virtual void submit(VkObjectRef<IFence> fence = nullptr) = 0;
};

class IPresentQueue : public IQueue {
public:
  virtual void present(VkObjectRef<ISwapchain> swapchain,
                       Type::vec<Type::u32> imageIndices,
                       VkObjectRef<ISemaphore> waitSemaphore = nullptr) = 0;
};

class GraphicQueue : public IGraphicQueue {
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
  ~GraphicQueue() override;

  void queue(Type::vec<VkObjectRef<ICommandBuffer>> commandBuffers,
             Type::vec<Type::PipelineStage> waitStages = {},
             Type::vec<VkObjectRef<ISemaphore>> waitSemaphores = {},
             Type::vec<VkObjectRef<ISemaphore>> signalSemaphores = {}) override;
  void submit(VkObjectRef<IFence> fence = nullptr) override;

  operator VkQueue() const override { return mQueue; }
};

class PresentQueue : public IPresentQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(PresentQueue);

private:
  VkObjectRef<Device> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;

public:
  PresentQueue(VkObjectRef<Device> device);
  ~PresentQueue() override;

  void present(VkObjectRef<ISwapchain> swapchain,
               Type::vec<Type::u32> imageIndices,
               VkObjectRef<ISemaphore> waitSemaphore = nullptr) override;

  operator VkQueue() const override { return mQueue; }
};
} // namespace Terreate::Core
