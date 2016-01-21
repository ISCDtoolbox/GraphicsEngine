#Préparation des répertoires
mkdir -p ~/include/GL/
mkdir -p ~/lib/

#Installation de glm
git clone https://github.com/g-truc/glm.git glm
cd glm
cp -r glm/ ~/include/
cd -
rm -rf glm/

#Installation de glew
git clone git://git.code.sf.net/p/glew/code glew
cd glew
make extensions
make
cp lib/libGLEW.so ~/lib/
cp include/GL/*.h ~/include/GL/
cd -
rm -rf glew/

#Installation de Commons
git clone https://github.com/ICStoolbox/Commons.git Commons
mkdir Commons/build
cd Commons/build
cmake ..
make
make install
cd -
rm -rf Commons/