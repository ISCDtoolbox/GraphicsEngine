#include "cgl/colorgenerator.h"
#include <cgl/canvas.h>
extern CglCanvas *pcv;

using namespace std;

CglColorGenerator::CglColorGenerator(){
  srand(time(0));
  start = (rand()/(double)(RAND_MAX + 1)) + 1;
  inc = 0;
}

CglColorGenerator::~CglColorGenerator(){

}

glm::vec3 CglColorGenerator::generateColor(){
  double golden_ratio_conjugate = 0.618033988749895;
  float h = (start + inc * golden_ratio_conjugate);
  h -= int(h);
  rgb rgbColor = hsv2rgb(hsv{double(h)*360, pcv->profile.saturation, pcv->profile.value});
  inc++;
  glm::vec3 col = glm::vec3(rgbColor.r, rgbColor.g, rgbColor.b);
  float fac = pcv->profile.mixFactor;
  return (1-fac) * col + fac * pcv->profile.mixColor;
}

hsv CglColorGenerator::rgb2hsv(rgb in){
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
            // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb CglColorGenerator::hsv2rgb(hsv in){
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}


CglPalette::CglPalette(float m, float M, CGL_PALETTE p){
        mini    = m;
        maxi    = M;
        palette = p;

        switch(palette){
        case(CGL_PALETTE_JET):
            colors.push_back(glm::vec4(0,   1,   0,   0));
            colors.push_back(glm::vec4(0.35, 0.75,   0.75, 0.));
            colors.push_back(glm::vec4(0.5, 0, 1, 0));
            colors.push_back(glm::vec4(0.65, 0., 0.75, 0.75));
            colors.push_back(glm::vec4(1,   0., 0.0, 1));
            break;
        case(CGL_PALETTE_GRAY):
            colors.push_back(glm::vec4(0,0,0,0));
            colors.push_back(glm::vec4(1,1,1,1));
            break;
        case(CGL_PALETTE_BR):
            colors.push_back(glm::vec4(0,0,0,1));
            colors.push_back(glm::vec4(0.5,1,1,1));
            colors.push_back(glm::vec4(1,1,0,0));
            break;
        }
    }
void CglPalette::setBoundaries(float m, float M){
        mini = m;
        maxi = M;
    }
glm::vec3 CglPalette::getColor(float val){
        val = (val-mini)/(maxi-mini);
        for(int i = 0 ; i < colors.size() ; i++){
            if(i==colors.size()-1){
                float fac       = (val - colors[i-1][0])/(colors[i][0] - colors[i-1][0]);
                glm::vec4 col   = (1-fac)*colors[i-1] + fac*colors[i];
                return glm::vec3(col[1], col[2], col[3]);
            }
            else{
                if( (val>colors[i][0]) && (val<colors[i+1][0]) ){
                    float fac       = (val - colors[i][0])/(colors[i+1][0] - colors[i][0]);
                    glm::vec4 col   = (1-fac)*colors[i] + fac*colors[i+1];
                    return glm::vec3(col[1], col[2], col[3]);
                }
            }
        }
        //glm::vec3 col0 = glm::vec3(colors[0][1], colors[0][1], colors[0][2]);
        //glm::vec3 col1 = glm::vec3(colors[1][1], colors[1][1], colors[1][2]);
        //return glm::vec3(val);
        //return val * col0 + (1-val)*col1;
    }











