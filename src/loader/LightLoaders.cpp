#include "loader/LightLoaders.hpp"

#include <starlight/command/CreateLight.hpp>

namespace loader
{
std::shared_ptr<std::vector<star::Light>> LoadMainLight(star::core::device::DeviceContext &ctx)
{
    star::command::CreateLight lCmd = star::command::CreateLight().setName("main");
    ctx.getCmdBus().submit(lCmd);
    auto [addResult, light] = lCmd.getReply().get();

    return light;
}
}