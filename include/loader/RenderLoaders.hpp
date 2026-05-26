#pragma once

#include <Light.hpp>
#include <StarCamera.hpp>
#include <StarObject.hpp>
#include <memory>
#include <star_common/Renderer.hpp>
#include <starlight/core/device/DeviceContext.hpp>
#include <vector>

namespace loader
{

#ifndef STAR_ENABLE_PRESENTATION
star::common::Renderer loadHeadlessRenderer(star::core::device::DeviceContext &ctx,
                                            std::vector<std::shared_ptr<star::StarObject>> objects,
                                            std::shared_ptr<std::vector<star::Light>> lights,
                                            std::shared_ptr<star::StarCamera> camera);
#endif
} // namespace loader