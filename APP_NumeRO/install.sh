#Preparing directories
mkdir -p ~/bin/
mkdir -p build
cd build

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
		if [ -f ~/bin/numero ]
		then
			rm ~/bin/numero
		fi
	else
		echo "Usage : sh install.sh [-o]"
		exit 1
	fi
fi

#Installing numero executable
if [ \( ! -f ~/bin/numero \) ]
then
    echo "-- Installing Numero"
    cmake ..
    make
    make install
else
	echo "-- Numero already installed. Skipping..."
fi

#Cleaning
cd ../
rm -rf build
