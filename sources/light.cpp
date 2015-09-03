#include <glic/light.h>
#include <glic/material.h>
#include <glm/ext.hpp>


// light constructor
CglicLight::CglicLight(const int& i)
{
  //cout << "  --- - [create CglicLight]" << endl;
  ltyp = TL_DIRECTION;
  lid = i;

  //setCol(CglicMaterial::TC_AMB, 0.0, 0.0, 0.0, 1.0);
  //setCol(CglicMaterial::TC_DIF, 1.0, 1.0, 1.0, 1.0);
  //setCol(CglicMaterial::TC_SPE, 1.0, 1.0, 1.0, 1.0);

  setPos(0, 0, 0, 1);
  setAtt(1, 0, 0);

  spotCutoff   = 45.0;
  spotDirect[0] =  0.0;
  spotDirect[1] =  0.0;
  spotDirect[2] = -1.0;
  spotExp      =  0.0;

  //cout << " -- Number of Light: " << lid << endl;

}


CglicLight::~CglicLight()
{
  //cout << "  --- - [destroy CglicLight]" << endl;
}
/*
void CglicLight::setCol(CglicMaterial::TcolType typ, const float r, const float g,const float b, const float a)
{
  switch (typ) {
    case CglicMaterial::TC_AMB:
      amb[0] = r;
      amb[1] = g;
      amb[2] = b;
      amb[3] = a;
      break;
    case CglicMaterial::TC_DIF:
      dif[0] = r;
      dif[1] = g;
      dif[2] = b;
      dif[3] = a;
      break;
    case CglicMaterial::TC_SPE:
      spe[0] = r;
      spe[1] = g;
      spe[2] = b;
      spe[3] = a;
      break;
    default:

      break;
  }
}
*/

void CglicLight::setPos(const float x, const float y, const float z, const float w)
{
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
  pos[3] = w;
  //cout << " ----- setPos w: " << w << "; ltyp: " << ltyp << endl;
  ltyp = ( w >=0.9 ) ? TL_POINT : TL_DIRECTION;
  //cout << " -----setPos ltyp: " << ltyp << endl;
}


void CglicLight::setAtt(const int c, const int l, const int q)
{
  cstatt  = c;
  linatt  = l;
  quadatt = q;
}


void CglicLight::setType(TlitType typ)
{
  ltyp = typ;
}


void CglicLight::setSpot(float cutoff, float sx, float sy, float sz, float expo)
{
  spotCutoff    = cutoff;
  spotDirect[0] = sx;
  spotDirect[1] = sy;
  spotDirect[2] = sz;
  spotExp       = expo;
}

/*
void CglicLight::getCol(CglicMaterial::TcolType typ, float &r, float &g, float &b, float &a) const
{
  switch (typ) {
    case CglicMaterial::TC_AMB:
      r = amb[0];
      g = amb[1];
      b = amb[2];
      a = amb[3];
      break;
    case CglicMaterial::TC_DIF:
      r = dif[0];
      g = dif[1];
      b = dif[2];
      a = dif[3];
      break;
    case CglicMaterial::TC_SPE:
      r = spe[0];
      g = spe[1];
      b = spe[2];
      a = spe[3];
      break;
    default:

      break;
  }
}
*/

void CglicLight::getPos(float &x, float &y, float &z, float &w ) const
{
  x = pos[0];
  y = pos[1];
  z = pos[2];
  w = pos[3];
}


void CglicLight::glicInit()
{
  // check if enabled ...
  GLenum   light = GL_LIGHT0 + lid;

  glLightfv(light, GL_AMBIENT, glm::value_ptr(amb));
  glLightfv(light, GL_DIFFUSE, glm::value_ptr(dif));
  glLightfv(light, GL_SPECULAR, glm::value_ptr(spe));

  switch (ltyp) {
    case TL_POINT:
      pos[3] = 1.0;
      glLightfv(light, GL_POSITION, glm::value_ptr(pos));
      glLightf(light, GL_SPOT_CUTOFF, 180.0);
      break;
    case TL_DIRECTION:
      pos[3] = 0.0;
      glLightfv(light, GL_POSITION, glm::value_ptr(pos));
      glLightf(light, GL_SPOT_CUTOFF, 180.0);
      break;
    case TL_SPOT:
      pos[3] = 1.0;
      glLightfv(light, GL_POSITION, glm::value_ptr(pos));
      glLightf(light, GL_SPOT_CUTOFF, spotCutoff);
      glLightfv(light, GL_SPOT_DIRECTION, spotDirect);
      glLightf(light, GL_SPOT_EXPONENT, spotExp);
      break;
    default:

      break;
  }

  glLighti(light, GL_CONSTANT_ATTENUATION, cstatt);
  glLighti(light, GL_LINEAR_ATTENUATION, linatt);
  glLighti(light, GL_QUADRATIC_ATTENUATION, quadatt);
}

