#ifndef PROFILE_H
#define PROFILE_H

#include "defs.h"

#include "cgl/colorgenerator.h"

class CglProfile
{
  public:
    CglProfile();
    ~CglProfile(){};
    glm::vec3 color();

    void switch_theme();
    void update_theme();
    void switch_colors();
    void update_colors();
    void update_objects_colors();
    void switch_interface();

  public:
    std::string path;
    CglColorGenerator colorGenerator;

    bool  displayShadows;
    bool  displayBottomGrid;
    bool  displayAxes;
    bool  displayAxesLabels;
    bool  displayReflection;
    bool  displayBackgroundGradient;

    bool  perspective;

    bool  flyingMode;
    bool  classicalMode;
    bool  accumulatedMode;

    bool smooth;

    bool  stereo;

    float bottomDistance;
    float bottomAngle;

    bool  invertVertical;
    bool  independantScale;
    bool  globalScale;
    bool  groupRotation;

    //GUI settings
    CGL_INTERFACE interface = CGL_INTERFACE_RADIAL;
    CGL_BUTTONS   interface_type = CGL_BUTTONS_BASIC;

    //Theme settings
    CGL_THEME  theme;
    glm::vec2  reflection_mix;
    glm::vec3  sele_color;
    glm::vec3  back_color, grid_color, idle_color;
    float      shadow_factor;

    //Mesh colors
    CGL_COLORS colors;
    float      saturation;
    float      value;
    float      mixFactor;
    glm::vec3  mixColor;

  protected:
  private:
};

#endif // PROFILE_H
