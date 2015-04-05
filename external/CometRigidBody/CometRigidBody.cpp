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

#include "CometRigidBody.h"
#include "Max.h"
#include "object.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <cmath>
#define CometRigidBody_CLASS_ID	Class_ID(0x798ab6c6, 0xb5c89c55)

#define PBLOCK_REF	0
#define PBLOCK_REF_NO	 0


class CometRigidBody : public HelperObject {
	public:
		//Constructor/Destructor
		CometRigidBody();
		~CometRigidBody() {}

		// Loading/Saving
		IOResult Load(ILoad *iload) {return IO_OK;}
		IOResult Save(ISave *isave) {return IO_OK;}

		//From Animatable
		Class_ID ClassID() {return CometRigidBody_CLASS_ID;}		
		SClass_ID SuperClassID() { return HELPER_CLASS_ID; }
		void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

		RefTargetHandle Clone( RemapDir &remap );
		RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, 
			PartID& partID, RefMessage message, BOOL propagate);

		int NumSubs() { return 1; }

		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);

		void DeleteThis() { delete this; }		

		CreateMouseCallBack* GetCreateMouseCallBack();

		ObjectState Eval(TimeValue t);

		void InvalidateUI();

		static CometRigidBody *editOb;

		// Parameter block
		IParamBlock2	*pblock;	//ref 0
};

CometRigidBody *CometRigidBody::editOb = NULL;



class CometRigidBodyClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new CometRigidBody(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return HELPER_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return CometRigidBody_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("CometRigidBody"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};

static CometRigidBodyClassDesc rigidBodyObjDesc;

CometRigidBody::CometRigidBody()
{
	rigidBodyObjDesc.MakeAutoParamBlocks(this);
}

//TODO: Add enumerations for each parameter block
enum { rigidbody_params };


//TODO: Add enumerations for various parameters
enum {
	rigidbody_size,
	rigidbody_mass,
	rigidbody_right
};

static ParamBlockDesc2 rigidBodyParamBlk(
	rigidbody_params, _T("RigidBodyObjectParameters"), 0, &rigidBodyObjDesc, P_AUTO_CONSTRUCT/* + P_AUTO_UI*/, PBLOCK_REF,/*,

	rigidbody_size, _T("size"), TYPE_FLOAT, P_ANIMATABLE, p_end,
	rigidbody_left, _T("test"), TYPE_FLOAT, P_ANIMATABLE, p_end,*/
	p_end
	/*
	//rollout
	IDD_PANEL, "RigidBody", 0, 0, NULL,
	pointobj_size, _T("size"), TYPE_WORLD, P_ANIMATABLE, _T("Size"),
	p_default, 20.0,
	p_ms_default, 20.0,
	p_range, 0.0f, float(1.0E30),
	p_ui, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_POINT_SIZE, IDC_POINT_SIZESPIN, SPIN_AUTOSCALE,
	p_end*/
	);

void CometRigidBody::InvalidateUI()
{
	//rigidBodyParamBlk.InvalidateUI(pblock->LastNotifyParamID());
}


ClassDesc2* GetCometRigidBodyDesc() { 
	static CometRigidBodyClassDesc CometRigidBodyDesc;
	return &CometRigidBodyDesc; 
}

class PointHelpObjCreateCallBack : public CreateMouseCallBack {
	CometRigidBody *ob;
public:
	int proc(ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat);
	void SetObj(CometRigidBody *obj) { ob = obj; }
};

int PointHelpObjCreateCallBack::proc(ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat)
{
	if (!vpt || !vpt->IsAlive())
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}

#ifdef _OSNAP
	if (msg == MOUSE_FREEMOVE)
	{
#ifdef _3D_CREATE
		vpt->SnapPreview(m, m, NULL, SNAP_IN_3D);
#else
		vpt->SnapPreview(m, m, NULL, SNAP_IN_PLANE);
#endif
	}
#endif

	if (msg == MOUSE_POINT || msg == MOUSE_MOVE) {
		switch (point) {
		case 0: {

					// Find the node and plug in the wire color
					ULONG handle;
					ob->NotifyDependents(FOREVER, (PartID)&handle, REFMSG_GET_NODE_HANDLE);
					INode *node;
					node = GetCOREInterface()->GetINodeByHandle(handle);
					if (node) {
						Point3 color = GetUIColor(COLOR_POINT_OBJ);
						node->SetWireColor(RGB(color.x*255.0f, color.y*255.0f, color.z*255.0f));
					}

					//ob->suspendSnap = TRUE;
#ifdef _3D_CREATE	
					mat.SetTrans(vpt->SnapPoint(m, m, NULL, SNAP_IN_3D));
#else	
					mat.SetTrans(vpt->SnapPoint(m, m, NULL, SNAP_IN_PLANE));
#endif				
					break;
		}

		case 1:
#ifdef _3D_CREATE	
			mat.SetTrans(vpt->SnapPoint(m, m, NULL, SNAP_IN_3D));
#else	
			mat.SetTrans(vpt->SnapPoint(m, m, NULL, SNAP_IN_PLANE));
#endif
			if (msg == MOUSE_POINT) {
				//ob->suspendSnap = FALSE;
				return 0;
			}
			break;
		}
	}
	else
	if (msg == MOUSE_ABORT) {
		return CREATE_ABORT;
	}

	return 1;
}

static PointHelpObjCreateCallBack pointHelpCreateCB;

CreateMouseCallBack* CometRigidBody::GetCreateMouseCallBack()
{
	pointHelpCreateCB.SetObj(this);
	return(&pointHelpCreateCB);
}

ObjectState CometRigidBody::Eval(TimeValue t)
{
	return ObjectState(this);
}

RefTargetHandle CometRigidBody::Clone(RemapDir& remap)
{
	CometRigidBody* newob = new CometRigidBody();
	//newob->showAxis = showAxis;
	//newob->axisLength = axisLength;
	//newob->ReplaceReference(0, remap.CloneRef(pblock2));
	BaseClone(this, newob, remap);
	return(newob);
}

RefResult CometRigidBody::NotifyRefChanged(
	const Interval& changeInt, RefTargetHandle hTarget,
	PartID& partID, RefMessage message, BOOL propagate)
{
	switch (message) {
	case REFMSG_CHANGE:
		if (editOb == this) InvalidateUI();
		break;
	}
	return(REF_SUCCEED);
}

int CometRigidBody::Display(TimeValue t, INode* inode, ViewExp *vpt, int flags)
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
		vpt->getGW()->setColor(TEXT_COLOR, GetUIColor(COLOR_SELECTION));
		vpt->getGW()->setColor(LINE_COLOR, GetUIColor(COLOR_SELECTION));
	}
	else
	{
		vpt->getGW()->setColor(TEXT_COLOR, color);
		vpt->getGW()->setColor(LINE_COLOR, color);
	}

	//Draw on top
	int limits = vpt->getGW()->getRndLimits();
	vpt->getGW()->setRndLimits(limits & ~GW_Z_BUFFER);
	//---------

	const int steps = 16;
	Point3 pts[steps+1];
	float size = 10.0f;
	for (int i = 0; i < steps; i++)
	{
		float a = 6.28f / ((float)(steps-1)) * i;
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
	/*
	// X
	pts[0] = Point3(-size, 0.0f, 0.0f); pts[1] = Point3(size, 0.0f, 0.0f);
	vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);
	// Y
	pts[0] = Point3(0.0f, -size, 0.0f); pts[1] = Point3(0.0f, size, 0.0f);
	vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);
	// Z
	pts[0] = Point3(0.0f, 0.0f, -size); pts[1] = Point3(0.0f, 0.0f, size);
	vpt->getGW()->polyline(2, pts, NULL, NULL, FALSE, NULL);
	*/



	return 0;
}