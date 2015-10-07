#include "cgl/group.h"
#include "cgl/canvas.h"

extern CglCanvas *pcv;

CglGroup::CglGroup(std::vector<pCglObject> groupOfObjects)
{
  listObject = groupOfObjects;
  group_color = glm::vec3(0,0.7,0);
  bbmin = glm::vec3(100000);
  bbmax = glm::vec3(-100000);

  //Centres du groupe
  for(int i = 0 ; i < numObjects() ; i++){
    pCenters.push_back(listObject[i]->getCenterPtr());
  }

  //BBOX
  for(int i = 0 ; i < numObjects() ; i++){
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

CglGroup::~CglGroup(){
  //On envoie le centre aux objets
  cout << "toto!!" << endl;
  for(int i = 0 ; i < numObjects() ; i++){
    glm::vec3 new_center = glm::vec3(listObject[i]->getMODEL()[3]);
    listObject[i]->setCenter(new_center);
  }
}

void CglGroup::unGroup(){
  //On envoie le centre aux objets
  cout << "toto!!" << endl;
  for(int i = 0 ; i < numObjects() ; i++){
    glm::vec3 new_center = glm::vec3(listObject[i]->getMODEL()[3]);
    listObject[i]->setCenter(new_center);
  }
}

void CglGroup::compute(){

  //On vérifie si un membre du groupe est sélectionné
  bool match = false;
  for(int i = 0 ; i < numObjects() ; i++){
    if(listObject[i]->isSelected()){
      match = true;
      selected = true;
    }
  }
  if(!match)
    selected = false;

  //On sélectionne les membres du groupe
  if(selected)
    for(int i = 0 ; i < numObjects() ; i++)
      listObject[i]->select();

  //On calcule le centre du groupe
  glm::vec3 tmpCenter;
  for(int i = 0 ; i < pCenters.size() ; i++){
    tmpCenter += *pCenters[i];
  }
  group_center = float( 1.0f/pCenters.size() ) * tmpCenter;
  //On envoie le centre aux objets
  for(int i = 0 ; i < numObjects() ; i++){
    listObject[i]->setRotationCenter(group_center);
  }

}


