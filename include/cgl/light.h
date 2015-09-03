
/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_LIGHT_H_
#define __CGL_LIGHT_H_

#include <cgl/material.h>
#include <cgl/object.h>


class CGL_API CglLight : public CglObject
{
public:
  enum TlitType {TL_POINT, TL_DIRECTION, TL_SPOT};
  int      lid;
private:
  glm::vec4 amb, dif, spe, pos;
  int       cstatt,linatt,quadatt;
  float     spotCutoff,spotDirect[3],spotExp;
  TlitType  ltyp;

public:
  // constructor + destructor
  CglLight(const int& i);
  virtual ~CglLight();

  //void setCol(CglMaterial::TcolType typ, const float r, const float g,const float b, const float a);
  void setPos(const float x, const float y, const float z, const float w);
  void setAtt(const int c, const int l, const int q);
  void setType(TlitType typ);
  void setSpot(float cutoff, float sx, float sy, float sz, float expo);

  //void getCol(CglMaterial::TcolType typ, float &r, float &g, float &b, float &a) const;
  void getPos(float &x, float &y, float &z, float &w) const;
public:
  virtual void cglInit();
};

typedef CglLight * pCglLight;


#endif
