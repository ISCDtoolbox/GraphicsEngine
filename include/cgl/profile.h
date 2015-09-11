#ifndef PROFILE_H
#define PROFILE_H

#include "defs.h"

#include "cgl/colorgenerator.h"

class CglProfile
{
  public:
    CglProfile();
    ~CglProfile();
    glm::vec3 color();
    void switch_theme();
    void update_theme();

  public:
    std::string path;
    CglColorGenerator colorGenerator;

    bool  displayShadows;
    bool  displayBottomGrid;
    bool  displayAxes;
    bool  displayAxesLabels;
    bool  displayReflection;
    bool  displayBackgroundGradient;
    bool  smooth;

    bool  perspective;

    bool  flyingMode;
    bool  classicalMode;
    bool  accumulatedMode;

    bool  stereo;

    float bottomDistance;
    float bottomAngle;

    bool  invertVertical;
    bool  independantScale;
    bool  globalScale;
    bool  groupRotation;

    CGL_THEME  theme;
    glm::vec2  reflection_mix;
    glm::vec3  sele_color;
    glm::vec3  back_color, grid_color, idle_color;
    float      shadow_factor;

    float saturation;
    float value;
    float mixWithWhite;

  protected:
  private:
};

#endif // PROFILE_H
