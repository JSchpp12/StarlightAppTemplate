#pragma once

#ifdef STAR_ENABLE_PRESENTATION

#include "Application.hpp"

#include <star_windowing/listener/ListenForKeyRelease.hpp>

class InteractiveApplication : public Application
{
  public:
    InteractiveApplication(ApplicationConfig config) : Application(std::move(config)), m_listenKeyRelease(*this)
    {
    }

    virtual std::shared_ptr<star::StarScene> loadScene(star::core::device::DeviceContext &context) override;
    virtual void shutdown(star::core::device::DeviceContext &context) override;
    void onKeyRelease(const star::windowing::event::KeyRelease &event, bool &keepAlive);
    virtual void frameUpdate(star::core::SystemContext &ctx) override;

  private:
    star::windowing::ListenForKeyRelease<InteractiveApplication> m_listenKeyRelease;
};

#endif
