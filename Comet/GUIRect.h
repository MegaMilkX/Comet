#pragma once
#include "GUIElement.h"
#include "Mesh.h"
#include "Node.h"

namespace Comet
{

	class GUIRect :
		public GUIElement
	{
	public:
		GUIRect(GUIElement* prnt);
		~GUIRect();

		void SetAlign(int al);
		void SetVPRect(float l, float b, float r, float t);
		void SetZIndex(int zi);
		void SetRect(float x, float y, float w, float h);
		void SetColor(float r, float g, float b, float a);
	protected:
		Mesh* mesh;
		Material* material;
	};

}