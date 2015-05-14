#include "Animation.h"

#include <iostream>
#include "math3f.h"

namespace Comet
{

	Animation::Animation() : length(100)
	{
	}


	Animation::~Animation()
	{
		for (unsigned int i = 0; i < curves.size(); ++i)
			delete curves[i];
		Unload();
	}

	void Animation::_GetKeys(FbxAnimCurveNode* curveNode, Curve* dst, int n, bool toRadians)
	{
		FbxAnimCurve* curve;
		int nKeys = 0;
		FbxAnimCurveKey key;
		float val = 0;

		for (int j = 0; j < n; ++j)
		{
			curve = curveNode->GetCurve(j);
			if (curve)
			{
				nKeys = curve->KeyGetCount();
				for (int i = 0; i < nKeys; ++i)
				{
					key = curve->KeyGet(i);
					val = key.GetValue();
					if (toRadians)
						val = val * (PI / 180.0f);
					dst->GetCurve(j)->SetKey((int)key.GetTime().GetFrameCount(), val);
				}
			}
		}

		curves.push_back(dst);
	}

	void Animation::_ProcessNode(FbxNode* node, FbxAnimLayer* layer)
	{
		Curve* dstCrv = 0;
		FbxAnimCurveNode* curveNode = node->LclTranslation.GetCurveNode(layer);
		if (curveNode)
		{
			dstCrv = new TranslationCurve();
			_GetKeys(curveNode, dstCrv, 3);
		}

		curveNode = node->LclRotation.GetCurveNode(layer);
		if (curveNode)
		{
			dstCrv = new RotationCurve();
			_GetKeys(curveNode, dstCrv, 3, true);
		}

		curveNode = node->LclScaling.GetCurveNode(layer);
		if (curveNode)
		{
			dstCrv = new ScaleCurve();
			_GetKeys(curveNode, dstCrv, 3);
		}

		int nChildren = node->GetChildCount();
		for (int i = 0; i < nChildren; ++i)
			_ProcessNode(node->GetChild(i), layer);
	}

	void Animation::Load(std::string path)
	{
		printf("Loading animation sequence \"%s\": ", path.c_str());
		FbxManager* sdkManager = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
		sdkManager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(sdkManager, "");

		bool importOk = importer->Initialize(path.c_str(), -1, sdkManager->GetIOSettings());

		if (!importOk)
		{
			printf("FbxImporter::Initialize() failed.\n");
			printf("Error desc: %s\n\n", importer->GetStatus().GetErrorString());
			return;
		}
		FbxScene* scene = FbxScene::Create(sdkManager, "scene");
		importer->Import(scene);
		importer->Destroy();

		FbxAxisSystem oglSys;
		oglSys.OpenGL.ConvertScene(scene);

		//One stack is basically one animation track
		int nStacks = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int stackId = 0; stackId < nStacks; ++stackId)
		{
			FbxAnimStack* stack = scene->GetSrcObject<FbxAnimStack>(stackId);
			//Getting the animation sequence length
			SetLength((int)stack->LocalStop.Get().GetFrameCount());
			std::cout << stack->GetName() << std::endl;
			
			int nLayers = stack->GetMemberCount<FbxAnimLayer>();
			for (int layerId = 0; layerId < nLayers; ++layerId)
			{
				FbxAnimLayer* layer = stack->GetMember<FbxAnimLayer>(layerId);
				std::cout << layer->GetName() << std::endl;

				FbxNode* node = scene->GetRootNode();
				_ProcessNode(node, layer);
			}
		}

		printf("Done.\n");
		isReady = true;
	}

	void Animation::Unload()
	{

	}
}