#INTRODUCTION

SUscCGL (Sorbonne Universités SCientific Computer Graphics Library) is an Open Source
library, aiming to ease the work of graphical development for scientific
vizualization.

SUcgl provides an user-friendly interface for the creation of a 3D
application:
- Scene creation
- Lighting control
- Object manipulation and interaction
- ...



#SETUP

##Installation

- Download the repository to your computer (via git or direct source download)
- Install the external libraries (Cf "Dependencies" below)
- Navigate to SUscCGL installation directory
- In a prompt, type the following commands:
> mkdir build
> cd build
> cmake ..
> make SUscCGL

If no error occured, a file named libSUscCGL.so should be created in the
folder, else, please consult the "Troubleshooting" section below

##Dependencies

The following packages need to be installed on Linux in order to use the library:
- cmake
- glew-devel
- freeglut-devel
- libXmu-devel
- libXi-devel
Use "sudo apt-get install [Package_Name]" in order to install those packages.

##Troubleshooting

Lorem ipsum truc...



#Usage

Please consult the provided examples and the documentation in order to get
further instructions about the use of SUscCGL.

##Examples

In order to execute sample applications based on SUscCGL, navigate to SUscCGL
installation directory and type the following commands in a new prompt:
> cd APP_[Chosen Example]
> mkdir build
> cd build
> cmake ..
> make [Chosen Example]
> [Chosen Example]

Replace [Chosen Example] with the name of one of the applications.



#License

SUcgl
Copyright (c) 2010-2015 ICS-SU.

SUcgl is distributed under the GNU Lesser General Public License
(LGPL).  This implies that you may link SUcgl into proprietary
applications, provided you follow the rules stated in the LGPL.  You can
also modify SUcgl; if you distribute a modified version, you must
distribute it under the terms of the LGPL, which in particular states
that you must release the source code for the modified software.  

See COPYING.LESSER for more information.


