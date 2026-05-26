#include "Application.hpp"

#include <starlight/command/command_order/TriggerPass.hpp>
#include <starlight/event/TriggerScreenshot.hpp>
#include <starlight/virtual/StarScene.hpp>

using namespace star;

void Application::TriggerSubmissionOfMainDraw(const star::core::device::DeviceContext &ctx,
                                              const star::Handle &registration,
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
    m_screenshotRegistrations.resize(context.frameTracker().getSetup().getNumUniqueTargetFramesForFinalization());

    auto camera = m_config.cameraLoader(context);
    {
        const auto position = glm::vec3{-2.0, 1.0f, -2.0f};
        camera->setPosition(position);
        camera->setForwardVector(glm::normalize(-position));
    }

    auto objects = m_config.sceneLoader(context);
    auto lights = m_config.lightLoader(context);
    auto renderer = m_config.rendererLoader(context, objects, lights, camera);
    m_mainRenderer = renderer.getRawBase();
    m_mainRendererSync = m_config.rendererSyncLoader(context);
    m_mainRendererSync.prepRender(context);
    m_screenshotRegistrations.resize(1);

    return std::make_shared<star::StarScene>(star::star_scene::makeWaitForAllObjectsReadyPolicy(objects),
                                             std::move(camera), std::move(renderer));
}

void Application::shutdown(star::core::device::DeviceContext &context)
{
}

void Application::init()
{
}

void Application::frameUpdate(star::core::SystemContext &ctx)
{
    auto &d = ctx.getAllDevices().getData()[0];
    TriggerSubmissionOfMainDraw(
        d, m_mainRenderer->getCommandBuffer(),
        m_mainRendererSync.getSemaphores()[d.frameTracker().getCurrent().getFinalTargetImageIndex()]);
    triggerScreenshot(d);
}

void Application::triggerScreenshot(star::core::device::DeviceContext &context)
{
    //const auto &frameTracker = context.frameTracker();
    //const std::string name = "ImageCapture.png";

    //size_t index = static_cast<size_t>(frameTracker.getCurrent().getFinalTargetImageIndex());

    //// submit screenshot processing
    //context.getEventBus().emit(star::event::TriggerScreenshot(
    //    context.getImageManager().get(m_mainRenderer->getRenderToColorImages()[index])->texture, name,
    //    m_mainRenderer->getCommandBuffer(), m_screenshotRegistrations[index]));
}
