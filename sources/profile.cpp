#include "glic/profile.h"

CglicProfile::CglicProfile()
{
  displayShadows    = true;
  displayBottomGrid = true;
  displayAxes       = true;
  displayAxesLabels = false;

  stereo            = false;

  perspective       = true;

  classicalMode     = true;
  accumulatedMode   = false;
  flyingMode        = false;

  invertVertical    = true;

  independantScale  = false;
  globalScale       = true;

  groupRotation     = true;


  bottomDistance    = 0.1;
  bottomAngle       = 0;//(float)3.14159/4;

  //Colors
  back_color    = glm::vec3(1,   1,   0.97);        //Background
  grid_color    = glm::vec3(0.5, 0.5, 0.5);         //Grid
  sele_color    = glm::vec3(1,   0.6, 0);           //Object selection
  idle_color    = glm::vec3(0.3, 0.3, 0.3);         //Unselected boxes
  //group_color   = glm::vec3(0,   0.8, 0);         //Group selection
}

CglicProfile::~CglicProfile(){}

glm::vec3 CglicProfile::color(){
  return colorGenerator.generateColor();
}
