#include "GUIElement.h"
#include "Core.h"

namespace Comet
{

	GUIElement::GUIElement(GUIElement* prnt)
	{
		name = "GUIElement";

		parent = prnt;

		if (parent)
		{
			node = parent->GetSceneNode()->CreateNode();
			//node = Core::GetInstance()->GetRenderer()->GetRoot()->CreateNode();
			//NOTE: Все элементы интерфейса сейчас крепятся на глобальные ноды (из-за использования минивьюпортов глобальные трансформы рендеру не нужны)
			//NOTE: Вернул как было
		}
		else
		{
			node = Core::GetInstance()->GetRenderer()->GetRoot()->CreateNode();
		}

		node->SetLayer(1);

		zindex = 0;

		alignment = LEFT | BOTTOM;
	}


	GUIElement::~GUIElement()
	{
		std::set<GUIElement*>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
		{
			delete (*it);
		}
	}

	void GUIElement::Update(GUICursor* cur)
	{
		if (IsInside(cur))
			cur->focusElement = this;

		std::set<GUIElement*>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
		{
			(*it)->Update(cur);
		}
	}

	void GUIElement::SetZIndex(int zi)
	{
		zindex = zi;
	}

	void GUIElement::SetAlign(int al)
	{
		if (!parent)
			return;

		alignment = al;

		float x;
		float y;

		/*if (al & HCENTER == HCENTER)
			x = parent->rect.z / 2 - rect.z / 2;
		else*/ if (al & LEFT)
			x = rect.x;
		else if (al & RIGHT)
			x = parent->rect.z - rect.x - rect.z;

		/*if (al & VCENTER == VCENTER)
			y = parent->rect.w / 2 - rect.w / 2;
		else*/ if (al & BOTTOM)
			y = rect.y;
		else if (al & TOP)
			y = parent->rect.w - rect.y - rect.w;

		

		node->SetPosition(vec3f(x, y, 0));
	}

	void GUIElement::SetRect(float x, float y, float w, float h)
	{
		rect.x = x;
		rect.y = y;
		rect.z = w;
		rect.w = h;

		node->SetPosition(vec3f(x, y, 0));

		std::set<GUIElement*>::iterator it;
		for (it = elements.begin(); it != elements.end(); it++)
		{
			(*it)->SetVPRect(x / 1280.0f, y / 720.0f, w / 1280.0f, h / 720.0f);
			(*it)->SetAlign((*it)->alignment);
		}

		SetAlign(alignment);

		//Обновить размеры всех элементов внутри этого

	}

	bool GUIElement::IsInside(GUICursor* cur)
	{
		return (cur->x >= rect.x && cur->y >= rect.y && cur->x <= (rect.z + rect.x) && cur->y <= (rect.w + rect.y));
	}

}