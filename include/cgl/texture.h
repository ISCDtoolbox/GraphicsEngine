#ifndef CGLTEXTURE_H
#define CGLTEXTURE_H

#include "defs.h"

class CglTexture
{
  private:
    int     width, height;
    GLuint  ID;
  public:
    CglTexture(){};
    ~CglTexture(){};
    void    load(string filename);
    GLuint  getID(){return ID;}
  protected:
  private:
};

#endif // CGLTEXTURE_H
