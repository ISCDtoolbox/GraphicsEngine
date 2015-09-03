
/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_LIGHT_H_
#define __GLIC_LIGHT_H_

#include <glic/material.h>
#include <glic/object.h>


class GLIC_API CglicLight : public CglicObject
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
  CglicLight(const int& i);
  virtual ~CglicLight();

  //void setCol(CglicMaterial::TcolType typ, const float r, const float g,const float b, const float a);
  void setPos(const float x, const float y, const float z, const float w);
  void setAtt(const int c, const int l, const int q);
  void setType(TlitType typ);
  void setSpot(float cutoff, float sx, float sy, float sz, float expo);

  //void getCol(CglicMaterial::TcolType typ, float &r, float &g, float &b, float &a) const;
  void getPos(float &x, float &y, float &z, float &w) const;
public:
  virtual void glicInit();
};

typedef CglicLight * pCglicLight;


#endif
