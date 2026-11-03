#pragma once

#include <starlight/common/entities/Light.hpp>
#include <starlight/core/renderer/IRenderPhaseProvider.hpp>
#include <starlight/templates/StarApplication.hpp>

#include <functional>
#include <memory>
#include <vector>

class Application : public star::StarApplication
{
  public:
    using CameraLoader = std::function<std::shared_ptr<star::StarCamera>(star::core::device::DeviceContext &)>;
    using LightLoader = std::function<std::shared_ptr<std::vector<star::Light>>(star::core::device::DeviceContext &)>;
    using SceneLoader =
        std::function<std::vector<std::shared_ptr<star::StarObject>>(star::core::device::DeviceContext &)>;

    Application(CameraLoader cameraLoader, LightLoader lightLoader, SceneLoader sceneLoader)
        : m_cameraLoader(std::move(cameraLoader)), m_lightLoader(std::move(lightLoader)),
          m_sceneLoader(std::move(sceneLoader))
    {
    }

    virtual ~Application() = default;

    void init() override
    {
    }

    std::shared_ptr<star::StarScene> loadScene(star::core::device::DeviceContext &context) override;
    virtual void shutdown(star::core::device::DeviceContext &context) override;
    virtual void frameUpdate(star::core::SystemContext &context) override
    {
        // Render phases submit their own pass every frame; nothing to drive here.
    }

  protected:
    virtual std::shared_ptr<star::StarCamera> createMainCamera(star::core::device::DeviceContext &context)
    {
        return m_cameraLoader(context);
    }

    virtual std::unique_ptr<star::core::renderer::IRenderPhaseProvider> createMainRenderer(
        star::core::device::DeviceContext &context, std::vector<std::shared_ptr<star::StarObject>> objects,
        std::shared_ptr<star::StarCamera> camera) = 0;

    virtual void initListeners(star::core::device::DeviceContext &context)
    {
    }

    CameraLoader m_cameraLoader;
    LightLoader m_lightLoader;
    SceneLoader m_sceneLoader;
    std::shared_ptr<star::StarScene> m_mainScene{nullptr};
    std::shared_ptr<std::vector<star::Light>> m_mainLight{nullptr};
};