#pragma once

#ifdef STAR_ENABLE_PRESENTATION
#include "interface/IRunMode.hpp"

class WindowedMode : public IRunMode
{
  public:
    virtual void run() override;
};

#endif