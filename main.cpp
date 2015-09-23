#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cgl.h>

using namespace std;


int main(int argc, char **argv){

  int idw, ids, ido;
  CglCanvas cv(argc,argv);

  if((argc==2) && (string(argv[1])=="cgl.save")){
    cv.initWindow(idw, ids, 800, 800);
    cv.load_meshes_from_file("cgl.save", ids);
  }

  else if (argc > 1)
  {
    cv.initWindow(idw, ids, 800, 800);
    vector<CglMesh*> mesh;
    for (int i=0; i < argc - 1; i++){
      cout << "i: " << i << endl;
      mesh.push_back(new CglMesh(argv[i+1]));
      mesh[i]->meshInfo(0);
      ido = cv.cglObject(mesh[i]);
      cv.cglSetObject(ido, ids);
    }
    cv.getScene(ids)->place_objects_on_grid();
  }

  else{ cout << "Invalid arguments" << endl; exit(1); }

  cv.cglMainLoop();

  return(EXIT_SUCCESS);
}
