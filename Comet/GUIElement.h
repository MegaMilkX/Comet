#pragma once

#include "GUICursor.h"

#include <set>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Node.h"

namespace Comet
{
	enum GUIAlign
	{
		TOP = 1,		//0001
		BOTTOM = 2,		//0010
		LEFT = 4,		//0100
		RIGHT = 8,		//1000
		CENTER = 15,	//1111
		HCENTER = 3,	//0011
		VCENTER = 12	//1100
	};

	class GUIElement
	{
	public:
		GUIElement(GUIElement* prnt);
		~GUIElement();

		void Update(GUICursor* cur);

		template<typename T>
		T* CreateElement()
		{
			T* elem = new T(this);
			elements.insert(elem);
			((GUIElement*)elem)->SetZIndex(zindex + 1);
			((GUIElement*)elem)->SetVPRect(rect.x / 1280.0f, rect.y / 720.0f, rect.z / 1280.0f, rect.w / 720.0f);
			return elem;
		}

		virtual void SetVPRect(float l, float b, float r, float t){}
		virtual void SetZIndex(int zi);
		virtual void SetRect(float x, float y, float w, float h);
		virtual void SetAlign(int al);
		virtual void SetColor(float r, float g, float b, float a)
		{color.r = r; color.g = g; color.b = b; color.a = a;}
		void SetName(std::string str){ name = str; }
		glm::vec4 GetColor(){ return color; }
		glm::vec4 GetRect(){ return rect; }
		std::string GetName(){ return name; }
		Node* GetSceneNode(){ return node; }
	protected:
		bool IsInside(GUICursor* cur);

		int zindex;

		GUIElement* parent;
		std::set<GUIElement*> elements;
		glm::vec4 color;
		glm::vec4 rect;
		int alignment;

		std::string name;

		Node* node;
	};

}