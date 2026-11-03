#include "WindowedMode.hpp"

#ifdef STAR_ENABLE_PRESENTATION

#include "WindowedApplication.hpp"

#include <star_windowing/policy/EngineExitPolicy.hpp>
#include <star_windowing/policy/EngineInitPolicy.hpp>
#include <star_windowing/policy/EngineMainLoopPolicy.hpp>
#include <star_windowing/service/EngineServicesFactory.hpp>
#include <starlight/StarEngine.hpp>
#include <starlight/service/EngineServicesFactory.hpp>

#include "loaders/CameraLoaders.hpp"
#include "loaders/LightLoaders.hpp"
#include "loaders/SceneLoaders.hpp"

int WindowedMode::run()
{
    using win_exit = star::windowing::EngineExitPolicy;
    using win_loop = star::windowing::EngineMainLoopPolicy;
    using win_init = star::windowing::EngineInitPolicy;

    star::windowing::WindowingContext winContext;

    auto engineServices = star::service::EngineServicesFactory::createDefault(false);
    star::windowing::EngineServicesFactory::addDefaultServices(engineServices, winContext);

    star::windowing::EngineInitPolicy windowInit{winContext, engineServices};
    win_loop windowLoop{winContext};
    win_exit windowExit{winContext};

    WindowedApplication application{[&winContext](star::core::device::DeviceContext &ctx) {
                                        return loaders::LoadInteractiveCamera(ctx, &winContext);
                                    },
                                    loaders::LoadMainLight, loaders::LoadLionScene, &winContext};

    star::StarEngine<win_init, win_loop, win_exit> engine{std::move(windowInit), std::move(windowLoop),
                                                          std::move(windowExit), application};

    engine.run();

    return 0;
}
#endif