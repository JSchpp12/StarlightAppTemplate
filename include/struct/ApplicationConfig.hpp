#pragma once

#include <starlight/common/entities/Light.hpp>
#include <starlight/core/renderer/TimelineSynchronizer.hpp>
#include <starlight/core/device/DeviceContext.hpp>
#include <starlight/virtual/StarCamera.hpp>
#include <star_common/Renderer.hpp>

#include <functional>

struct ApplicationConfig
{
    using CameraLoader = std::function<std::shared_ptr<star::StarCamera>(star::core::device::DeviceContext &)>;
    using LightLoader = std::function<std::shared_ptr<std::vector<star::Light>>(star::core::device::DeviceContext &)>;
    using SceneLoader =
        std::function<std::vector<std::shared_ptr<star::StarObject>>(star::core::device::DeviceContext &)>;
    using MainRendererLoader = std::function<star::common::Renderer(
        star::core::device::DeviceContext &, std::vector<std::shared_ptr<star::StarObject>>,
        std::shared_ptr<std::vector<star::Light>>, std::shared_ptr<star::StarCamera>)>;
    using MainRendererSyncLoader =
        std::function<star::core::renderer::TimelineSynchronizer(star::core::device::DeviceContext &)>;

    CameraLoader cameraLoader;
    LightLoader lightLoader;
    SceneLoader sceneLoader;
    MainRendererLoader rendererLoader;
    MainRendererSyncLoader rendererSyncLoader;
};