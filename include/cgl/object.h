/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_OBJECT_H_
#define __CGL_OBJECT_H_

#include "defs.h"
#include <cgl/transform.h>
#include <cgl/material.h>

class               CglScene;
class               CglObject;
class               CglGroup;
typedef CglScene*   pCglScene;
typedef CglObject*  pCglObject;
typedef CglGroup*   pCglGroup;


class CGL_API CglObject
{
  public:
    CglTransform            transform;
    string                  meshFile;

    //Links with other objects
    std::vector<pCglObject> listPart;
    pCglObject              parent;

  protected:
    pCglScene       pScene;
    pCglGroup       pGroup;
    pCglMaterial    pMaterial;

    //GEOMETRY
    bool        isMesh;
    float       localScale;
    float       scaleFactor;

    GLuint      meshBuffer;
    GLuint      normalBuffer;
    GLuint      indicesBuffer;

    int         objectID;
    glm::vec3   pickingColor;
    int         nTriangles;

    //POSTIONS, TRANSFORMATIONS
    glm::mat4   MODEL;
    glm::vec3   center;
    glm::vec3   *rotationCenter;

    bool        isRotationConstrained;
    bool        isTranslationConstrained;
    glm::vec3   constrainedRotationAxis;
    glm::vec3   constrainedTranslationAxis;

    //RENDER PARAMETERS
    bool        selected;
    bool        box;
    bool        line;
    bool        hidden;



  //Public methods
  public:
    CglObject();
    virtual ~CglObject();

    void setScene(pCglScene pSc, int ID){
        pScene          = pSc;
        objectID        = ID;
        pickingColor    = glm::vec3(float(ID%255)/255.0f, 1, 1);
    }

    void setRotationCenter(glm::vec3 &center);
    void setScaleFactor(float sf);

    void setColor(glm::vec3 col);

    void setMODEL(glm::mat4 M);
    void setCenter(glm::vec3 C);
    void setGroup(pCglGroup G);
    void setFileName(std::string n);

    virtual void display(){};
    virtual void artifactsDisplay(){};
    virtual void shadowsDisplay(){};
    virtual bool isSuper(){return false;}
    virtual void pickingDisplay();

    virtual void  toogleSelected();
    virtual void  select();
    virtual void  unSelect();

    virtual void  applyTransformation();
    void saveTransformations();
    void undoLast();
    void resetAll();


  //Public Accessors
  public:
    virtual glm::vec3 getBBMIN(){};
    virtual glm::vec3 getBBMAX(){};
    bool  isMeshObject(){return isMesh;}
    glm::mat4 getMODEL(){return MODEL;}
    glm::vec3 getRotationCenter(){return *rotationCenter;}
    glm::vec3 getCenter(){return center;}

    void        computeGroup();
    pCglGroup   getGroup();
    int         getID();
    float       getLocalScale();
    void        resetGroup();
    glm::vec3*  getCenterPtr();
    //Selection & picking
    virtual bool  isPicked(int ID);
    bool  isHidden();
    void  hide();
    void  unHide();
    bool  isSelected();

    //render modes
    void  toogleBBox();
    void  toogleMesh();

    //Constrained movements
    bool  isConstrainedInRotation();
    bool  isConstrainedInTranslation();
    void  constrainRotation(glm::vec3 axis);
    void  constrainTranslation(glm::vec3 axis);
    void  setConstrainedRotation(float angle);
    void  setConstrainedTranslation(float tr);
    void  unConstrain();

    //Accessors for scene parameters
    glm::vec3 sCENTER();
    glm::vec3 sUP();
    glm::vec3 sCAM();
    glm::mat4 sMODEL();
    glm::mat4 sVIEW();
    glm::mat4 sPROJ();


    //OPNEGL "Facilitators"
    void enableFog(int ID);
    void disableFog(int ID);
    int  initProgram(int ID);
    void createBuffer(GLuint *pBuffer, std::vector<float> *data);
    void createBuffer(GLuint *pBuffer, std::vector<short> *data);
    void createBuffer(GLuint *pBuffer, std::vector<int>   *data);
    void bindBuffer(int attrib, int bufferType, GLuint buffer);
    void freeBuffer();
    void draw(int ID,int s, int mBuffer, int nBuffer, int iBuffer);
    void uniform(int ID, float     f);
    void uniform(int ID, glm::vec3 v);
    void uniform(int ID, glm::mat4 &m);

protected:
  virtual void cglInit();

};

typedef CglObject* pCglObject;


#endif
