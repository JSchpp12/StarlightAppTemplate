#include "WindowedApplication.hpp"

#ifdef STAR_ENABLE_PRESENTATION

#include <star_windowing/InteractivityBus.hpp>
#include <star_windowing/SwapChainRenderPhaseProvider.hpp>
#include <star_windowing/event/RequestSwapChainFromService.hpp>

void WindowedApplication::initListeners(star::core::device::DeviceContext &context)
{
    star::windowing::InteractivityBus::Init(&context.getEventBus(), m_winContext);
}

std::unique_ptr<star::core::renderer::IRenderPhaseProvider> WindowedApplication::createMainRenderer(
    star::core::device::DeviceContext &context, std::vector<std::shared_ptr<star::StarObject>> objects,
    std::shared_ptr<star::StarCamera> camera)
{
    vk::SwapchainKHR swapchain{VK_NULL_HANDLE};
    context.getEventBus().emit(star::windowing::event::RequestSwapChainFromService{swapchain});

    return std::make_unique<star::windowing::SwapChainRenderPhaseProvider>(m_winContext, swapchain, context,
                                                                           std::move(objects), m_mainLight, camera);
}

#endif