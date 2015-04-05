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

#include "MaxSceneExport.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdmat.h>

#include <modstack.h>

#include <fstream>

#include <MeshIO.h>

#define MaxSceneExport_CLASS_ID	Class_ID(0x73245529, 0xc3f48ef2)

class MaxSceneExport : public SceneExport {
public:
	//Constructor/Destructor
	MaxSceneExport();
	~MaxSceneExport();

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int  DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	void ProcessNode(INode* node, TiXmlNode* xmlNode);
	void WriteSceneGraphToFile(std::string name);

	void _fillMaterials(Mtl* mtl, TiXmlElement* e);

	TriObject* _getTriObjFromNode(INode* node, BOOL &deleteIt);
	void _exportMeshNode(INode* node, MeshIO::MeshIO* meshfile);

	TiXmlDocument doc;

	Interface* ip;

	std::string targetFileName;
};



class MaxSceneExportClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new MaxSceneExport(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxSceneExport_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MaxSceneExport"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetMaxSceneExportDesc() { 
	static MaxSceneExportClassDesc MaxSceneExportDesc;
	return &MaxSceneExportDesc; 
}





INT_PTR CALLBACK MaxSceneExportOptionsDlgProc(HWND hWnd,UINT message,WPARAM,LPARAM lParam) {
	static MaxSceneExport* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MaxSceneExport *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- MaxSceneExport -------------------------------------------------------

glm::quat matrix43ToQuat(Matrix3 mat3)
{
	glm::mat3 m3;
	m3[0][0] = mat3.GetRow(0).x; m3[0][1] = mat3.GetRow(0).y; m3[0][2] = mat3.GetRow(0).z;
	m3[1][0] = mat3.GetRow(1).x; m3[1][1] = mat3.GetRow(1).y; m3[1][2] = mat3.GetRow(1).z;
	m3[2][0] = mat3.GetRow(2).x; m3[2][1] = mat3.GetRow(2).y; m3[2][2] = mat3.GetRow(2).z;
	glm::quat qt = glm::toQuat(m3);
	return qt;
}

vec3 getScale(Matrix3 mat3)
{
	float x, y, z;
	vec3 v3;
	x = mat3.GetRow(0).x; y = mat3.GetRow(0).y; z = mat3.GetRow(0).z; //X axis
	v3.x = sqrt(x*x + y*y + z*z);
	x = mat3.GetRow(2).x; y = mat3.GetRow(2).y; z = mat3.GetRow(2).z; //Z
	v3.y = sqrt(x*x + y*y + z*z);
	x = mat3.GetRow(1).x; y = mat3.GetRow(1).y; z = mat3.GetRow(1).z; //Y
	v3.z = sqrt(x*x + y*y + z*z);
	return v3;
}

IParamBlock2* GetParamBlock(ReferenceMaker* obj)
{
	int nRefs = obj->NumRefs();
	for (int i = 0; i < nRefs; ++i)
	{
		ReferenceTarget* ref = obj->GetReference(i);

		if (ref && ref->SuperClassID() == PARAMETER_BLOCK2_CLASS_ID)
		{
			return dynamic_cast<IParamBlock2*>(ref);
		}
	}
	return NULL;
}

MaxSceneExport::MaxSceneExport()
{

}

MaxSceneExport::~MaxSceneExport() 
{

}

int MaxSceneExport::ExtCount()
{
	//#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *MaxSceneExport::Ext(int /*i*/)
{		
	//#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("graph");
}

const TCHAR *MaxSceneExport::LongDesc()
{
	//#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("Comet Engine Scene Hierarchy File");
}
	
const TCHAR *MaxSceneExport::ShortDesc() 
{			
	//#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Comet Hierarchy");
}

const TCHAR *MaxSceneExport::AuthorName()
{			
	//#pragma message(TODO("Return ASCII Author name"))
	return _T("Comet Engine");
}

const TCHAR *MaxSceneExport::CopyrightMessage() 
{	
	//#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *MaxSceneExport::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *MaxSceneExport::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int MaxSceneExport::Version()
{				
	//#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void MaxSceneExport::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

BOOL MaxSceneExport::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	//#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}


int	MaxSceneExport::DoExport(const TCHAR* name, ExpInterface* ei, Interface* ip, BOOL suppressPrompts, DWORD options)
{
	//#pragma message(TODO("Implement the actual file Export here and"))
	this->ip = ip;

	if(!suppressPrompts)
		DialogBoxParam(hInstance, 
		MAKEINTRESOURCE(IDD_DIALOG1),
				GetActiveWindow(), 
				MaxSceneExportOptionsDlgProc, (LPARAM)this);

	INode* rootNode = ip->GetRootNode();

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	ProcessNode(rootNode, &doc);

	std::wstring wname(name);
	std::string n(wname.begin(), wname.end());
	targetFileName = n;
	WriteSceneGraphToFile(n); //This only saves the scene graph (.graph)

	//#pragma message(TODO("return TRUE If the file is exported properly"))
	return true;
}

enum {
	pb_shape,
	pb_x,
	pb_y,
	pb_z,
	pb_mass,
	pb_isstatic,
	pb_fric
};


void MaxSceneExport::ProcessNode(INode* node, TiXmlNode* xmlNode)
{
	TiXmlNode* entity;
	//Записываем сущность
	entity = new TiXmlElement("entity");
	std::wstring wname(node->GetName());
	std::string name(wname.begin(), wname.end());
	((TiXmlElement*)entity)->SetAttribute("name", name.c_str());
	xmlNode->LinkEndChild(entity);

	if (!node->IsRootNode())
	{
		Object* obj = node->GetObjectRef();
		if (obj == 0)
			return;

		//Трансформ для сущности (все объекты в 3д максе имеют трансформацию, так что это обязательный шаг)
		TiXmlElement* transform = new TiXmlElement("transform");
		float x, y, z, w;
		TimeValue t = ip->GetTime();
		Matrix3 mat3 = node->GetNodeTM(t) * Inverse(node->GetParentNode()->GetNodeTM(t)); //

		TiXmlElement* position = new TiXmlElement("position");
		x = mat3.GetRow(3).x; y = mat3.GetRow(3).z; z = -mat3.GetRow(3).y;
		position->SetDoubleAttribute("x", x);
		position->SetDoubleAttribute("y", y);
		position->SetDoubleAttribute("z", z);
		transform->LinkEndChild(position);

		TiXmlElement* rotation = new TiXmlElement("rotation");
		glm::quat q = matrix43ToQuat(mat3);
		rotation->SetDoubleAttribute("x", q.x);
		rotation->SetDoubleAttribute("y", q.y);
		rotation->SetDoubleAttribute("z", q.z);
		rotation->SetDoubleAttribute("w", q.w);
		transform->LinkEndChild(rotation);

		TiXmlElement* scale = new TiXmlElement("scale");
		vec3 v3 = getScale(mat3);
		scale->SetDoubleAttribute("x", v3.x);
		scale->SetDoubleAttribute("y", v3.y);
		scale->SetDoubleAttribute("z", v3.z);
		transform->LinkEndChild(scale);
		
		entity->LinkEndChild(transform);

		///////////////////////////////////////////////////////
		//Проверяем наличие модификаторов (rigid body например)
		///////////////////////////////////////////////////////
		while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID && obj)
		{
			IDerivedObject* derivedObjectPtr = (IDerivedObject*)(obj);

			int ModStackIndex = 0;
			while (ModStackIndex < derivedObjectPtr->NumModifiers())
			{
				Modifier* modPtr = derivedObjectPtr->GetModifier(ModStackIndex);

				if (modPtr->ClassID() == Class_ID(0x5ba95448, 0x5295c5e5)) //Rigid Body Modifier
				{
					IParamBlock2* pb = GetParamBlock((ReferenceMaker*)modPtr);
					TiXmlElement* rigidBody = new TiXmlElement("rigidbody");
					int type = 0;
					pb->GetValue(pb_shape, 0, type, FOREVER);
					rigidBody->SetAttribute("type", type);
					float fval = 0;
					if (type <= 4)
					{
						pb->GetValue(pb_x, 0, fval, FOREVER);
						rigidBody->SetAttribute("x", fval);
					}
					if (type <= 4 && type > 0)
					{
						pb->GetValue(pb_y, 0, fval, FOREVER);
						rigidBody->SetAttribute("y", fval);
					}
					if (type == 1)
					{
						pb->GetValue(pb_z, 0, fval, FOREVER);
						rigidBody->SetAttribute("z", fval);
					}
					pb->GetValue(pb_mass, 0, fval, FOREVER);
					rigidBody->SetAttribute("mass", fval);
					pb->GetValue(pb_fric, 0, fval, FOREVER);
					rigidBody->SetAttribute("friction", fval);

					//TODO Check if the type is TriMesh or ConvexHull
					//TODO Save mesh or hull
					rigidBody->SetAttribute("mesh", "TODO");


					entity->LinkEndChild(rigidBody);
				}

				ModStackIndex++;
			}
			obj = derivedObjectPtr->GetObjRef();
		}

		////////////////////////////////////////
		//Теперь узнаем что это за объект вообще
		////////////////////////////////////////
		TiXmlElement* elem;
		if (obj->SuperClassID() == CAMERA_CLASS_ID)
		{
			TiXmlElement* elem = new TiXmlElement("camera");
			entity->LinkEndChild(elem);
		}
		else if (obj->SuperClassID() == LIGHT_CLASS_ID)
		{
			TiXmlElement* elem = new TiXmlElement("light");
			elem->SetAttribute("type", "omni");
			entity->LinkEndChild(elem);
		}
		else 
		{
			obj = node->EvalWorldState(0).obj;
			if (obj->SuperClassID() == GEOMOBJECT_CLASS_ID) //Mesh Object
			{
				TiXmlElement* elem = new TiXmlElement("mesh");
				elem->SetAttribute("file", std::string(name + ".xyz").c_str());
				entity->LinkEndChild(elem);
				//Материалы
				Mtl* mtl = node->GetMtl();
				if (mtl)
				{
					_fillMaterials(mtl, elem);
				}
				//Теперь нужно сохранить меш в отдельный файл
				#pragma message(TODO("Save this mesh to separate .xyz file"))
				MeshIO::MeshIO* meshfile = new MeshIO::MeshIO();
				_exportMeshNode(node, meshfile);
				meshfile->Write(std::wstring(L"test.xyz"), MeshIO::MSH_BIN);
			}
		}
	}

	for (int i = 0; i < node->NumberOfChildren(); i++)
	{
		ProcessNode(node->GetChildNode(i), entity);
	}
}

TriObject* MaxSceneExport::_getTriObjFromNode(INode* node, BOOL &deleteIt)
{
	if (node->IsObjectHidden())
		return 0;
	deleteIt = FALSE;
	Object *obj;
	//obj = node->GetObjectRef();
	obj = node->EvalWorldState(0).obj;

	if (obj->ClassID() == BONE_OBJ_CLASSID)
		return 0;
	//file << "OBJCLASS: " << std::to_wstring(obj->ClassID().PartA()) << " " << std::to_wstring(obj->ClassID().PartB()) << "\n";

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		Object *tri = obj->ConvertToType(0,
			Class_ID(TRIOBJ_CLASS_ID, 0));

		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return (TriObject*)tri;
	}
	else
	{
		return NULL;
	}
}

void MaxSceneExport::_exportMeshNode(INode* node, MeshIO::MeshIO* meshfile)
{
	if (node->IsRootNode())
		return;

	BOOL deleteIt = false;

	TriObject* TObj;
	TObj = _getTriObjFromNode(node, deleteIt);




	if (deleteIt)
		delete TObj;
}

void MaxSceneExport::_fillMaterials(Mtl* mtl, TiXmlElement* e)
{
	TiXmlElement* m = new TiXmlElement("material");
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		StdMat* stdmat = (StdMat*)mtl;
		bool twosided = stdmat->GetTwoSided();

		Texmap* tmap = stdmat->GetSubTexmap(ID_DI);
		if (tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex* bmt = (BitmapTex*)tmap;
			std::wstring wname(bmt->GetMapName());
			std::string name(wname.begin(), wname.end());
			TiXmlElement* tex = new TiXmlElement("texture");
			tex->SetAttribute("file", name.c_str() + name.rfind("\\") + 1);
			m->LinkEndChild(tex);

			//Copy texture to a new location
			std::ifstream src(name, std::ios::binary);
			std::string dstname = targetFileName;
			dstname.erase(targetFileName.rfind("\\") + 1);
			dstname += "textures\\";
			dstname += std::string(name.c_str() + name.rfind("\\") + 1);

			std::ifstream checkdst(dstname);
			if (!checkdst.good())
			{
				std::ofstream dst(dstname, std::ios::binary);
				dst << src.rdbuf();
				dst.close();
			}
			src.close();
			checkdst.close();
		}

		#pragma message(TODO("Сохранить материал в ассоциативный массив (видимо по имени), чтобы позже каждый в массиве сохранить в отдельный .mat файл"))
	}
	else //Это, вероятно, мультиматериал, нужно пройтись по его детям
	{
		for (int i = 0; i < mtl->NumSubMtls(); i++)
		{
			_fillMaterials(mtl->GetSubMtl(i), m);
		}
	}
	e->LinkEndChild(m);
	
}

void MaxSceneExport::WriteSceneGraphToFile(std::string name)
{
	doc.SaveFile(name.c_str());
}