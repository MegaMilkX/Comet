#include "Sequence.h"

namespace Comet
{

	Sequence::Sequence()
	{
	}


	Sequence::~Sequence()
	{
		Unload();
	}

	void Sequence::Load(std::string path)
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



		printf("Done.\n");
		isReady = true;
	}

	void Sequence::Unload()
	{

	}
}