#include "Application.hpp"

#include <starlight/command/command_order/TriggerPass.hpp>
#include <starlight/virtual/StarScene.hpp>

using namespace star;

static void TriggerSubmissionOfMainDraw(const star::core::device::DeviceContext &ctx, const star::Handle &registration,
                                        const star::Handle &timelineSemaphoreToUse)
{
    const size_t ii = static_cast<size_t>(ctx.frameTracker().getCurrent().getFrameInFlightIndex());
    const size_t signalValue = static_cast<size_t>(ctx.frameTracker().getCurrent().getNumTimesFrameProcessed()) + 1;

    ctx.getCmdBus().submit(star::command_order::TriggerPass()
                               .setPass(registration)
                               .setTimelineSemaphore(timelineSemaphoreToUse)
                               .setSignalValue(signalValue));
}

std::shared_ptr<star::StarScene> Application::loadScene(star::core::device::DeviceContext &context)
{
    auto camera = m_cameraLoader(context);
    auto objects = m_sceneLoader(context);
    auto lights = m_lightLoader(context);
    auto renderer = m_rendererLoader(context, objects, lights, camera);
    m_mainRenderer = renderer.getRawBase(); 
    m_mainRendererSync = m_rendererSyncLoader(context);
    m_mainRendererSync.prepRender(context);

    return std::make_shared<star::StarScene>(star::star_scene::makeAlwaysReadyPolicy(), std::move(camera),
                                             std::move(renderer));
}

void Application::shutdown(star::core::device::DeviceContext &context)
{
}

void Application::init()
{
}

void Application::frameUpdate(star::core::SystemContext &context)
{
    const auto &d = context.getAllDevices().getData()[0];
    TriggerSubmissionOfMainDraw(
        d, m_mainRenderer->getCommandBuffer(),
        m_mainRendererSync.getSemaphores()[d.frameTracker().getCurrent().getFinalTargetImageIndex()]);
}