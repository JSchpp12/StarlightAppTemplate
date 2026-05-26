#include "renderer/headless/Orchestrator.hpp"

#include "renderer/headless/GiveImageToTransfer.hpp"

namespace renderer::headless
{

void Orchestrator::prepRender(star::common::IDeviceContext &ctx)
{
    this->star::core::renderer::DefaultRenderer::prepRender(ctx);
}

star::core::device::manager::ManagerCommandBuffer::Request Orchestrator::getCommandBufferRequest()
{
    return star::core::device::manager::ManagerCommandBuffer::Request{
        .recordBufferCallback = std::bind(&Orchestrator::recordCommandBuffer, this, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3),
        .order = star::Command_Buffer_Order::main_render_pass,
        .orderIndex = star::Command_Buffer_Order_Index::first,
        .type = star::Queue_Type::Tgraphics,
        .waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .willBeSubmittedEachFrame = true,
        .recordOnce = false};
}

void Orchestrator::recordCommands(vk::CommandBuffer &commandBuffer, const star::common::FrameTracker &frameTracker,
                                  const uint64_t &frameIndex)
{
    this->star::core::renderer::DefaultRenderer::recordCommands(commandBuffer, frameTracker, frameIndex);

    //todo: Eventually use to do queue ownership transfers
    //GiveImageToTransfer transfer{
    //    .image = this->m_renderingContext.recordDependentImage
    //                 .get(this->m_renderToImages[frameTracker.getCurrent().getFinalTargetImageIndex()])
    //                 ->getVulkanImage(),
    //    .aspectFlags = vk::ImageAspectFlagBits::eColor,
    //    .srcImageLayout = vk::ImageLayout::eColorAttachmentOptimal,
    //    .dstImageLayout = vk::ImageLayout::eTransferSrcOptimal};

    //transfer.record(commandBuffer);
}

} // namespace renderer::headless