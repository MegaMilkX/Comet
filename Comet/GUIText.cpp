#include "GUIText.h"
#include "Core.h"

namespace Comet
{

	GUIText::GUIText(GUIElement* prnt) : GUIElement(prnt)
	{
		name = "GUIText";
		renderable = new TextRenderable("gui_text", "data\\fonts\\Magic.ttf", 16);
		node->Attach(renderable);
		renderable->GetMaterial()->SetZTest(false);
		renderable->GetMaterial()->SetZWrite(false);
	}


	GUIText::~GUIText()
	{
	}

	void GUIText::SetText(std::string str)
	{
		text = str;
		//Update text renderable
		renderable->SetText(str);
		rect.z = renderable->GetBoundingRect().z;
		rect.w = renderable->GetBoundingRect().w;

		SetAlign(alignment);
	}

	void GUIText::SetZIndex(int zi)
	{
		GUIElement::SetZIndex(zi);
		renderable->GetMaterial()->SetOrder(10000 + zindex);
	}

}