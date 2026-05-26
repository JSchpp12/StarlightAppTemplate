#include "StarEngine.hpp"

#include <star_common/helper/PathHelpers.hpp>
#include <starlight/common/ConfigFile.hpp>

#include <vulkan/vulkan.hpp>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#ifdef STAR_ENABLE_PRESENTATION
#include "WindowedMode.hpp"
void runWindowed()
{
    WindowedMode window;
    window.run();
}
#else
#include "WindowedMode.hpp"
void runHeadless()
{
}
#endif

int main()
{
    star::ConfigFile::load(star::common::paths::GetRuntimePath().parent_path() / "StarEngine.cfg");

#ifdef STAR_ENABLE_PRESENTATION
    runWindowed();
#else
    runHeadless();
#endif
}