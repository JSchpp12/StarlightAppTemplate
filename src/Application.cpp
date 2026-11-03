#include "Application.hpp"

#include <starlight/command/CreateLight.hpp>
#include <starlight/command/SaveSceneState.hpp>
#include <starlight/core/logging/LoggingFactory.hpp>
#include <starlight/virtual/StarScene.hpp>

#include <sstream>

std::shared_ptr<star::StarScene> Application::loadScene(star::core::device::DeviceContext &context)
{
    initListeners(context);

    auto camera = createMainCamera(context);

    {
        // Named lvalue: the rvalue overload is rejected for reply-carrying commands.
        star::command::CreateLight lCmd = star::command::CreateLight().setName("main");
        context.getCmdBus().submit(lCmd);
        auto [addResult, light] = lCmd.getReply().get();
        m_mainLight = light;
        if (addResult == star::command::create_light::fail)
        {
            star::core::logging::warning("Light command reported failure; installing fallback 'main' light");
            m_mainLight->emplace_back(star::Light()
                                          .setPosition({0.0, 4.0, 0.0})
                                          .setType(star::Type::Light::directional)
                                          .setAmbient({1.0f, 1.0f, 1.0f})
                                          .setLuminance(20)
                                          .setDirection({0.0f, -1.0f, 0.0f}));
        }
    }

    auto objects = m_sceneLoader(context);

    auto provider = createMainRenderer(context, objects, camera);

    // The same objects vector feeds both the readiness policy and the provider
    // (shared_ptrs, so both observe the same objects).
    m_mainScene = std::make_shared<star::StarScene>(star::star_scene::makeWaitForAllObjectsReadyPolicy(objects),
                                                    std::move(camera));

    m_mainScene->addProvider(std::move(provider));

    return m_mainScene;
}

void Application::shutdown(star::core::device::DeviceContext &context)
{
    // Exercises the SceneLoaderService; a missing scene file on disk only warns.
    auto cmd = star::command::SaveSceneState();
    context.begin().set(cmd).submit();
}