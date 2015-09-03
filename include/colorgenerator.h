#ifndef CGLICCOLORGENERATOR_H
#define CGLICCOLORGENERATOR_H

#include <glm/glm.hpp>
#include <iostream>

typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

class CglicColorGenerator
{
  public:
    int inc;
    float start;

    CglicColorGenerator();
    ~CglicColorGenerator();
    glm::vec3 generateColor();

    hsv   rgb2hsv(rgb in);
    rgb   hsv2rgb(hsv in);

  protected:
  private:
};

#endif // CGLICCOLORGENERATOR_H
