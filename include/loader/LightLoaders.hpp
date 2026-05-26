#pragma once

#include <starlight/common/entities/Light.hpp>
#include <starlight/core/device/DeviceContext.hpp>

#include <memory>

namespace loader
{

std::shared_ptr<std::vector<star::Light>> LoadMainLight(star::core::device::DeviceContext &ctx);

}