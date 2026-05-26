#include "loader/RenderLoaders.hpp"

#ifndef STAR_ENABLE_PRESENTATION

#include "renderer/headless/Orchestrator.hpp"

#include <starlight/event/RegisterMainGraphicsRenderer.hpp>

star::common::Renderer loader::loadHeadlessRenderer(star::core::device::DeviceContext &ctx,
                                                    std::vector<std::shared_ptr<star::StarObject>> objects,
                                                    std::shared_ptr<std::vector<star::Light>> lights,
                                                    std::shared_ptr<star::StarCamera> camera)
{
    star::common::Renderer sc{renderer::headless::Orchestrator{ctx, lights, camera, objects}};

    ctx.getEventBus().emit(star::event::RegisterMainGraphicsRenderer{sc.getRawBase()});

    return sc;
}

#endif