#include "WindowedMode.hpp"

#ifdef STAR_ENABLE_PRESENTATION

#include "Application.hpp"

#include <star_windowing/event/RequestSwapChainFromService.hpp>
#include <star_windowing/policy/EngineExitPolicy.hpp>
#include <star_windowing/policy/EngineInitPolicy.hpp>
#include <star_windowing/policy/EngineMainLoopPolicy.hpp>
#include <starlight/StarEngine.hpp>
#include <starlight/core/renderer/TimelineSynchronizer.hpp>

#include "loaders/CameraLoaders.hpp"
#include "loaders/LightLoaders.hpp"
#include "loaders/SceneLoaders.hpp"
#include <star_windowing/SwapChainRenderer.hpp>

void WindowedMode::run()
{
    using win_exit = star::windowing::EngineExitPolicy;
    using win_loop = star::windowing::EngineMainLoopPolicy;
    using win_init = star::windowing::EngineInitPolicy;

    star::windowing::WindowingContext winContext;
    Application app{[&winContext](star::core::device::DeviceContext &ctx) {
                        return loaders::LoadInteractiveCamera(ctx, &winContext);
                    },
                    loaders::LoadMainLight, loaders::LoadLionScene,
                    [&winContext](star::core::device::DeviceContext &ctx,
                                  std::vector<std::shared_ptr<star::StarObject>> objects,
                                  std::shared_ptr<std::vector<star::Light>> lights,
                                  std::shared_ptr<star::StarCamera> camera) -> star::common::Renderer {
                        vk::SwapchainKHR swapchain{VK_NULL_HANDLE};
                        ctx.getEventBus().emit(star::windowing::event::RequestSwapChainFromService{swapchain});

                        auto sc = star::common::Renderer{
                            star::windowing::SwapChainRenderer{&winContext, swapchain, ctx, objects, lights, camera}};
                        return sc;
                    },
                    [](const star::core::device::DeviceContext &ctx) -> star::core::renderer::TimelineSynchronizer {
                        return star::core::renderer::TimelineSynchronizer(
                            ctx.frameTracker().getSetup().getNumUniqueTargetFramesForFinalization());
                    }};
    star::StarEngine<win_init, win_loop, win_exit> engine{win_init{winContext}, win_loop{winContext},
                                                          win_exit{winContext}, app};

    engine.run();
}
#endif