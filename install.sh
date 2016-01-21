#Préparation des répertoires
mkdir -p ~/include/GL/
mkdir -p ~/lib/
mkdir -p build
cd build

#Installation de glm
if [ ! -d ~/include/glm/ ]
then
    echo "-- Installing glm"
    git clone https://github.com/g-truc/glm.git glm
    cd glm
    cp -r glm/ ~/include/
    cd -
    rm -rf glm/
fi

#Installation de glew
if [ \( ! -f ~/lib/libGLEW.so \) -a  \( ! -f ~/lib/libGLEW.dylib \) ]
then
    echo "-- Installing glew"
    git clone git://git.code.sf.net/p/glew/code glew
    cd glew
    make extensions
    make
    cp lib/libGLEW.so ~/lib/
    cp include/GL/*.h ~/include/GL/
    cd -
    rm -rf glew/
fi

#Installation de Commons
if [ \( ! -f ~/lib/libCommons.so \) -a \( ! -f ~/lib/libCommons.dylib \) ]
then
    echo "-- Installing Commons"
    git clone https://github.com/ICStoolbox/Commons.git Commons
    mkdir Commons/build
    cd Commons/build
    cmake ..
    make
    make install
    cd -
    rm -rf Commons/
fi

#Installation de SUscCGL
if [ \( ! -f ~/lib/libSUscCGL.so \)  -a \( ! -f ~/lib/libSUscCGL.dylib \) ]
then
    echo "-- Installing SUscCGL"
    cmake ..
    make
    make install
fi

cd ../
