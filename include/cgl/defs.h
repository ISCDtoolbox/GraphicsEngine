/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef _CGL_DEF_H
#define _CGL_DEF_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace std;

#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef WIN32
#ifdef CGL_DLL
#ifdef CGL_LIB
#define CGL_API __declspec(dllexport)
#else
#define CGL_API __declspec(dllimport)
#endif
#else
#define CGL_API
#endif
#else
#define CGL_API
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>



#endif
