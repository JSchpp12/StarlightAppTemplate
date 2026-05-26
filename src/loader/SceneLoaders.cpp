#include "loader/SceneLoaders.hpp"

#include <starlight/command/CreateObject.hpp>
#include <starlight/command/detail/create_object/DirectObjCreation.hpp>
#include <starlight/common/ConfigFile.hpp>
#include <starlight/common/objects/BasicObject.hpp>
#include <starlight/core/Exceptions.hpp>

#include <filesystem>
#include <glm/fwd.hpp>
#include <sstream>

static std::shared_ptr<star::StarObject> LoadLion(const star::core::device::DeviceContext &ctx,
                                                  const std::filesystem::path &mediaPath)
{
    const auto path = mediaPath / "models" / "lion-statue" / "source" / "rapid.obj";

    auto cmd = star::command::CreateObject::Builder()
                   .setLoader(std::make_unique<star::command::create_object::DirectObjCreation>(
                       std::make_shared<star::BasicObject>(path.string())))
                   .setUniqueName("lion")
                   .build();

    ctx.getCmdBus().submit(cmd);
    return cmd.getReply().get();
}

static std::shared_ptr<star::StarObject> LoadPlant(const star::core::device::DeviceContext &ctx,
                                                   const std::filesystem::path &mediaPath)
{
    const auto path = mediaPath / "models" / "aloevera" / "aloevera.obj";

    if (!std::filesystem::is_regular_file(path))
    {
        std::ostringstream oss;
        oss << "Failed to load plant file with path: " << path.string();
        STAR_THROW(oss.str());
    }

    auto cmd = star::command::CreateObject::Builder()
                   .setLoader(std::make_unique<star::command::create_object::DirectObjCreation>(
                       std::make_shared<star::BasicObject>(path.string())))
                   .setUniqueName("aloevera")
                   .build();

    ctx.getCmdBus().submit(cmd);
    return cmd.getReply().get();
}

std::vector<std::shared_ptr<star::StarObject>> loader::LoadLionScene(star::core::device::DeviceContext &ctx)
{
    const std::filesystem::path mediaPath =
        std::filesystem::canonical(star::ConfigFile::getSetting(star::Config_Settings::mediadirectory));
    auto lion = LoadLion(ctx, mediaPath);
    lion->init(ctx);

    {
        auto &li = lion->getInstance();
        li.setScale(glm::vec3{0.04f, 0.04f, 0.04f});
        li.setPosition(glm::vec3{0.0, -1.0, 0.0});
        li.rotateGlobal(star::Type::Axis::x, -90);
    }
    auto plant = LoadPlant(ctx, mediaPath);
    plant->init(ctx);

    std::vector<std::shared_ptr<star::StarObject>> objects{std::move(lion), std::move(plant)};
    return objects;
}
