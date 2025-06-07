#include "../../include/util/frame.hpp"

namespace Terreate::Util {
Frame::Frame(Core::Context *context,
             Core::VkObjectRef<Core::ICommandPool> commandPool,
             Core::VkObjectRef<Core::IFramebuffer> framebuffer,
             Core::VkObjectRef<Core::ISwapchain> swapchain,
             Core::VkObjectRef<Core::GraphicQueue> graphic,
             Core::VkObjectRef<Core::PresentQueue> present,
             Core::VkObjectRef<Core::IPipeline> pipeline)
    : mFramebuffer(framebuffer), mSwapchain(swapchain), mGraphicQueue(graphic),
      mPresentQueue(present), mPipeline(pipeline) {
  mCommandBuffer = commandPool->createCommandBuffer();
  mImageAvailableSemaphore = context->createSemaphore();
  mRenderFinishedSemaphore = context->createSemaphore();
  mFence = context->createFence();
}

void Frame::begin(Type::vec<float> clearColor) {
  mFence->wait();
  mFence->reset();
  mCurrentImageIndex = mSwapchain->getNextImageIndex(mImageAvailableSemaphore);

  mCommandBuffer->begin();
  mCommandBuffer->setRenderPass((*mFramebuffer)[mCurrentImageIndex], clearColor,
                                Type::SubpassContent::INLINE);
  mCommandBuffer->setViewport(mViewportOffset[0], mViewportOffset[1],
                              mViewportSize[0], mViewportSize[1],
                              mViewportDepth[0], mViewportDepth[1]);
  mCommandBuffer->setScissor(mScissorOffset[0], mScissorOffset[1],
                             mScissorSize[0], mScissorSize[1]);
}

void Frame::setViewport(Type::vec<float> offset, Type::vec<float> size,
                        Type::vec<float> depth) {
  mViewportOffset = offset;
  mViewportSize = size;
  mViewportDepth = depth;
}

void Frame::setScissor(Type::vec<Type::u32> offset, Type::vec<Type::u32> size) {
  mScissorOffset = offset;
  mScissorSize = size;
}

void Frame::end() {
  mCommandBuffer->end();
  mGraphicQueue->queue({mCommandBuffer},
                       {Type::PipelineStage::COLOR_ATTACHMENT_OUTPUT_BIT},
                       {mImageAvailableSemaphore}, {mRenderFinishedSemaphore});
  mGraphicQueue->submit(mFence);
  mPresentQueue->present(mSwapchain, {mCurrentImageIndex},
                         mRenderFinishedSemaphore);
}
} // namespace Terreate::Util
