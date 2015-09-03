#ifndef CglCOLORGENERATOR_H
#define CglCOLORGENERATOR_H

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

class CglColorGenerator
{
  public:
    int inc;
    float start;

    CglColorGenerator();
    ~CglColorGenerator();
    glm::vec3 generateColor();

    hsv   rgb2hsv(rgb in);
    rgb   hsv2rgb(hsv in);

  protected:
  private:
};

#endif // CglCOLORGENERATOR_H
