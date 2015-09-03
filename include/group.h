#ifndef CGLICGROUP_H
#define CGLICGROUP_H

#include <glic/object.h>

class CglicGroup
{
  public:
    std::vector<pCglicObject> listObject;
    glm::vec3 group_color;
    bool selected;

    std::vector< glm::vec3* > pCenters;
    glm::vec3 group_center;
    glm::vec3 bbmin, bbmax;

    CglicGroup(std::vector<pCglicObject> groupOfObjects);
    ~CglicGroup();
    void compute();
    bool isSelected();

  protected:
  private:
};

typedef CglicGroup* pCglicGroup;

#endif // CGLICGROUP_H
