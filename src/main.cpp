#include <star_common/helper/PathHelpers.hpp>
#include <starlight/common/ConfigFile.hpp>
#include <starlight/core/Exceptions.hpp>
#include <starlight/core/logging/LoggingFactory.hpp>

#include <vulkan/vulkan.hpp>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#ifdef STAR_ENABLE_PRESENTATION
#include "WindowedMode.hpp"
#else
#include "HeadlessMode.hpp"
#endif

int main()
{
    star::core::install_terminate_handler();

    star::ConfigFile::load(star::common::paths::GetRuntimePath().parent_path() / "StarEngine.cfg");

#ifdef STAR_ENABLE_PRESENTATION
    star::core::logging::info("Starting starlight_app_template in windowed mode");
    WindowedMode windowedInstance;
    const int result = windowedInstance.run();
#else
    star::core::logging::info("Starting starlight_app_template in headless mode");
    HeadlessMode headlessInstance;
    const int result = headlessInstance.run();
#endif

    star::core::logging::info("Application exiting");
    return result;
}