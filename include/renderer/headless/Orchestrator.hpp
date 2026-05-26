#pragma once

#include <StarCommandBuffer.hpp>
#include <device/managers/ManagerCommandBuffer.hpp>
#include <starlight/core/renderer/DefaultRenderer.hpp>
#include <starlight/core/renderer/RenderingTargetInfo.hpp>

namespace renderer::headless
{
class Orchestrator : public star::core::renderer::DefaultRenderer
{
  public:
    Orchestrator(star::core::device::DeviceContext &context, std::shared_ptr<std::vector<star::Light>> lights,
                 std::shared_ptr<star::StarCamera> camera, std::vector<std::shared_ptr<star::StarObject>> objects)
        : star::core::renderer::DefaultRenderer(context, lights, camera, objects)
    {
    }
    virtual void prepRender(star::common::IDeviceContext &device) override;
    virtual star::core::device::manager::ManagerCommandBuffer::Request getCommandBufferRequest() override;
    virtual void recordCommands(vk::CommandBuffer &commandBuffer, const star::common::FrameTracker &frameTracker,
                                const uint64_t &frameIndex) override;
};
} // namespace renderer