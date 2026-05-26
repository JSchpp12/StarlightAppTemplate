#include "renderer/headless/GiveImageToTransfer.hpp"

void renderer::GiveImageToTransfer::record(vk::CommandBuffer cmdBuffer)
{
    const auto imgBarrier = makeImgBarrier();
    cmdBuffer.pipelineBarrier2(vk::DependencyInfo().setImageMemoryBarriers(imgBarrier));
}

vk::ImageMemoryBarrier2 renderer::GiveImageToTransfer::makeImgBarrier() const
{
    return vk::ImageMemoryBarrier2()
        .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
        .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
        .setDstAccessMask(vk::AccessFlagBits2::eNone)
        .setSubresourceRange(vk::ImageSubresourceRange()
                                 .setAspectMask(aspectFlags)
                                 .setBaseArrayLayer(0)
                                 .setLayerCount(vk::RemainingArrayLayers)
                                 .setBaseMipLevel(0)
                                 .setLevelCount(vk::RemainingMipLevels))
        .setImage(image);
}
