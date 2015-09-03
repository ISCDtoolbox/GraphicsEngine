/****************************************************************
*
* GLIC: ICS Graphics Library
*
* Original Authors:
*   J. Foulon, P. Frey
*
* GLIC: ICS Graphics Library
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

#include <glic/canvas.h>
#include <glic/light.h>
#include <glic/scene.h>
#include <glic/view.h>
#include <glic/window.h>
#include <glic/mesh.h>
#include <glic/sphere.h>
#include <glic/cube.h>
#include <glic/axis.h>
#include <glic/transform.h>
#include <glic/shader.h>
#include <glic/material.h>
#include <glic/profile.h>
#include <glic/group.h>
#include <glic/colorgenerator.h>


/*-- The major/minor/patch version. ---------------------------*/
#define GLIC_VERSION_MAJOR    0
#define GLIC_VERSION_MINOR    0
#define GLIC_VERSION_PATCH    0

/*---------------------------------------------------------------
  Do not modify anything below this line.
 --------------------------------------------------------------*/

/*-- Declare a version string constant ------------------------*/
#define GLIC_STR(s)           # s
#define GLIC_xSTR(s)          GLIC_STR(s)
#define GLIC_DOT(a,b)         a ## . ## b
#define GLIC_xDOT(a,b)        GLIC_DOT(a,b)

#define GLIC_VERSION_STRING \
	GLIC_xDOT( \
		GLIC_xDOT(GLIC_VERSION_MAJOR, GLIC_VERSION_MINOR), \
		         GLIC_VERSION_PATCH)

namespace glic
{
	inline const char* glicGetVersion()
  {
		return GLIC_xSTR(GLIC_VERSION_STRING);
  }
}

/*-- Cleanup all internal macros ------------------------------*/
#undef GLIC_DOT
#undef GLIC_xDOT
#undef GLIC_STR
#undef GLIC_xSTR
#undef GLIC_VERSION_STRING
#undef GLIC_VERSION_MAJOR
#undef GLIC_VERSION_MINOR
#undef GLIC_VERSION_PATCH

#endif
