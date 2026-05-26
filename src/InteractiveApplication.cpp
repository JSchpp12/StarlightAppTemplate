#include "InteractiveApplication.hpp"
#ifdef STAR_ENABLE_PRESENTATION

#include <GLFW/glfw3.h>

std::shared_ptr<star::StarScene> InteractiveApplication::loadScene(star::core::device::DeviceContext &context)
{
    m_listenKeyRelease.init(context.getEventBus());

    return this->Application::loadScene(context);
}

void InteractiveApplication::shutdown(star::core::device::DeviceContext &context)
{
}

void InteractiveApplication::onKeyRelease(const star::windowing::event::KeyRelease &event, bool &keepAlive)
{
}

void InteractiveApplication::frameUpdate(star::core::SystemContext &ctx)
{
    auto &d = ctx.getAllDevices().getData()[0];
    TriggerSubmissionOfMainDraw(
        d, m_mainRenderer->getCommandBuffer(),
        m_mainRendererSync.getSemaphores()[d.frameTracker().getCurrent().getFinalTargetImageIndex()]);
}
#endif