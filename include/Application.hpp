#pragma once

#include <starlight/common/entities/Light.hpp>
#include <starlight/core/renderer/RendererBase.hpp>
#include <starlight/core/renderer/TimelineSynchronizer.hpp>
#include <starlight/templates/StarApplication.hpp>

#include <functional>

class Application : public star::StarApplication
{
  public:
    using CameraLoader = std::function<std::shared_ptr<star::StarCamera>(star::core::device::DeviceContext &)>;
    using LightLoader = std::function<std::shared_ptr<std::vector<star::Light>>(star::core::device::DeviceContext &)>;
    using SceneLoader =
        std::function<std::vector<std::shared_ptr<star::StarObject>>(star::core::device::DeviceContext &)>;
    using MainRendererLoader = std::function<star::common::Renderer(
        star::core::device::DeviceContext &, std::vector<std::shared_ptr<star::StarObject>>,
        std::shared_ptr<std::vector<star::Light>>, std::shared_ptr<star::StarCamera>)>;
    using MainRendererSyncLoader =
        std::function<star::core::renderer::TimelineSynchronizer(star::core::device::DeviceContext &)>;
    Application(CameraLoader cameraLoader, LightLoader lightLoader, SceneLoader sceneLoader,
                MainRendererLoader rendererLoader, MainRendererSyncLoader rendererSyncLoader)
        : m_mainRendererSync(0), m_cameraLoader(std::move(cameraLoader)), m_lightLoader(std::move(lightLoader)),
          m_sceneLoader(std::move(sceneLoader)), m_rendererLoader(std::move(rendererLoader)),
          m_rendererSyncLoader(std::move(rendererSyncLoader))
    {
    }

    virtual ~Application() = default;

    std::shared_ptr<star::StarScene> loadScene(star::core::device::DeviceContext &context) override;
    virtual void shutdown(star::core::device::DeviceContext &context) override;
    virtual void init() override;
    virtual void frameUpdate(star::core::SystemContext &context) override;

  private:
    star::core::renderer::TimelineSynchronizer m_mainRendererSync;
    CameraLoader m_cameraLoader;
    LightLoader m_lightLoader;
    SceneLoader m_sceneLoader;
    MainRendererLoader m_rendererLoader;
    MainRendererSyncLoader m_rendererSyncLoader;
    star::core::renderer::RendererBase *m_mainRenderer{nullptr};
};
