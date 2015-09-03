/****************************************************************
*
* CGL: ICS Graphics Library
*
* Original Authors:
*   J. Foulon, P. Frey
*
* CGL: ICS Graphics Library
* Copyright (C) 2014- ICS graphic team
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later
* version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General
* Public License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place, Suite 330,
* Boston, MA 02111-1307 USA
*
****************************************************************/
#ifndef __ICS_GL_h_
#define __ICS_GL_h_

#include <cgl/canvas.h>
#include <cgl/light.h>
#include <cgl/scene.h>
#include <cgl/view.h>
#include <cgl/window.h>
#include <cgl/mesh.h>
#include <cgl/sphere.h>
#include <cgl/cube.h>
#include <cgl/axis.h>
#include <cgl/transform.h>
#include <cgl/shader.h>
#include <cgl/material.h>
#include <cgl/profile.h>
#include <cgl/group.h>
#include <cgl/colorgenerator.h>


/*-- The major/minor/patch version. ---------------------------*/
#define CGL_VERSION_MAJOR    0
#define CGL_VERSION_MINOR    0
#define CGL_VERSION_PATCH    0

/*---------------------------------------------------------------
  Do not modify anything below this line.
 --------------------------------------------------------------*/

/*-- Declare a version string constant ------------------------*/
#define CGL_STR(s)           # s
#define CGL_xSTR(s)          CGL_STR(s)
#define CGL_DOT(a,b)         a ## . ## b
#define CGL_xDOT(a,b)        CGL_DOT(a,b)

#define CGL_VERSION_STRING \
	CGL_xDOT( \
		CGL_xDOT(CGL_VERSION_MAJOR, CGL_VERSION_MINOR), \
		         CGL_VERSION_PATCH)

namespace cgl
{
	inline const char* cglGetVersion()
  {
		return CGL_xSTR(CGL_VERSION_STRING);
  }
}

/*-- Cleanup all internal macros ------------------------------*/
#undef CGL_DOT
#undef CGL_xDOT
#undef CGL_STR
#undef CGL_xSTR
#undef CGL_VERSION_STRING
#undef CGL_VERSION_MAJOR
#undef CGL_VERSION_MINOR
#undef CGL_VERSION_PATCH

#endif
