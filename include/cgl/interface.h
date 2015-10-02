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
    glm::vec3  color;
    glm::vec2  mins, maxs;
    int        rank;

  public:
    glm::vec2  center;

  public:
    CglButton(float s, glm::vec2 c, glm::vec3 col, string texturePath);
    ~CglButton(){};
    void updateTexture(string texturePath);
    void display();
    void activate();
    void setRank(int i ){rank = i;}
    glm::vec2 getMins(){return mins;}
    glm::vec2 getMaxs(){return maxs;}
};
typedef CglButton* pCglButton;



////////////////////////////////////////////////////////////////
//Interface class

class CglInterface{
  protected:
    int                     nbButtons;
    std::vector<pCglButton> buttons;
    std::vector<string>     icons;
    std::vector<bool>       hovered;
    std::string             folder;
    std::string             type;
    bool                    active;
    bool                    isMouseOnPanel;

  public:
    CglInterface(){active=false;};
    ~CglInterface(){};
    void unactive(){active = false;}
    bool isActive(){return active;}
    bool isMouseOnZone(){return isMouseOnPanel;}
    void updateTextures();
    std::string getType(){return type;}
    std::string getIconsFolder();
    pCglButton  getButton(int i){return buttons[i];};
    void        init_buttons();
    virtual void init(int nb, float off){};
    virtual void init(glm::vec2 cen, float rad){};
    virtual void display(){};
    std::vector<pCglButton>* getButtonList(){return &buttons;}

    void hover(int indButton);
    void checkHoveredButtons(int x, int y);
    void checkClickedButtons(int b, int s, int x, int y);
};
typedef CglInterface* pCglInterface;


class CglLinearInterface: public CglInterface{
  protected:
    float offset;
  public:
    void init(float off);
    void display();
};
typedef CglLinearInterface* pCglLinearInterface;


class CglRadialInterface: public CglInterface{
  protected:
    float     radius;
    glm::vec2 center;
  public:
    void init(glm::vec2 cen, float rad);
    void display();
    void displayWheel(int step);
};
typedef CglRadialInterface* pCglRadialInterface;


#endif // CGLINTERFACE_H
