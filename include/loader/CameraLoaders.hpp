#pragma once

#include <starlight/core/device/DeviceContext.hpp>
#include <starlight/virtual/StarCamera.hpp>

#ifdef STAR_ENABLE_PRESENTATION
#include <star_windowing/WindowingContext.hpp>
#endif

namespace loader
{
#ifdef STAR_ENABLE_PRESENTATION
std::shared_ptr<star::StarCamera> LoadInteractiveCamera(star::core::device::DeviceContext &ctx,
                                                        star::windowing::WindowingContext *winContext);
#endif

std::shared_ptr<star::StarCamera> LoadHeadlessCamera(star::core::device::DeviceContext &ctx);
} // namespace loaders