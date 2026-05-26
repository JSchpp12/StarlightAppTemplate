#pragma once

#include "struct/ApplicationConfig.hpp"

#include <starlight/core/renderer/RendererBase.hpp>
#include <starlight/templates/StarApplication.hpp>

#include <optional>

class Application : public star::StarApplication
{
  public:
    Application(ApplicationConfig config) : m_config(std::move(config))
    {
    }
    virtual ~Application() = default;
    virtual std::shared_ptr<star::StarScene> loadScene(star::core::device::DeviceContext &ctx) override;
    virtual void shutdown(star::core::device::DeviceContext &context) override;
    virtual void init() override;
    virtual void frameUpdate(star::core::SystemContext &context) override;

  protected:
    ApplicationConfig m_config;
    star::core::renderer::TimelineSynchronizer m_mainRendererSync;
    std::vector<star::Handle> m_screenshotRegistrations;
    star::core::renderer::RendererBase *m_mainRenderer{nullptr};

    void triggerScreenshot(star::core::device::DeviceContext &context);

    static void TriggerSubmissionOfMainDraw(const star::core::device::DeviceContext &ctx,
                                            const star::Handle &registration,
                                            const star::Handle &timelineSemaphoreToUse);
};
