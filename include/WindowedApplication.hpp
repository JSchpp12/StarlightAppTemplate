#pragma once

#ifdef STAR_ENABLE_PRESENTATION

#include "Application.hpp"

#include <star_windowing/WindowingContext.hpp>

class WindowedApplication : public Application
{
  public:
    WindowedApplication(CameraLoader cameraLoader, LightLoader lightLoader, SceneLoader sceneLoader,
                        star::windowing::WindowingContext *winContext)
        : Application(std::move(cameraLoader), std::move(lightLoader), std::move(sceneLoader)), m_winContext(winContext)
    {
    }

    virtual ~WindowedApplication() = default;

  protected:
    void initListeners(star::core::device::DeviceContext &context) override;

    std::unique_ptr<star::core::renderer::IRenderPhaseProvider> createMainRenderer(
        star::core::device::DeviceContext &context, std::vector<std::shared_ptr<star::StarObject>> objects,
        std::shared_ptr<star::StarCamera> camera) override;

  private:
    star::windowing::WindowingContext *m_winContext = nullptr;
};

#endif