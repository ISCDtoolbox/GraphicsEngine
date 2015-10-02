// Installation procedure:
//
// Download SUscCGL via github
// In SUscCGL main directory:
//   - mkdir build
//   - cd build
//   - cmake ..
//   - sudo make install
//
// Link SUscCGL include files in CMakeLists.txt
// Link against libSUscCGL.so
// In the .cpp, include "cgl.h".
// Everything should be set up and ready to go!
//

#include <cgl.h>

using namespace std;

int main(int argc, char **argv){

  int idw, ids, ido;
  string configFile = "@CMAKE_SOURCE_DIRECTORY@/numero.config";
  CglCanvas cv(argc, argv, configFile);

  try{
    if((argc==2) && (string(argv[1])=="cgl.save")){
      cv.initWindow(idw, ids, 400, 400);
      cv.load_meshes_from_file("cgl.save", ids);
    }

    else if (argc > 1){
      cv.initWindow(idw, ids, 800, 800);
      vector<CglMesh*> mesh;
      for (int i=0; i < argc - 1; i++){
        cout << "i: " << i << endl;
        mesh.push_back(new CglMesh(argv[i+1]));
        mesh[i]->meshInfo(0);
        ido = cv.cglObject(mesh[i]);
        cv.cglSetObject(ido, ids);
      }

      if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
        cv.getScene(ids)->place_objects_on_grid();
    }

    else{ throw string("Invalid arguments"); }
  }

  catch(string const& error){
    cerr << "ERROR: " << error << endl;
    return(1);
  }

  
  cv.cglMainLoop();

  return 0;
}
