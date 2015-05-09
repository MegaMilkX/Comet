#pragma once

#include <math3f.h>

#include <set>
#include <map>

#include "Component.h"

namespace Comet
{
	class RenderObject;
	class Renderable;
	class Renderer;

	class Node : public Component
	{
	public:
		enum TSPACE { LOCAL, PARENT, WORLD };
						Node();
						~Node();

		//Overriden
		void			SetEntity(Entity* e);
		
		//
		Node*			CreateNode();
		void			Attach(Node* n);
		void			Attach(RenderObject* obj);
		void			DestroyNode(Node* n);
		void			Detach(Node* n);
		void			Detach(RenderObject* obj);
		void			SetParent(Node* n);

		void			SetPosition(vec3f pos);
		void			SetRotation(quat rot);
		void			Translate(vec3f vec, TSPACE space = WORLD);
		void			Rotate(float angle, vec3f vec, TSPACE space = LOCAL);
		void			Scale(vec3f vec);

		vec3f			GetPosition();
		vec3f			GetLocalPos();
		quat			GetRotation(){ return rotation; }
		vec3f			GetScale(){ return scale; }
		mat4f			GetTransform();
		mat4f			GetLocalTransform();
		vec3f			GetUp();
		vec3f			GetBack();
		vec3f			GetRight();

		std::string		GetName(){ return name; }
		void			SetName(std::string n){ name = n; }

		void			SetLayer(unsigned int l){ layer = l; }
		unsigned int	GetLayer(){ return layer; }

		std::set<Node*> GetNodes(){ return nodes; }

		void			dirty();
		bool			IsDirty();

		friend Renderer;
	private:
		vec3f			position;
		quat			rotation;
		vec3f			rotEuler;
		vec3f			scale;
		mat4f			transform;
		std::string		name;

		Node*			parent;
		std::set<Node*> nodes;
		std::set<RenderObject*> objects;

		unsigned int	layer;

		bool			drt;
	};

};