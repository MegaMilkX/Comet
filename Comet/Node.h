#pragma once

#include <math3f.h>

#include "Component.h"
#include <set>
#include <map>
#include <vector>
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
						Node(std::string name);
						~Node();

		//TODO: Maybe this doesn't need to be a template method?
		template<typename T>
		T*				NewComponent();
		//But this certainly does
		template<typename T>
		T*				GetComponent() const;
		
		//
		Node*			CreateNode();

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

		unsigned int	GetChildCount() const { return nodes.size(); }
		Node*			GetChild(const unsigned int &id) const { return nodes[id]; }

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
		std::vector<Node*> nodes;
		std::map<std::type_index, Component*> components;

		unsigned int	layer;

		bool			drt;
	};

	template<typename T>
	T* Node::NewComponent()
	{
		T* comp = new T;
		comp->SetNode(this);
		components.insert(std::make_pair(std::type_index(typeid(T)), comp));
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