#include "StarEngine.hpp"

#include "interface/IRunMode.hpp"

#include <star_common/helper/PathHelpers.hpp>
#include <starlight/common/ConfigFile.hpp>

#include <vulkan/vulkan.hpp>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#ifdef STAR_ENABLE_PRESENTATION

#include "run_mode/Windowed.hpp"
static WindowedMode setWindowed()
{
    return WindowedMode();
}

#else

#include "run_mode/Headless.hpp"
static Headless setHeadless()
{
    return Headless();
}

#endif

int main()
{
    star::ConfigFile::load(star::common::paths::GetRuntimePath().parent_path() / "StarEngine.cfg");

#ifdef STAR_ENABLE_PRESENTATION
    auto mode = setWindowed();
#else
    auto mode = setHeadless();
#endif

    static_cast<IRunMode &>(mode).run();
}