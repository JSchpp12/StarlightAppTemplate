#pragma once

#ifndef STAR_ENABLE_PRESENTATION

#include "Application.hpp"

#include <starlight/core/renderer/HeadlessRenderPhaseProvider.hpp>
#include <starlight/event/RegisterMainGraphicsRenderer.hpp>

/// Finalization provider for headless runs. Identical to the stock
/// HeadlessRenderPhaseProvider, except that the built phase registers itself as
/// the engine's main graphics renderer so the HeadlessRenderResultWriteService
/// can capture each frame's output (build() runs during scene prepRender,
/// before the first StartOfNextFrame).
class HeadlessFinalizationProvider : public star::core::renderer::HeadlessRenderPhaseProvider
{
  public:
    using HeadlessRenderPhaseProvider::HeadlessRenderPhaseProvider;

    std::unique_ptr<star::core::renderer::RenderPhase> build(star::core::device::DeviceContext &context,
                                                             star::core::renderer::RenderPhaseRegistry &phases) override
    {
        auto phase = HeadlessRenderPhaseProvider::build(context, phases);

        context.getEventBus().emit(star::event::RegisterMainGraphicsRenderer{phase.get()});

        return phase;
    }
};

class HeadlessApplication : public Application
{
  public:
    HeadlessApplication(CameraLoader cameraLoader, LightLoader lightLoader, SceneLoader sceneLoader)
        : Application(std::move(cameraLoader), std::move(lightLoader), std::move(sceneLoader))
    {
    }

    virtual ~HeadlessApplication() = default;

  protected:
    std::unique_ptr<star::core::renderer::IRenderPhaseProvider> createMainRenderer(
        star::core::device::DeviceContext &context, std::vector<std::shared_ptr<star::StarObject>> objects,
        std::shared_ptr<star::StarCamera> camera) override
    {
        return std::make_unique<HeadlessFinalizationProvider>(context, m_mainLight, camera, std::move(objects));
    }
};

#endif