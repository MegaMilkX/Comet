#include "GUISprite.h"
#include "ResMan.h"

namespace Comet
{

	GUISprite::GUISprite(GUIElement* prnt) : GUIRect(prnt)
	{
		name = "GUISprite";

		material->SetShader(ResMan::GetInstance()->GetResource<Shader>("ShaderRectTexturedDefault"));
	}


	GUISprite::~GUISprite()
	{
	}

	void GUISprite::SetTexture(std::string str)
	{
		mesh->SetTexture2D(ResMan::GetInstance()->GetResource<Texture2D>(str), 0);
		material->SetTexture2D(ResMan::GetInstance()->GetResource<Texture2D>(str), 0);
	}
}