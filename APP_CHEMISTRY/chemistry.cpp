// Installation procedure:
//
// Download GraphicsEngine via github
// In GraphicsEngine main directory:
//   - mkdir build
//   - cd build
//   - cmake ..
//   - sudo make install
//
// Link GraphicsEngine include files in CMakeLists.txt
// Link against libGraphicsEngine.so
// In the .cpp, include "cgl.h".
// Everything should be set up and ready to go!
//

#include <cgl.h>

using namespace std;

void functionA(pCglScene sc){
    cout << "Fonction A utilisee" << endl;
}
void functionB(pCglScene sc){
    cout << "Fonction B utilisee" << endl;
}
void selectAll(pCglScene sc){
    for(int i = 0 ; i < sc->numObjects() ; i++){
        sc->getObject(i)->select();
    }
    sc->unSelect();
}

int main(int argc, char **argv){
  CglCanvas cv(argc, argv);
  string configFile = "@CMAKE_SOURCE_DIR@/APP_chemistry/chemistry.config"; // Remplacer avec "at"CMAKE_SOURCE_DIR"at"
  cv.getProfile()->read_configuration_file(configFile);

  cv.getKeyboard()->addAction('K', functionA);
  cv.getKeyboard()->addAction('L', functionB);
  cv.getKeyboard()->addAction('A', selectAll);

    pCglWindow window2 = new CglWindow(800, 0, 800, 800);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pCglSubWindow SW3 = new CglSubWindow(window2, CGL_FULL);
    pCglScene  scene3  = new CglScene();
    pCglLight fill3    = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side3    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
    pCglLight back3    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);

    scene3->addLight( fill3 );
    scene3->addLight( side3 );
    scene3->addLight( back3 );
    SW3->setScene(   scene3);
    cv.initGLEW();

    //scene3->addObject(new CglCube(  0,0.12,0));
    //scene3->addObject(new CglSphere(0,0,0.12));
    //scene3->addObject(new CglCylinder(  -0.025,-0.025,-0.025,   -0.025,-0.025,0.025));

    /*

    std::vector<pCglSphere> atoms;
    float space = 0.05;
    float begin = -0.075;
    int dim     = 3;

    for(int i = 0 ; i < dim ; i++){
        for(int j = 0 ; j < dim ; j++){
            for(int k = 0 ; k < dim ; k++){
                float r,g,b;
                if ((dim*dim*i + dim*j + k)%2==0){r=1;g=0.3;b=0.3;}
                else{r=0.3;g=0.3;b=1;}
                atoms.push_back(new CglSphere(  begin + space * i,
                                                begin + space * j,
                                                begin + space * k,
                                                r,g,b));
            }
        }
    }



    std::vector<pCglCylinder> links;
    for(int i = 0 ; i < atoms.size() ; i++){
      for(int j = 0 ; j < atoms.size() ; j++){
        if( (i!=j) && ((i+j)%11==0) )
            links.push_back(new CglCylinder(atoms[i], atoms[j], 1., 1., .95));
      }
    }

    */

    std::vector<pCglMesh> atoms;
    for(int i = 0 ; i < 10 ; i++)
      atoms.push_back(new CglMesh(CGL_CUBE));
    std::vector<pCglMesh> links;
    //for(int i = 0 ; i < 1 ; i++)
    //  links.push_back(new CglMesh(CGL_CYLINDER));

    /*
    pCglSuperObject OBJ = new CglSuperObject();
    scene3->addObject(OBJ);
    for(int i = 0 ; i < atoms.size() ; i++)
      OBJ->addPart(atoms[i]);
    for(int i = 0 ; i < links.size() ; i++)
      OBJ->addPart(links[i]);
    */
    // PROBLEME EN DECIDANT D'UTILISER LES MESH ET PAS OBJECT.
    // SEGFAULT AVEC LIBGL.so

    for(int i = 0 ; i < atoms.size() ; i++)
      scene3->addObject(atoms[i]);

    SW3->loadShaders();

  cv.loop();

  return 0;
}

/*
ToogleMolecule(ATOM atom){
    for atom...
        do...
        end;

}*/

//addAction("K", (void*)ToogleMolecule)
//onRightClick( ptrFunc );

/*

enum atomType{Oxygen, Bohr, Carbon, Helium};
class Atom;
typedef Atom* pAtom;


class Molecule: public CglSuperObject{
private:
    std::vector<pAtom> atoms;
public:
    Molecule(std::vector<pAtom> ato)
    void init();
};
Molecule::Molecule(std::vector<pAtom> ato){
    for(int i = 0 ; i < ato.size() ; i++)
        atoms.push_back(ato[i]);
}
Molecule::init(){
    //Ajout des atomes
    for(int i = 0 ; i < atoms.size() ; i++)
        addPart(atoms[i]);
    //Ajout des liens
    for(int i = 0 ; i < atoms.size() ; i++){
        for(int j = 0 ; j < atoms[i]->neighbors.size() ; j++)
            addPart(new Link(atoms[i], atoms[i]->neighbors[j]));
    }
}






//Get neighbors based on position
class Atom : public CglSphere{
public:
    atomType            atom;
    glm::vec3           position;
    std::vector<pAtom>  neighbors;
    glm::vec3           color;
    float               radius;

    Atom(atomType atom_type, glm::vec3 pos);
    void        addNeighbor(Atom atom);
};

Atom::Atom(atomType atom_type, glm::vec3 pos){
    atomType    = atom_type;
    position    = pos;

    switch(atomType){
    case(Oxygen):
        color   = glm::vec3(1,0,0);
        radius  = 1.0;
        break;
    case(Bohr):
        color = glm::vec3(0,0,1);
        radius = 0.5;
        break;
    case(Carbon):
        color = glm::vec3(0.3,0.3,0.3);
        radius = 0.8;
        break;
    case(Helium):
        color = glm::vec3(1,1,0.5);
        radius = 0.25;
        break;
    }
}
glm::vec3 Atom::addNeighbor(pAtom atom){neighbors.push_back(pAtom);}
*/
