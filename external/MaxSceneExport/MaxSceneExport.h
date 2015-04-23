#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>

#include <tinyxml.h>
#include <string>
//SIMPLE TYPE


#include <impexp.h>

#include <triobj.h>

struct quat
{
	float x, y, z, w;
};

struct vec3
{
	float x, y, z;
};

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

struct indexed_vertex
{
	indexed_vertex() : pos(0), nrm(0), tex(0), col(0)
	{}
	unsigned int pos, nrm, tex, col;
};