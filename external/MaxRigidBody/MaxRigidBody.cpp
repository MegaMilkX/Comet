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
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MaxRigidBody.h"
#include "Max.h"
#include "object.h"
#include "simpmod.h"
#define MaxRigidBody_CLASS_ID	Class_ID(0x5ba95448, 0x5295c5e5)


#define PBLOCK_REF	0


class MaxRigidBody : public Modifier
{
public:
	//Constructor/Destructor
	MaxRigidBody();
	virtual ~MaxRigidBody();

	virtual void DeleteThis() { delete this; }

	// From Animatable
	virtual const TCHAR *GetObjectName() { return GetString(IDS_CLASS_NAME); }

		virtual ChannelMask ChannelsUsed()  { return GEOM_CHANNEL|TOPO_CHANNEL; }
	#pragma message(TODO("Add the channels that the modifier actually modifies"))
	virtual ChannelMask ChannelsChanged() { return GEOM_CHANNEL; }
		
	#pragma message(TODO("Return the ClassID of the object that the modifier can modify"))
	Class_ID InputType() {return defObjectClassID;}

	virtual void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
	virtual void NotifyInputChanged(const Interval& changeInt, PartID partID, RefMessage message, ModContext *mc);

	virtual void NotifyPreCollapse(INode *node, IDerivedObject *derObj, int index);
	virtual void NotifyPostCollapse(INode *node,Object *obj, IDerivedObject *derObj, int index);


	virtual Interval LocalValidity(TimeValue t);

	// From BaseObject
	#pragma message(TODO("Return true if the modifier changes topology"))
	virtual BOOL ChangeTopology() {return FALSE;}
	
	virtual CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

	virtual BOOL HasUVW();
	virtual void SetGenUVW(BOOL sw);


	virtual void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
	virtual void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);


	virtual Interval GetValidity(TimeValue t);

	// Automatic texture support
	
	// Loading/Saving
	virtual IOResult Load(ILoad *iload);
	virtual IOResult Save(ISave *isave);

	//From Animatable
	virtual Class_ID ClassID() {return MaxRigidBody_CLASS_ID;}
	virtual SClass_ID SuperClassID() { return OSM_CLASS_ID; }
	virtual void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

	virtual RefTargetHandle Clone( RemapDir &remap );
	virtual RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate);

	virtual int NumSubs() { return 1; }
	virtual TSTR SubAnimName(int /*i*/) { return GetString(IDS_PARAMS); }
	virtual Animatable* SubAnim(int /*i*/) { return pblock; }

	// TODO: Maintain the number or references here
	virtual int NumRefs() { return 1; }
	virtual RefTargetHandle GetReference(int i);

	virtual int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
	virtual IParamBlock2* GetParamBlock(int /*i*/) { return pblock; } // return i'th ParamBlock
	virtual IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

	int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags, ModContext* mc);

protected:
	virtual void SetReference(int , RefTargetHandle rtarg);

private:
	// Parameter block
	IParamBlock2 *pblock; //ref 0
};



void MaxRigidBody::SetReference( int i, RefTargetHandle rtarg )
{
	if (i == PBLOCK_REF)
	{
		pblock=(IParamBlock2*)rtarg;
	}
}

RefTargetHandle MaxRigidBody::GetReference( int i)
{
	if (i == PBLOCK_REF)
	{
		return pblock;
	}
	return nullptr;
}



class MaxRigidBodyClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new MaxRigidBody(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return OSM_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxRigidBody_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MaxRigidBody"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetMaxRigidBodyDesc() { 
	static MaxRigidBodyClassDesc MaxRigidBodyDesc;
	return &MaxRigidBodyDesc; 
}




enum { maxrigidbody_params };


//TODO: Add enums for various parameters
enum { 
	pb_shape,
	pb_x,
	pb_y,
	pb_z,
	pb_mass,
	pb_isstatic,
	pb_fric
};




static ParamBlockDesc2 maxrigidbody_param_blk ( maxrigidbody_params, _T("params"),  0, GetMaxRigidBodyDesc(), 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	// params
	pb_shape, _T("shape"),
		TYPE_INT,
		P_RESET_DEFAULT + P_ANIMATABLE, IDS_SPIN,
		p_ui, TYPE_INT_COMBOBOX,
		IDC_COMBO,
		8,
		IDS_SPHERE, IDS_BOX, IDS_CYLLINDER, IDS_CAPSULE, IDS_CONE, IDS_TRIMESH, IDS_CONVEX, IDS_PLANE,
		p_vals, 0, 1, 2, 3, 4, 5, 6, 7,
		p_default, 0,
		p_end,
	pb_x, 			_T("x"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_SPIN, 
		p_default, 		1.0f, 
		p_range, 		0.0f,1000.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_EDIT,	IDC_SPIN, 0.01f, 
		p_end,
	pb_y, _T("y"), TYPE_FLOAT, P_ANIMATABLE, IDS_SPIN,
		p_default, 1.0f,
		p_range, 0.0f, 1000.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDITY, IDC_SPINY, 0.01f,
		p_end,
	pb_z, _T("z"), TYPE_FLOAT, P_ANIMATABLE, IDS_SPIN,
		p_default, 1.0f,
		p_range, 0.0f, 1000.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_EDITZ, IDC_SPINZ, 0.01f,
		p_end,
	pb_mass, _T("mass"), TYPE_FLOAT, P_ANIMATABLE, IDS_SPIN,
		p_default, 1.0f,
		p_range, 0.0f, 1000.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_MASSEDIT, IDC_MASSSPIN, 0.01f,
		p_end,
	pb_isstatic, _T("isstatic"), TYPE_BOOL, P_ANIMATABLE, IDS_SPIN,
		p_default, true,
		p_ui, TYPE_SINGLECHEKBOX, IDC_ISSTATIC,
		p_end,
	pb_fric, _T("fric"), TYPE_FLOAT, P_ANIMATABLE, IDS_SPIN,
		p_default, 1.0f,
		p_range, 0.0f, 1000.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_FRICEDIT, IDC_FRICSPIN, 0.01f,
		p_end,
	p_end
	);




//--- MaxRigidBody -------------------------------------------------------
MaxRigidBody::MaxRigidBody()
	: pblock(nullptr)
{
	GetMaxRigidBodyDesc()->MakeAutoParamBlocks(this);
}

MaxRigidBody::~MaxRigidBody()
{
}

/*===========================================================================*\
 |	The validity of the parameters.  First a test for editing is performed
 |  then Start at FOREVER, and intersect with the validity of each item
\*===========================================================================*/
Interval MaxRigidBody::LocalValidity(TimeValue /*t*/)
{
	// if being edited, return NEVER forces a cache to be built 
	// after previous modifier.
	if (TestAFlag(A_MOD_BEING_EDITED))
		return NEVER;  
	#pragma message(TODO("Return the validity interval of the modifier"))
	return NEVER;
}


/*************************************************************************************************
*
	Between NotifyPreCollapse and NotifyPostCollapse, Modify is
	called by the system.  NotifyPreCollapse can be used to save any plugin dependant data e.g.
	LocalModData
*
\*************************************************************************************************/

void MaxRigidBody::NotifyPreCollapse(INode* /*node*/, IDerivedObject* /*derObj*/, int /*index*/)
{
	#pragma message(TODO("Perform any Pre Stack Collapse methods here"))
}



/*************************************************************************************************
*
	NotifyPostCollapse can be used to apply the modifier back onto to the stack, copying over the
	stored data from the temporary storage.  To reapply the modifier the following code can be 
	used

	Object *bo = node->GetObjectRef();
	IDerivedObject *derob = NULL;
	if(bo->SuperClassID() != GEN_DERIVOB_CLASS_ID)
	{
		derob = CreateDerivedObject(obj);
		node->SetObjectRef(derob);
	}
	else
		derob = (IDerivedObject*) bo;

	// Add ourselves to the top of the stack
	derob->AddModifier(this,NULL,derob->NumModifiers());

*
\*************************************************************************************************/

void MaxRigidBody::NotifyPostCollapse(INode* /*node*/,Object* /*obj*/, IDerivedObject* /*derObj*/, int /*index*/)
{
	#pragma message(TODO("Perform any Post Stack collapse methods here."))

}


/*************************************************************************************************
*
	ModifyObject will do all the work in a full modifier
    This includes casting objects to their correct form, doing modifications
	changing their parameters, etc
*
\************************************************************************************************/


void MaxRigidBody::ModifyObject(TimeValue /*t*/, ModContext& /*mc*/, ObjectState* /*os*/, INode* /*node*/) 
{
	#pragma message(TODO("Add the code for actually modifying the object"))
}


void MaxRigidBody::BeginEditParams( IObjParam* ip, ULONG flags, Animatable* prev )
{
	TimeValue t = ip->GetTime();
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_BEGIN_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_ON);
	SetAFlag(A_MOD_BEING_EDITED);	

	GetMaxRigidBodyDesc()->BeginEditParams(ip, this, flags, prev);
}

void MaxRigidBody::EndEditParams( IObjParam *ip, ULONG flags, Animatable *next)
{
	GetMaxRigidBodyDesc()->EndEditParams(ip, this, flags, next);

	TimeValue t = ip->GetTime();
	ClearAFlag(A_MOD_BEING_EDITED);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_END_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_OFF);
}



Interval MaxRigidBody::GetValidity(TimeValue /*t*/)
{
	Interval valid = FOREVER;
	#pragma message(TODO("Return the validity interval of the modifier"))
	return valid;
}




RefTargetHandle MaxRigidBody::Clone(RemapDir& remap)
{
	MaxRigidBody* newmod = new MaxRigidBody();
	#pragma message(TODO("Add the cloning code here"))
	newmod->ReplaceReference(PBLOCK_REF,remap.CloneRef(pblock));
	BaseClone(this, newmod, remap);
	return(newmod);
}


//From ReferenceMaker 
RefResult MaxRigidBody::NotifyRefChanged(
		const Interval& /*changeInt*/, RefTargetHandle hTarget,
		PartID& /*partID*/,  RefMessage message, BOOL /*propagate*/) 
{
	#pragma message(TODO("Add code to handle the various reference changed messages"))
	switch (message)
	{
	case REFMSG_TARGET_DELETED:
		{
			if (hTarget == pblock)
			{
				pblock = nullptr;
			}
		}
		break;
	}
	return REF_SUCCEED;
}

/****************************************************************************************
*
 	NotifyInputChanged is called each time the input object is changed in some way
 	We can find out how it was changed by checking partID and message
*
\****************************************************************************************/

void MaxRigidBody::NotifyInputChanged(const Interval& /*changeInt*/, PartID /*partID*/, RefMessage /*message*/, ModContext* /*mc*/)
{

}



//From Object
BOOL MaxRigidBody::HasUVW() 
{ 
	#pragma message(TODO("Return whether the object has UVW coordinates or not"))
	return TRUE; 
}

void MaxRigidBody::SetGenUVW(BOOL sw) 
{  
	if (sw==HasUVW()) 
		return;
	#pragma message(TODO("Set the plugin internal value to sw"))
}

IOResult MaxRigidBody::Load(ILoad* /*iload*/)
{
	#pragma message(TODO("Add code to allow plugin to load its data"))
	
	return IO_OK;
}

IOResult MaxRigidBody::Save(ISave* /*isave*/)
{
	#pragma message(TODO("Add code to allow plugin to save its data"))
	
	return IO_OK;
}

int MaxRigidBody::Display(TimeValue t, INode* inode, ViewExp *vpt, int flags, ModContext* mc)
{
	if (!vpt || !vpt->IsAlive())
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}

	Color color(inode->GetWireColor());
	color = Color(0.0f, 0.9f, 0.1f);
	if (inode->Selected())
	{
		color = Color(0.0f, 1.0f, 0.0f);
		//vpt->getGW()->setColor(TEXT_COLOR, GetUIColor(COLOR_SELECTION));
		//vpt->getGW()->setColor(LINE_COLOR, GetUIColor(COLOR_SELECTION));
	}
	else
	{
		color = Color(0.2f, 0.9f, 0.2f);
	}
	vpt->getGW()->setColor(TEXT_COLOR, color);
	vpt->getGW()->setColor(LINE_COLOR, color);

	//Draw on top
	int limits = vpt->getGW()->getRndLimits();
	vpt->getGW()->setRndLimits(limits & ~GW_Z_BUFFER);
	//---------

	const int steps = 16;
	Point3 pts[steps + 1];
	float size = 10.0f;
	float height = 1.0f;
	float depth = 1.0f;
	int type = 0;

	pblock->GetValue(pb_shape, 0, type, FOREVER);


	if (type == 0) //Sphere
	{
		pblock->GetValue(pb_x, 0, size, FOREVER);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = 0;
			pts[i].y = sin(a) * size;
			pts[i].z = cos(a) * size;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = 0;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = sin(a) * size;
			pts[i].y = 0;
			pts[i].z = cos(a) * size;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
	}
	else if (type == 1) //Box
	{
		pblock->GetValue(pb_x, 0, size, FOREVER);
		pblock->GetValue(pb_y, 0, height, FOREVER);
		pblock->GetValue(pb_z, 0, depth, FOREVER);
		//size *= 0.5f; height *= 0.5f; depth *= 0.5f;
		Point3 boxPts[6];
		boxPts[0].x = size; boxPts[0].y = depth; boxPts[0].z = -height;
		boxPts[1].x = size; boxPts[1].y = depth; boxPts[1].z = height;
		boxPts[2].x = -size; boxPts[2].y = depth; boxPts[2].z = height;
		boxPts[3].x = -size; boxPts[3].y = depth; boxPts[3].z = -height;
		boxPts[4].x = size; boxPts[4].y = depth; boxPts[4].z = -height;
		vpt->getGW()->polyline(5, boxPts, NULL, NULL, FALSE, NULL);

		boxPts[0].x = size; boxPts[0].y = -depth; boxPts[0].z = -height;
		boxPts[1].x = size; boxPts[1].y = -depth; boxPts[1].z = height;
		boxPts[2].x = -size; boxPts[2].y = -depth; boxPts[2].z = height;
		boxPts[3].x = -size; boxPts[3].y = -depth; boxPts[3].z = -height;
		boxPts[4].x = size; boxPts[4].y = -depth; boxPts[4].z = -height;
		vpt->getGW()->polyline(5, boxPts, NULL, NULL, FALSE, NULL);

		boxPts[0].x = size; boxPts[0].y = depth; boxPts[0].z = -height;
		boxPts[1].x = size; boxPts[1].y = -depth; boxPts[1].z = -height;
		vpt->getGW()->polyline(2, boxPts, NULL, NULL, FALSE, NULL);

		boxPts[0].x = size; boxPts[0].y = depth; boxPts[0].z = height;
		boxPts[1].x = size; boxPts[1].y = -depth; boxPts[1].z = height;
		vpt->getGW()->polyline(2, boxPts, NULL, NULL, FALSE, NULL);

		boxPts[0].x = -size; boxPts[0].y = depth; boxPts[0].z = height;
		boxPts[1].x = -size; boxPts[1].y = -depth; boxPts[1].z = height;
		vpt->getGW()->polyline(2, boxPts, NULL, NULL, FALSE, NULL);

		boxPts[0].x = -size; boxPts[0].y = depth; boxPts[0].z = -height;
		boxPts[1].x = -size; boxPts[1].y = -depth; boxPts[1].z = -height;
		vpt->getGW()->polyline(2, boxPts, NULL, NULL, FALSE, NULL);
	}
	else if (type == 2) //Cyllinder	TODO: Разобраться зачем цилиндру третий скаляр (скругление краев? или что?)
	{
		pblock->GetValue(pb_x, 0, size, FOREVER);
		pblock->GetValue(pb_y, 0, height, FOREVER);
		pblock->GetValue(pb_z, 0, depth, FOREVER);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = -height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[0].x = cos(a) * size;
			pts[0].y = sin(a) * size;
			pts[0].z = height;
			pts[1].x = cos(a) * size;
			pts[1].y = sin(a) * size;
			pts[1].z = -height;
			vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);
		}
	}
	else if (type == 3) //Capsule
	{
		pblock->GetValue(pb_x, 0, size, FOREVER);
		pblock->GetValue(pb_y, 0, height, FOREVER);

		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = -height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		pts[0].x = 0; pts[0].y = -size; pts[0].z = -height;
		pts[1].x = 0; pts[1].y = -size; pts[1].z = height;
		vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);

		pts[0].x = 0; pts[0].y = size; pts[0].z = -height;
		pts[1].x = 0; pts[1].y = size; pts[1].z = height;
		vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);

		pts[0].x = -size; pts[0].y = 0; pts[0].z = -height;
		pts[1].x = -size; pts[1].y = 0; pts[1].z = height;
		vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);

		pts[0].x = size; pts[0].y = 0; pts[0].z = -height;
		pts[1].x = size; pts[1].y = 0; pts[1].z = height;
		vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 3.14f / ((float)(steps - 1)) * i - 1.57f;
			pts[i].x = sin(a) * size;
			pts[i].y = 0;
			pts[i].z = cos(a) * size + height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 3.14f / ((float)(steps - 1)) * i - 1.57f;
			pts[i].x = 0;
			pts[i].y = sin(a) * size;
			pts[i].z = cos(a) * size + height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 3.14f / ((float)(steps - 1)) * i + 1.57f;
			pts[i].x = sin(a) * size;
			pts[i].y = 0;
			pts[i].z = cos(a) * size - height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 3.14f / ((float)(steps - 1)) * i + 1.57f;
			pts[i].x = 0;
			pts[i].y = sin(a) * size;
			pts[i].z = cos(a) * size - height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);
	}
	else if (type == 4) //Cone
	{
		pblock->GetValue(pb_x, 0, size, FOREVER);
		pblock->GetValue(pb_y, 0, height, FOREVER);

		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[i].x = cos(a) * size;
			pts[i].y = sin(a) * size;
			pts[i].z = -height;
		}
		vpt->getGW()->polyline(steps, pts, NULL, NULL, FALSE, NULL);

		for (int i = 0; i < steps; i++)
		{
			float a = 6.28f / ((float)(steps - 1)) * i;
			pts[0].x = 0;
			pts[0].y = 0;
			pts[0].z = height;
			pts[1].x = cos(a) * size;
			pts[1].y = sin(a) * size;
			pts[1].z = -height;
			vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);
		}
	}
	else if (type == 5) //TriMesh
	{
	}
	else if (type == 6) //Convex
	{
	}
	else if (type == 7) //InfiPlane
	{

	}

	return 0;
}