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

#include "MaxExportPlugin.h"

#define MaxExportPlugin_CLASS_ID	Class_ID(0xba20357e, 0x5d6a85f9)

class MaxExportPlugin : public SceneExport {
public:
	//Constructor/Destructor
	MaxExportPlugin();
	~MaxExportPlugin();

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

private:
	std::wofstream file;
	Interface* IP;
	MeshIO::MeshIO* meshFile;

	std::vector<TriObject*> triObjects;
	std::vector<INode*> nodeList;
	std::vector<bool> deleteTriObj;

	void ProcessTriObjects();
	void ProcessNode(INode* node, int depth);
	std::vector<indexed_vertex> GetIndexedVerts(Mesh* mesh, bool& hasTex, bool& hasCol);
	std::vector<unsigned int> InitIndices(unsigned int sz);
	TriObject* GetTriObjFromNode(INode* node, BOOL &deleteIt);
	ISkin* FindSkinModifier(INode* node);
};



class MaxExportPluginClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new MaxExportPlugin(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxExportPlugin_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MaxExportPlugin"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetMaxExportPluginDesc() { 
	static MaxExportPluginClassDesc MaxExportPluginDesc;
	return &MaxExportPluginDesc; 
}





INT_PTR CALLBACK MaxExportPluginOptionsDlgProc(HWND hWnd,UINT message,WPARAM,LPARAM lParam) {
	static MaxExportPlugin* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MaxExportPlugin *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- MaxExportPlugin -------------------------------------------------------
MaxExportPlugin::MaxExportPlugin()
{

}

MaxExportPlugin::~MaxExportPlugin() 
{

}

int MaxExportPlugin::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *MaxExportPlugin::Ext(int /*i*/)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("XYZ");
}

const TCHAR *MaxExportPlugin::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("Comet mesh format");
}
	
const TCHAR *MaxExportPlugin::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Comet mesh file");
}

const TCHAR *MaxExportPlugin::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("dude");
}

const TCHAR *MaxExportPlugin::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("fuck you");
}

const TCHAR *MaxExportPlugin::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *MaxExportPlugin::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int MaxExportPlugin::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void MaxExportPlugin::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

BOOL MaxExportPlugin::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}


int	MaxExportPlugin::DoExport(const TCHAR* name, ExpInterface* /*ei*/, Interface* ip, BOOL suppressPrompts, DWORD /*options*/)
{
	#pragma message(TODO("Implement the actual file Export here and"))
	IP = ip;

	if(!suppressPrompts)
		DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PANEL), GetActiveWindow(), MaxExportPluginOptionsDlgProc, (LPARAM)this);

	meshFile = new MeshIO::MeshIO();

	//Пишем в файл
	//file.open(name, std::ios::out | std::ios::binary);

	INode* rootNode = ip->GetRootNode();
	MessageBox(0, L"Processing nodes", 0, 0);
	ProcessNode(rootNode, 0);
	MessageBox(0, L"Processing TriObjects", 0, 0);
	ProcessTriObjects();
	
	MessageBox(0, L"Writing", 0, 0);
	//file.close();
	meshFile->SwapYZ();
	meshFile->Write(std::wstring(name), MeshIO::MSH_BIN);
	delete meshFile;
	
	#pragma message(TODO("return TRUE If the file is exported properly"))
	return TRUE;
}

void MaxExportPlugin::ProcessTriObjects()
{
	for (int i = 0; i < triObjects.size(); i++)
	{
		MessageBox(0, L"TriObj", 0, 0);
		TriObject* TObj = triObjects[i];
		INode* node = nodeList[i];

		std::vector<indexed_vertex> indOut;		//Финальный набор вершин (индексированых)
		MessageBox(0, L"Here?", 0, 0);
		bool hasColor = false, hasTex = false;
		std::vector<indexed_vertex> indVerts = GetIndexedVerts(&(TObj->mesh), hasTex, hasColor);
		MessageBox(0, L"or here?", 0, 0);
		std::vector<unsigned int> indices = InitIndices(indVerts.size());
		
		MessageBox(0, L"Marking vert duplicates", 0, 0);

		//Отмечаем дупликаты вершин
		std::vector<bool> vertDupes;
		vertDupes.resize(indVerts.size());
		std::fill(vertDupes.begin(), vertDupes.end(), false);
		for (int i = 0; i < indVerts.size(); i++)
		{
			if (!vertDupes[i])
			{
				for (int j = i + 1; j < indVerts.size(); j++)
				{
					if (indVerts[i].pos == indVerts[j].pos
						&& indVerts[i].col == indVerts[j].col
						&& indVerts[i].tex == indVerts[j].tex)
					{
						vertDupes[j] = true;
						indices[j] = i;
					}
				}
			}
		}
		
		std::vector<unsigned int> newVertPos;
		newVertPos.resize(indVerts.size());

		for (int i = 0; i < indVerts.size(); i++)
		{
			if (!vertDupes[i])
			{
				newVertPos[i] = indOut.size();
				indOut.push_back(indVerts[i]);
			}
		}
		
		for (int i = 0; i < indices.size(); i++)
		{
			indices[i] = newVertPos[indices[i]];
		}

		MeshIO::Mesh mesh;
		std::wstring wname(node->GetName());
		std::string cname(wname.begin(), wname.end());
		mesh.name = cname;

		MessageBox(0, L"Filling vertices", 0, 0);

		//Вертексы
		for (int i = 0; i < indOut.size(); i++)
		{
			Point3 v = TObj->mesh.verts[indOut[i].pos];
			mesh.verts.insert(mesh.verts.end(), MeshIO::vec3(v.x, v.y, v.z));
		}

		MessageBox(0, L"Filling indices", 0, 0);

		//Индексы
		for (int i = 0; i < indices.size(); i++)
		{
			mesh.faces.insert(mesh.faces.end(), indices[i]);
		}

		//Цвет
		if (hasColor)
		{
			MessageBox(0, L"Filling color", 0, 0);
			for (int i = 0; i < indOut.size(); i++)
			{
				Point3 v = TObj->mesh.vertCol[indOut[i].col];
				mesh.color.insert(mesh.color.end(), MeshIO::vec3(v.x, v.y, v.z));
			}
		}

		//Текстурные координаты
		if (hasTex)
		{
			MessageBox(0, L"Filling uvws", 0, 0);
			mesh.uvw.insert(mesh.uvw.end(), std::vector<MeshIO::vec3>());
			for (int i = 0; i < indOut.size(); i++)
			{
				Point3 v = TObj->mesh.tVerts[indOut[i].tex];
				mesh.uvw[0].insert(mesh.uvw[0].end(), MeshIO::vec3(v.x, v.y, v.z));
			}
		}
		meshFile->meshes.insert(meshFile->meshes.end(), mesh);
	}

	for (int i = 0; i < triObjects.size(); i++)
	{
		if (deleteTriObj[i])
			triObjects[i]->DeleteMe();
	}
}

void MaxExportPlugin::ProcessNode(INode* node, int depth)
{
	depth++;
	//dududu
	BOOL deleteIt = false;
	TriObject* TObj = 0;
	if (!node->IsRootNode())
		TObj = GetTriObjFromNode(node, deleteIt);

	if (TObj)
	{
		triObjects.push_back(TObj);
		nodeList.push_back(node);
		deleteTriObj.push_back(deleteIt);
	}

	for (int i = 0; i < node->NumberOfChildren(); i++)
	{
		ProcessNode(node->GetChildNode(i), depth);
	}
}

std::vector<unsigned int> MaxExportPlugin::InitIndices(unsigned int sz)
{
	std::vector<unsigned int> indices;
	indices.resize(sz);
	for (int i = 0; i < sz; i++)
	{
		indices[i] = i;
	}
	return indices;
}

std::vector<indexed_vertex> MaxExportPlugin::GetIndexedVerts(Mesh* mesh, bool& hasTex, bool& hasCol)
{
	std::vector<indexed_vertex> indexedVertices;
	indexed_vertex indVert;
	//TODO: Цвета у модели может и не быть, так же как и текстурных координат (вроде сделал)
	hasTex = mesh->numTVerts;
	hasCol = mesh->numCVerts;
	for (int faceId = 0; faceId < mesh->getNumFaces(); faceId++)
	{
					indVert.pos = mesh->faces[faceId].getVert(0);
		if(hasTex) indVert.tex = mesh->tvFace[faceId].getTVert(0);
		if(hasCol) indVert.col = mesh->vcFace[faceId].getTVert(0);
		indexedVertices.push_back(indVert);
					indVert.pos = mesh->faces[faceId].getVert(1);
		if (hasTex) indVert.tex = mesh->tvFace[faceId].getTVert(1);
		if (hasCol) indVert.col = mesh->vcFace[faceId].getTVert(1);
		indexedVertices.push_back(indVert);
					indVert.pos = mesh->faces[faceId].getVert(2);
		if (hasTex) indVert.tex = mesh->tvFace[faceId].getTVert(2);
		if (hasCol) indVert.col = mesh->vcFace[faceId].getTVert(2);
		indexedVertices.push_back(indVert);
	}

	return indexedVertices;
}

TriObject* MaxExportPlugin::GetTriObjFromNode(INode* node, BOOL &deleteIt)
{
	if (node->IsObjectHidden())
		return 0;
	deleteIt = FALSE;
	Object *obj;
	//obj = node->GetObjectRef();
	obj = node->EvalWorldState(IP->GetTime()).obj;

	if (obj->ClassID() == BONE_OBJ_CLASSID)
		return 0;
	//file << "OBJCLASS: " << std::to_wstring(obj->ClassID().PartA()) << " " << std::to_wstring(obj->ClassID().PartB()) << "\n";

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *)obj->ConvertToType(IP->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));

		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else
	{
		return NULL;
	}
}

ISkin* MaxExportPlugin::FindSkinModifier(INode* node)
{
	Object* object = node->GetObjectRef();

	if (!object)
		return 0;

	while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID && object)
	{
		IDerivedObject* derivedObject = (IDerivedObject*)object;

		int modStackId = 0;
		while (modStackId < derivedObject->NumModifiers())
		{
			Modifier* mod = derivedObject->GetModifier(modStackId);

			//file << "MODIFIER: " << std::to_wstring(mod->ClassID().PartA()) << " " << std::to_wstring(mod->ClassID().PartB()) << "\n";
			if (mod->ClassID() == SKIN_CLASS_ID)
			{
				return (ISkin*)(mod->GetInterface(I_SKIN));
			}
			modStackId++;
		}
		object = derivedObject->GetObjRef();
	}

	return 0;
}