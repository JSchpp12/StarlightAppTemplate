#pragma once

#include <starlight/core/device/DeviceContext.hpp>
#include <starlight/object/StarObject.hpp>

#include <memory>
#include <vector>

namespace loaders
{
std::vector<std::shared_ptr<star::StarObject>> LoadLionScene(star::core::device::DeviceContext &context);
}