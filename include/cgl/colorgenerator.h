#ifndef CglCOLORGENERATOR_H
#define CglCOLORGENERATOR_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <enums.h>

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

//Palette linéaire
class CglPalette
{
    float mini;
    float maxi;
    CGL_PALETTE palette;

    //X = entre 0 et 1
    //Y,Z,W entre 0 et 1, correspondent à RGB
    std::vector<glm::vec4> colors;

public:
    CglPalette(float m = 0.0f, float M=1.0f, CGL_PALETTE p=CGL_PALETTE_JET);
    void setBoundaries(float m, float M);
    glm::vec3 getColor(float val);
};

#endif // CglCOLORGENERATOR_H
