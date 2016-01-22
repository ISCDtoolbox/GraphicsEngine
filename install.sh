#Preparing directories
mkdir -p ~/include/GL/
mkdir -p ~/lib/
mkdir -p build
cd build
platform=$(uname)

#Parsing arguments
if [ $# -gt 1 ]
then
    echo "Usage : sh install.sh [-o]"
	exit 1
fi
if [ $# -eq 1 ]
then
	if [ $1 == "-o" ]
	then
		rm -rf ~/lib/libSUscCGL.*
		rm -rf ~/lib/libCommons.*
		rm -rf ~/include/SUscCGL
		rm -rf ~/include/glm
		rm -rf ~/include/GL
	else
		echo "Usage : sh install.sh [-o]"
		exit 1
	fi
fi

#Installing glm
if [ ! -d ~/include/glm/ ]
then
    echo "-- Installing glm"
    git clone https://github.com/g-truc/glm.git glm
    cd glm
    cp -r ./glm ~/include/
    cd -
    rm -rf glm/
else
	echo "-- glm already installed. Skipping..."
fi

#Installing glew
if [ ${platform} != "Darwin" ]
then
	if [ \( ! -f ~/lib/libGLEW.so \) -a  \( ! -f ~/lib/libGLEW.dylib \) ]
	then
		echo "-- Installing glew"
		git clone https://github.com/nigels-com/glew.git glew
		cd glew
   	 	make extensions
    	make
    	cp lib/libGLEW.so ~/lib/
    	cp include/GL/*.h ~/include/GL/
    	cd -
    	rm -rf glew/
	else
		echo "-- glew already installed. Skipping..."
	fi
else
	echo "-- Detected OS = " ${platform} ", skipping glew installation."
fi

#Installing Commons
if [ \( ! -f ~/lib/libCommons.so \) -a \( ! -f ~/lib/libCommons.dylib \) ]
then
    echo "-- Installing Commons"
    git clone https://github.com/ICStoolbox/Commons.git Commons
    mkdir Commons/build
    cd Commons/build
    cmake ..
    make install
    cd -
    rm -rf Commons/
else
	echo "-- Commons already installed. Skipping..."
fi

#Installing SUscCGL
if [ \( ! -f ~/lib/libSUscCGL.so \)  -a \( ! -f ~/lib/libSUscCGL.dylib \) ]
then
    echo "-- Installing SUscCGL"
    cmake ..
    make
    make install
else
	echo "-- SUscCGL already installed. Skipping..."
fi

cd ../
