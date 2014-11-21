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
#include <triobj.h>
#include <modstack.h>
#include <iskin.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <MeshIO.h>
//SIMPLE TYPE


#include <impexp.h>

#define SKIN_CLASS_ID Class_ID(9815843, 87654)


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

struct indexed_vertex
{
	unsigned int pos, nrm, tex, col;
};
