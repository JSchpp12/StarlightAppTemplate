#include "HeadlessMode.hpp"

#ifndef STAR_ENABLE_PRESENTATION

#include "HeadlessApplication.hpp"

#include <starlight/StarEngine.hpp>
#include <starlight/policy/DefaultEngineInitPolicy.hpp>
#include <starlight/policy/DefaultEngineLoopPolicy.hpp>
#include <starlight/policy/EngineExitAfterNumberOfFrames.hpp>
#include <starlight/service/EngineServicesFactory.hpp>

#include "loaders/CameraLoaders.hpp"
#include "loaders/LightLoaders.hpp"
#include "loaders/SceneLoaders.hpp"

int HeadlessMode::run()
{
    using loop = star::policy::DefaultEngineLoopPolicy;
    using exit = star::policy::EngineExitAfterNumberOfFrames;

    HeadlessApplication application{loaders::LoadHeadlessCamera, loaders::LoadMainLight, loaders::LoadLionScene};

    auto engineServices = star::service::EngineServicesFactory::createDefault(true);

    star::policy::DefaultEngineInitPolicy init{engineServices, {}, std::nullopt};
    loop engineLoop{};
    exit engineExit{10};

    star::StarEngine<star::policy::DefaultEngineInitPolicy, loop, exit> engine{std::move(init), std::move(engineLoop),
                                                                               std::move(engineExit), application};

    engine.run();

    return 0;
}
#endif