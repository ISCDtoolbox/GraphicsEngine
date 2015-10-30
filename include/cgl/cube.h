/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_CUBE_H_
#define __CGL_CUBE_H_

#include <cgl/object.h>

class CglPrimitive: public CglObject{
protected:
    GLuint mBuffer;
    GLuint iBuffer;
    glm::vec3 color;
    glm::vec3 pos;
    float size;

public:
    glm::vec3 getPos(){return pos;}
    void display();
};
typedef CglPrimitive*   pCglPrimitive;




class CGL_API CglCube : public CglPrimitive{
public:
    CglCube(float x, float y, float z);
    ~CglCube();
};
typedef CglCube*        pCglCube;




class CGL_API CglSphere : public CglPrimitive{
public:
    CglSphere(float x, float y, float z);
    ~CglSphere();
};
typedef CglSphere*      pCglSphere;




class CGL_API CglCylinder : public CglPrimitive{
private:
    //glm::vec3 pt1, pt2;
public:
    CglCylinder(float x, float y, float z, float x2, float y2, float z2);
    CglCylinder(pCglPrimitive obj1, pCglPrimitive obj2);
    void init(float x,  float y,  float z, float x2, float y2, float z2);
    ~CglCylinder();
};
typedef CglCylinder*    pCglCylinder;



#endif
