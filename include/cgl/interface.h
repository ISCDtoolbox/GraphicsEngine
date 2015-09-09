#ifndef CGLINTERFACE_H
#define CGLINTERFACE_H

#include "cgl/defs.h"
#include "cgl/texture.h"

////////////////////////////////////////////////////////////////
//Button class

class CglButton{
  private:
    CglTexture texture;
    float      size;
    glm::vec2  center;
    glm::vec3  color;
    glm::vec2  mins, maxs;

  public:
    CglButton(float s, glm::vec2 c, glm::vec3 col, string texturePath);
    ~CglButton(){};
    void display();
    glm::vec2 getMins(){return mins;}
    glm::vec2 getMaxs(){return maxs;}
};
typedef CglButton* pCglButton;



////////////////////////////////////////////////////////////////
//Interface class

class CglInterface{
  private:
    std::vector<pCglButton> buttons;
    bool                    isMouseOnPanel;
  public:
    CglInterface(){};
    ~CglInterface(){};
    void init(int nb);
    void display();
    std::vector<pCglButton>* getButtonList(){return &buttons;}
};
typedef CglInterface* pCglInterface;


#endif // CGLINTERFACE_H
