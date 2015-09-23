#ifndef PROFILE_H
#define PROFILE_H

#include "defs.h"

#include "cgl/colorgenerator.h"

class CglProfile
{
  public:
    CglProfile();
    void read_configuration_file(std::string fileName);
    ~CglProfile(){};
    glm::vec3 color();

    void switch_theme();
    void update_theme();
    void switch_colors();
    void update_colors();
    void update_ground();
    void update_objects_colors();
    void switch_interface();


  public:
    std::string path;
    CglColorGenerator colorGenerator;

    bool            displayAxes;
    bool            smooth;

    bool            perspective;

    bool            stereo;
    bool            invertVertical;
    bool            fullscreen;


    CGL_INTERFACE   interface;
    CGL_BUTTONS     buttons;

    //Ground settings
    CGL_GROUND      ground;
    bool            displayBottomGrid;
    bool            displayReflection;
    bool            displayShadows;
    float           bottomDistance;
    float           bottomAngle;

    //Initial arrangement
    CGL_ARRANGEMENT initial_arrangement;
    bool            independantScale;
    bool            globalScale;

    //Camera settings
    CGL_CAM         camera;
    bool            flyingMode;

    //Theme settings
    CGL_THEME       theme;
    glm::vec2       reflection_mix;
    glm::vec3       sele_color;
    glm::vec3       back_color, grid_color, idle_color;
    float           shadow_factor;

    //Mesh colors
    CGL_COLORS      colors;
    float           saturation;
    float           value;
    float           mixFactor;
    glm::vec3       mixColor;
};

#endif // PROFILE_H
