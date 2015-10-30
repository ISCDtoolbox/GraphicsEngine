#ifndef CglGROUP_H
#define CglGROUP_H
#include "cgl/defs.h"

#include <cgl/object.h>

class CglGroup
{
private:
    std::vector<pCglObject>   listObject;
    glm::vec3                 group_color;
    bool                      selected;
    std::vector< glm::vec3* > pCenters;
    glm::vec3                 group_center;
    glm::vec3                 bbmin, bbmax;

public:
    CglGroup(std::vector<pCglObject> groupOfObjects);
    ~CglGroup();
    void       unGroup();
    void       compute();
    int        numObjects(){      return listObject.size();}
    bool       isSelected(){      return selected;}
    glm::vec3  getColor(){        return group_color;}
    pCglObject getObject(int ind){return listObject[ind];}
};

typedef CglGroup* pCglGroup;



class CglSuperObject:public CglObject{
public:
    //std::vector<pCglObject>   listPart;
    std::vector< glm::vec3* > pCenters;//TOTRASH
public:
    CglSuperObject(){};
    void addPart(pCglObject obj);
    void display();
    void pickingDisplay();
    void applyTransformation();
    void compute();
    bool isSuper(){return true;}
    //~CglSuperObject();
    void  toogleSelected();
    void  select();
    void  unSelect();
};

typedef CglSuperObject* pCglSuperObject;



#endif // CglGROUP_H
