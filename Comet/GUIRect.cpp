#include "GUIRect.h"
#include "Core.h"

namespace Comet
{

	GUIRect::GUIRect(GUIElement* prnt) : GUIElement(prnt)
	{
		name = "GUIRect";

		mesh = new Mesh(ResMan::GetInstance()->GetResource<MeshData>("MeshRectDefault"));
		mesh->SetName("MeshRectDefault");
		node->Attach(mesh);
		
		material = new Material();
		material->SetZTest(false);
		material->SetZWrite(false);
		material->SetShader(ResMan::GetInstance()->GetResource<Shader>("ShaderRectResizableDefault"));

		mesh->SetMaterial(material);

		material->SetParameter("ScreenWidth", (float)(Core::GetInstance()->GetRenderer()->GetWindowWidth()));
		material->SetParameter("ScreenHeight", (float)(Core::GetInstance()->GetRenderer()->GetWindowHeight()));
	}


	GUIRect::~GUIRect()
	{
		node->Detach(mesh);
		//Уничтожить меш и нод (не через делит, рендерер сам должен их удалить, но об этом ему нужно сообщить)
		//Пока мне лень

		delete material;
	}

	void GUIRect::SetAlign(int al)
	{
		GUIElement::SetAlign(al);


	}

	void GUIRect::SetVPRect(float l, float b, float r, float t)
	{
		material->SetSubViewportRect(l, b, r, t);
	}

	void GUIRect::SetZIndex(int zi)
	{
		GUIElement::SetZIndex(zi);

		material->SetOrder(10000 + zindex);
	}

	void GUIRect::SetRect(float x, float y, float w, float h)
	{
		GUIElement::SetRect(x, y, w, h);
		//node->Scale(glm::vec3(w, h, 1.0f));
		material->SetParameter("rect", glm::vec4(x, y, w, h));

		
	}

	void GUIRect::SetColor(float r, float g, float b, float a)
	{
		GUIElement::SetColor(r, g, b, a);
		material->SetParameter("_color", color);
	}
}