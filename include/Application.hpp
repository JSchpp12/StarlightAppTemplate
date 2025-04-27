#pragma once 

#include "StarApplication.hpp"

#include <GLFW/glfw3.h>

#include <string> 

class Application :
    public star::StarApplication
{
public:
    Application(star::StarScene& scene);

    void Load();

    virtual std::string getApplicationName() { return "Starlight Application"; }

    void onKeyPress(int key, int scancode, int mods) override;

protected:

private:
    void onKeyRelease(int key, int scancode, int mods) override;
    void onMouseMovement(double xpos, double ypos) override;
    void onMouseButtonAction(int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onWorldUpdate(const uint32_t& frameInFlightIndex) override;
};
