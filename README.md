# GraphicsEngine
GraphicsEngine is a C++ library providing an Open Source Framework to ease the creation of 3D software applications in a scientific context.
![Example](https://cloud.githubusercontent.com/assets/11873158/12479197/d67e8e90-c039-11e5-8c36-1402adbf06c3.png "NumeRO example")

# Installation

Clone this repository on your computer:
```
git clone https://github.com/ICStoolbox/GraphicsEngine.git
```
then type once in a command prompt:
```
cd GraphicsEngine
sh install.sh
```

The install.sh script will automatically download some of the dependencies for GraphicsEngine library and applications (glm, GLEW and libCommons, developped at ICS).

If you later want to modify the sources of GraphicsEngine, you can manually enter the build directory and type in a command prompt:
```
cmake ..
make
make install
```

Other dependencies might be needed, amongst which:
* LINUX: freeglut-dev, libXmu-dev, libXi-dev, cmake.
* MACOS: XQuartz, XCode, cmake.


# Usage

After compiling the dynamic library, one only needs to include the paths to libGraphicsEngine and its include directory in the compiler flag, and insert the following preprocessing directive at the beginning of its application:

        #include <cgl.h>

For more information on the usage of GraphicsEngine, please refer to the Wiki page (coming soon), or the template applications present in the ***APP_*** folders.

### Examples

In order to execute sample applications based on GraphicsEngine, navigate to GraphicsEngine
installation directory and type the following commands in a new prompt:

        cd APP_[Chosen_Example]
        mkdir build
        cd build
        cmake ..
        make [Chosen_Example]

***Replace [Chosen Example] with the name of the chose application (Currently "Numero" or "Chemistry").***

A binary file will be created in your  home/***username***/bin/  folder. 

To execute the app, just call the corresponding executable with the expected arguments.

### Useful Ressources

* [ICS@SU website](http://ics.sorbonne-universites.fr/)
* [CMake download and installation](https://cmake.org/download/)
* [mmgTools, a powerful remeshing toolbox](https://github.com/MmgTools/mmg)
* [Interesting collection of tutorials on modern OpenGL](http://www.opengl-tutorial.org/)
* [Medit documentation, .mesh and .sol file formats documentation](http://www.ann.jussieu.fr/frey/logiciels/Docmedit.dir/Docmedit.html#SECTION00030000000000000000)


# License

SUcgl  
Copyright (c) 2010-2015 ICS-SU.

SUcgl is distributed under the GNU Lesser General Public License
(LGPL). See [License](LICENSE.md)

