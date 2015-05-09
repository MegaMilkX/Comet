#include "WorldMapTile.h"
#include "AdventureGame.h"

#include "ResMan.h"

#include <sstream>

WorldMapTile::WorldMapTile(AdventureGame* game, int x, int y)
{
	char* cstr = new char[256];
	sprintf(cstr, "data\\textures\\worldmap\\world_%i_%i.png", x, y);

	//TODO: Дырки между тайлами
	Comet::Node* tileNode = game->GetRenderer()->GetRoot()->CreateNode();
	tileNode->SetPosition(vec3f(x*25.0f, y*25.0f, 0));
	tileNode->Scale(vec3f(25.0f, 25.0f, 0));
	//Comet::Mesh* mesh = new Comet::Mesh(Comet::ResMan::GetInstance()->GetResource<Comet::MeshData>("plane"));
	Comet::Mesh* mesh = new Comet::Mesh(game->GetRenderer()->GetMeshDataPrimitive("plane"));
	tileNode->Attach(mesh);
	
	//Comet::Mesh* mesh2 = new Comet::Mesh(Comet::ResMan::GetInstance()->GetResource<Comet::MeshData>("data\\models\\test2.xyz"));
	//mesh->SetTexture2D(Comet::ResMan::GetInstance()->GetResource<Comet::Texture2D>(ss.str()), 0);

	Comet::Material* mat = new Comet::Material();
	mat->SetShader(Comet::ResMan::GetInstance()->GetResource<Comet::Shader>("data\\shaders\\test.glsl"));
	mat->SetTexture2D(Comet::ResMan::GetInstance()->GetResource<Comet::Texture2D>(std::string(cstr)), 0);
	sprintf(cstr, "MaterialWorldMap%i%i.png", x, y);
	Comet::ResMan::GetInstance()->SetResource(std::string(cstr), mat);
	mesh->SetMaterial(mat);

	tileNode->Attach(mesh);

	delete cstr;
}


WorldMapTile::~WorldMapTile()
{
}
