#include "run_mode/Windowed.hpp"

#ifdef STAR_ENABLE_PRESENTATION

#include "InteractiveApplication.hpp"
#include "struct/ApplicationConfig.hpp"

#include <star_windowing/event/RequestSwapChainFromService.hpp>
#include <star_windowing/policy/EngineExitPolicy.hpp>
#include <star_windowing/policy/EngineInitPolicy.hpp>
#include <star_windowing/policy/EngineMainLoopPolicy.hpp>
#include <starlight/StarEngine.hpp>
#include <starlight/core/renderer/TimelineSynchronizer.hpp>

#include "loader/CameraLoaders.hpp"
#include "loader/LightLoaders.hpp"
#include "loader/SceneLoaders.hpp"

#include <star_windowing/SwapChainRenderer.hpp>

void WindowedMode::run()
{
    using win_exit = star::windowing::EngineExitPolicy;
    using win_loop = star::windowing::EngineMainLoopPolicy;
    using win_init = star::windowing::EngineInitPolicy;

    star::windowing::WindowingContext winContext;
    InteractiveApplication app{ApplicationConfig{
        [&winContext](star::core::device::DeviceContext &ctx) {
            return loader::LoadInteractiveCamera(ctx, &winContext);
        },
        loader::LoadMainLight, loader::LoadLionScene,
        [&winContext](star::core::device::DeviceContext &ctx, std::vector<std::shared_ptr<star::StarObject>> objects,
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
        }}};
        star::StarEngine<win_init, win_loop, win_exit> engine{win_init{winContext}, win_loop{winContext},
                                                              win_exit{winContext}, app};

        engine.run();
    }
#endif