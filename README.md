![Example](Screenshot_1.png?raw=true "NumeRO example")

# INTRODUCTION

SUscCGL (Sorbonne Universités SCientific Computer Graphics Library) is a C++ library, developed at ICS@SU (Institut du Calcul et de la Simulation), providing an Open Source Framework to ease the creation of 3D software applications in a scientific context.

Among the different features of SUscCGL, the developer will find all the tools necessary for:
- Window and subwindow creation and management
- 3D objects manipulation
- Custom user interaction, via a graphical interface or mouse and keyboard shortcuts
- Extensive rendering options (material, lighting, background, color palettes... customisation)
- ...

# Installation (Mac OS & Linux)

Please note that currently, only the Linux Platforms are fully supported. 
Mac OS support should be implemented soon. 

1. Download the repository to your computer (via git or direct source download)
2. Install the external dependencies depending on your platform (Cf "Dependencies" below)
3. Navigate to SUscCGL installation directory
4. In a command prompt, type the following commands:

        mkdir build 
        cd build
        cmake ..
        make SUscCGL

If no error occured, a dynamic library file (libSUscCGL.so or libSUscCGL.dylib) should appear in the build folder.

## Dependencies
### Linux
The following packages need to be installed in order to use the library:
- cmake
- glew-devel (might be removed)
- freeglut-devel
- libXmu-devel
- libXi-devel

Use "sudo apt-get install [Package_Name]" in order to install those packages.

### Mac OS
As stated above, Mac OS is not ***yet*** fully supported. At least XCode and CMake must be operational in order to compile the library, however serious performance issues make the practical use of the provided tools impossible at the time being.

We hope however that a fix will be encountered really soon!

# Usage

After compiling the dynamic library, one only needs to include the paths to libSUscCGL and its include directory in the compiler flag, and insert the following preprocessing directive at the beginning of its application:

        #include <cgl.h>

For more information on the usage of SUscCGL, please refer to the Wiki page (coming soon), or the template applications present in the ***APP_*** folders.

### Examples

In order to execute sample applications based on SUscCGL, navigate to SUscCGL
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
(LGPL).  This implies that you may link SUcgl into proprietary
applications, provided you follow the rules stated in the LGPL.  You can
also modify SUcgl; if you distribute a modified version, you must
distribute it under the terms of the LGPL, which in particular states
that you must release the source code for the modified software.  

See COPYING.LESSER for more information.

