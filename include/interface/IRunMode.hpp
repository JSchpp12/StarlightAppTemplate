#pragma once

class IRunMode
{
  public:
    virtual ~IRunMode() = default; 
    virtual void run() = 0;
};