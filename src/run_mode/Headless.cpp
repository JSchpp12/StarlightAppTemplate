#include "run_mode/Headless.hpp"

#ifndef STAR_ENABLE_PRESENTATION
#include "Application.hpp"
#include "loader/CameraLoaders.hpp"
#include "loader/LightLoaders.hpp"
#include "loader/RenderLoaders.hpp"
#include "loader/SceneLoaders.hpp"
#include "struct/ApplicationConfig.hpp"

#include <starlight/StarEngine.hpp>
#include <starlight/policy/DefaultEngineInitPolicy.hpp>
#include <starlight/policy/DefaultEngineLoopPolicy.hpp>
#include <starlight/policy/EngineExitAfterNumberOfFrames.hpp>

void Headless::run()
{
    using h_exit = star::policy::EngineExitAfterNumberOfFrames;
    using h_loop = star::policy::DefaultEngineLoopPolicy;
    using h_init = star::policy::DefaultEngineInitPolicy;

    Application app{ApplicationConfig{.cameraLoader = loader::LoadHeadlessCamera,
                                      .lightLoader = loader::LoadMainLight,
                                      .sceneLoader = loader::LoadLionScene,
                                      .rendererLoader = loader::loadHeadlessRenderer,
                                      .rendererSyncLoader = {[](const star::core::device::DeviceContext &ctx)
                                                                 -> star::core::renderer::TimelineSynchronizer {
                                          return star::core::renderer::TimelineSynchronizer(
                                              ctx.frameTracker().getSetup().getNumUniqueTargetFramesForFinalization());
                                      }}}};

    star::StarEngine engine{h_init{}, h_loop{}, h_exit{1}, app};
    engine.run();
}

#endif