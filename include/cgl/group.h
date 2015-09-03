#ifndef CglGROUP_H
#define CglGROUP_H

#include <cgl/object.h>

class CglGroup
{
  public:
    std::vector<pCglObject> listObject;
    glm::vec3 group_color;
    bool selected;

    std::vector< glm::vec3* > pCenters;
    glm::vec3 group_center;
    glm::vec3 bbmin, bbmax;

    CglGroup(std::vector<pCglObject> groupOfObjects);
    ~CglGroup();
    void compute();
    bool isSelected();

  protected:
  private:
};

typedef CglGroup* pCglGroup;

#endif // CglGROUP_H
