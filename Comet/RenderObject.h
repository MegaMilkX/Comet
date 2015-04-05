#pragma once

#include <string>

#include "Component.h"

namespace Comet
{
	class Node;

	class RenderObject : public Component
	{
	public:
		RenderObject();
		virtual ~RenderObject();

		void SetName(std::string n){ name = n; }

		Node* GetNode(){return node;}
		std::string GetName(){ return name; }

		virtual bool IsRenderable(){ return false; }

		virtual void Update(){}

		//
		void SetEntity(Entity* e);

		friend Node;
	protected:
		Node* node;
		std::string name;
	};

};