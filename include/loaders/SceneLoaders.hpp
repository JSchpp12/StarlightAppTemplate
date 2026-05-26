#pragma once

#include <starlight/core/device/DeviceContext.hpp>
#include <starlight/virtual/StarObject.hpp>

#include <vector>

namespace loaders
{
std::vector<std::shared_ptr<star::StarObject>> LoadLionScene(star::core::device::DeviceContext &context);
}