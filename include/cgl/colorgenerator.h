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
    CglPalette(float m, float M, CGL_PALETTE p){
        mini    = m;
        maxi    = M;
        palette = p;

        switch(palette){
        case(CGL_PALETTE_JET):
            colors.push_back(glm::vec4(0,   0,   0,   0.2));
            colors.push_back(glm::vec4(0.3, 0,   0.2, 0.8));
            colors.push_back(glm::vec4(0.5, 0.2, 0.8, 0.2));
            colors.push_back(glm::vec4(0.7, 0.8, 0.2, 0.0));
            colors.push_back(glm::vec4(1,   0.2, 0.0, 0.0));
            break;
        case(CGL_PALETTE_GRAY):
            colors.push_back(glm::vec4(0,0,0,0));
            colors.push_back(glm::vec4(1,1,1,1));
            break;
        case(CGL_PALETTE_BR):
            colors.push_back(glm::vec4(0,0,0,0.5));
            colors.push_back(glm::vec4(0.5,1,1,1));
            colors.push_back(glm::vec4(1,0.5,0,0));
            break;
        }
    }
    CglPalette(){CglPalette(0.0, 1.0, CGL_PALETTE_JET);}

    void setBoundaries(float m, float M){
        mini = m;
        maxi = M;
    }

    glm::vec3 getColor(float val){
        val = (val-mini)/(maxi-mini);
        for(int i = 0 ; i < colors.size() ; i++){
            if(i==colors.size()-1){
                float fac       = (val - colors[i-1][0])/(colors[i][0] - colors[i-1][0]);
                glm::vec4 col   = (1-fac)*colors[i-1] + fac*colors[i];
                return glm::vec3(col[1], col[2], col[3]);
            }
            else if(val>colors[i][0]){
                float fac       = (val - colors[i][0])/(colors[i+1][0] - colors[i][0]);
                glm::vec4 col   = (1-fac)*colors[i] + fac*colors[i+1];
                return glm::vec3(col[1], col[2], col[3]);
            }
        }
    }
};

#endif // CglCOLORGENERATOR_H
