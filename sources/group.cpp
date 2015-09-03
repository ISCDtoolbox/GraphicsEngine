#include "glic/defs.h"

#include "glic/group.h"
#include "glic/canvas.h"

extern CglicCanvas *pcv;

CglicGroup::CglicGroup(std::vector<pCglicObject> groupOfObjects)
{
  listObject = groupOfObjects;
  group_color = glm::vec3(0,0.7,0);
  bbmin = glm::vec3(100000);
  bbmax = glm::vec3(-100000);

  //Centres du groupe
  for(int i = 0 ; i < listObject.size() ; i++){
    pCenters.push_back(listObject[i]->getCenterPtr());
  }

  //BBOX
  for(int i = 0 ; i < listObject.size() ; i++){
    glm::vec3 mini = listObject[i]->getBBMIN();
    glm::vec3 maxi = listObject[i]->getBBMAX();
    bbmin.x = min(bbmin.x, mini.x);
    bbmin.y = min(bbmin.y, mini.y);
    bbmin.z = min(bbmin.z, mini.z);
    bbmax.x = max(bbmax.x, maxi.x);
    bbmax.y = max(bbmax.y, maxi.y);
    bbmax.z = max(bbmax.z, maxi.z);
  }
}

CglicGroup::~CglicGroup()
{
  //dtor
}

void CglicGroup::compute(){

  //On vérifie si un membre du groupe est sélectionné
  bool match = false;
  for(int i = 0 ; i < listObject.size() ; i++){
    if(listObject[i]->isSelected()){
      match = true;
      selected = true;
    }
  }
  if(!match)
    selected = false;

  //On sélectionne les membres du groupe
  if(selected)
    for(int i = 0 ; i < listObject.size() ; i++)
      listObject[i]->select();

  //On update éventuellement le centre de rotation des objets
  if(pcv->profile.groupRotation){

    //On calcule le centre du groupe
    glm::vec3 tmpCenter;
    for(int i = 0 ; i < pCenters.size() ; i++){
      tmpCenter += *pCenters[i];
    }
    group_center = float( 1.0f/pCenters.size() ) * tmpCenter;

    //On envoie le centre aux objets
    for(int i = 0 ; i < listObject.size() ; i++){
      listObject[i]->setRotationCenter(group_center);
    }
  }
}

bool CglicGroup::isSelected(){return selected;}
