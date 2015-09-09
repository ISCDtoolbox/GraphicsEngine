#ifndef CGLTEXTURE_H
#define CGLTEXTURE_H

#include "defs.h"

class CglTexture
{
  private:
    unsigned width, height;
    GLuint   ID;
  public:
    CglTexture(){};
    ~CglTexture(){};
    void    loadBMP(string filename);
    void    loadPNG(string filename);
    GLuint  getID(){return ID;}
};

#endif // CGLTEXTURE_H
