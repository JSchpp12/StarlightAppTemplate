#include "loaders/CameraLoaders.hpp"

#include <starlight/core/logging/LoggingFactory.hpp>

#include <glm/glm.hpp>

#ifdef STAR_ENABLE_PRESENTATION

#include <star_windowing/BasicCamera.hpp>

std::shared_ptr<star::StarCamera> loaders::LoadInteractiveCamera(star::core::device::DeviceContext &ctx,
                                                                 star::windowing::WindowingContext *winContext)
{
    auto camera = star::windowing::BasicCamera::Builder()
                      .setWidth(ctx.getEngineResolution().width)
                      .setHeight(ctx.getEngineResolution().height)
                      .setHorizontalFieldOfView(90.0f)
                      .setNearClippingPlaneDistance(0.001f)
                      .setFarClippingPlaneDistance(250.0f)
                      .setMovementSpeed(2.0f)
                      .setSensitivity(0.1f)
                      .buildShared();

    camera->init(ctx.getEventBus());

    {
        const auto position = glm::vec3{-2.0, 1.0f, -2.0f};
        camera->setPosition(position);
        camera->setForwardVector(glm::normalize(-position));
    }

    star::core::logging::info("Interactive camera created");

    return camera;
}

#endif

std::shared_ptr<star::StarCamera> loaders::LoadHeadlessCamera(star::core::device::DeviceContext &ctx)
{
    auto camera = std::make_shared<star::StarCamera>(ctx.getEngineResolution().width, ctx.getEngineResolution().height,
                                                     90.0f, 1.0f, 250.0f);

    // Frame the lion scene the same way the interactive camera does, so headless
    // captures show the same content (the default position sits inside the near
    // clip of both demo models).
    {
        const auto position = glm::vec3{-2.0, 1.0f, -2.0f};
        camera->setPosition(position);
        camera->setForwardVector(glm::normalize(-position));
    }

    star::core::logging::info("Headless camera created");

    return camera;
}