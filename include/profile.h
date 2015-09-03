#ifndef PROFILE_H
#define PROFILE_H

#include "defs.h"
#include "glic/colorgenerator.h"

class CglicProfile
{
  public:
    //Constructor from config file
    CglicProfile();
    ~CglicProfile();
    glm::vec3 color();

  public:
    CglicColorGenerator colorGenerator;

    bool  displayShadows;
    bool  displayBottomGrid;
    bool  displayAxes;
    bool  displayAxesLabels;

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

    glm::vec3 back_color, grid_color, idle_color;
    glm::vec3 sele_color; //group_color;

  protected:
  private:
};

#endif // PROFILE_H
