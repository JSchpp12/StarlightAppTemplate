#pragma once

#ifndef STAR_ENABLE_PRESENTATION

#include "interface/IRunMode.hpp"

class Headless : public IRunMode
{
  public:
    virtual void run() override;
};

#endif // !STAR_ENABLE_PRESENTATION