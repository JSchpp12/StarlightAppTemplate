#include "loaders/SceneLoaders.hpp"

#include <starlight/ShaderResolver.hpp>
#include <starlight/command/CreateObject.hpp>
#include <starlight/command/detail/create_object/FromObjFileLoader.hpp>
#include <starlight/common/ConfigFile.hpp>
#include <starlight/core/Exceptions.hpp>
#include <starlight/core/logging/LoggingFactory.hpp>
#include <starlight/object/BasicObject.hpp>

#include <filesystem>
#include <glm/glm.hpp>
#include <sstream>

static std::shared_ptr<star::StarObject> LoadObj(star::core::device::DeviceContext &ctx,
                                                 const std::filesystem::path &path, const std::string &name)
{
    if (!std::filesystem::is_regular_file(path))
    {
        std::ostringstream oss;
        oss << "Failed to load object file with path: " << path.string();
        star::core::logging::error(oss.str());
        STAR_THROW(oss.str());
    }

    star::core::logging::info("Loading object '" + name + "' from " + path.string());

    auto resolver = star::BasicObject::PrepareResolver(path.string(), ctx.getCmdBus());

    auto cmd = star::command::CreateObject::Builder()
                   .setLoader(std::make_unique<star::command::create_object::FromObjFileLoader>(path.string()))
                   .setShaderResolver(std::move(resolver))
                   .setUniqueName(name)
                   .build();

    ctx.begin().set(cmd).submit();
    cmd.getReply().get()->init(ctx);

    return cmd.getReply().get();
}

std::vector<std::shared_ptr<star::StarObject>> loaders::LoadLionScene(star::core::device::DeviceContext &ctx)
{
    // starlight forces exe-relative media; no canonical() — it throws when
    // prepare_media has not been run.
    const std::filesystem::path mediaPath{star::ConfigFile::getSetting(star::Config_Settings::mediadirectory)};

    auto lion = LoadObj(ctx, mediaPath / "models" / "lion-statue" / "source" / "rapid.obj", "lion");
    {
        auto &li = lion->getInstance();
        li.setScale(glm::vec3{0.04f, 0.04f, 0.04f});
        li.setPosition(glm::vec3{0.0, -1.0, 0.0});
        li.rotateGlobal(star::Type::Axis::x, -90);
    }

    auto aloevera = LoadObj(ctx, mediaPath / "models" / "aloevera" / "aloevera.obj", "aloevera");

    std::vector<std::shared_ptr<star::StarObject>> objects{std::move(lion), std::move(aloevera)};
    return objects;
}