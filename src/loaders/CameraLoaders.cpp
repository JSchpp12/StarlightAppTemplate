#include "loaders/CameraLoaders.hpp"

#include <glm/fwd.hpp>
#include <star_windowing/BasicCamera.hpp>
#include <star_windowing/InteractivityBus.hpp>

std::shared_ptr<star::StarCamera> loaders::LoadInteractiveCamera(star::core::device::DeviceContext &ctx,
                                                                 star::windowing::WindowingContext *winContext)
{
    star::windowing::InteractivityBus::Init(&ctx.getEventBus(), winContext);

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

    return camera;
}

std::shared_ptr<star::StarCamera> loaders::LoadHeadlessCamera(star::core::device::DeviceContext &ctx)
{
    return std::make_shared<star::StarCamera>(ctx.getEngineResolution().width, ctx.getEngineResolution().height, 90.0f,
                                              1.0f, 10.0f);
}
