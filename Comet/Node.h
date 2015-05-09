#pragma once

#include <math3f.h>

#include "Component.h"
#include <set>
#include <map>
#include <typeindex>

namespace Comet
{
	class RenderObject;
	class Renderable;
	class Renderer;

	class Node
	{
	public:
		enum TSPACE { LOCAL, PARENT, WORLD };
						Node();
						~Node();

		//TODO: Maybe this doesn't need to be a template method
		template<typename T>
		T*				AddComponent(T* comp);
		//But this certainly does
		template<typename T>
		T*				GetComponent() const;
		
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
		std::set<RenderObject*> objects; //DELETE THIS
		std::map<std::type_index, Component*> components;

		unsigned int	layer;

		bool			drt;
	};

	//TODO: Maybe this doesn't need to be template method
	template<typename T>
	T* Node::AddComponent(T* comp)
	{
		//TODO What do if a component of this type already exists?

		if (dynamic_cast<Component*>(comp) == 0) //Not a component, abort
		{
			printf("That's not a component you're trying to add\n");
			return 0;
		}

		//TODO: Instead of type_index should use some sort of COMPID
		//Like comp->GetID(); So all Renderable components will be of one type
		//therefore it wont be possible to add multiple renderables to one entity.
		//Or use dynamic_cast<Renderable*>

		components.insert(std::make_pair(std::type_index(typeid(T)), comp));
		comp->SetNode(this);

		return comp;
	}

	template<typename T>
	T* Node::GetComponent() const
	{
		std::map<std::type_index, Component*>::const_iterator it
			= components.find(std::type_index(typeid(T)));
		if (it != components.end())
			return (T*)(it->second);
		else
			return 0;
	}
};