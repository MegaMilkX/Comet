#pragma once

#include "Component.h"
#include <set>
#include <map>
#include <typeindex>

namespace Comet
{
	class Node;
	class Renderable;
	class RigidBody;
	class Entity
	{
	public:
		Entity();
		~Entity();

		template<typename T>
		void AddComponent(T* comp)
		{
			//TODO What do if a component of this type already exists?
			components.insert(std::make_pair(std::type_index(typeid(T)), comp));
			comp->SetEntity(this); 
		}

		template<typename T>
		T* GetComponent()
		{
			std::map<std::type_index, Component*>::iterator it;
			it = components.find(std::type_index(typeid(T)));
			if (it != components.end())
				return (T*)(it->second);
			else
				return 0;
		}

		Node* GetTransform();
		Renderable* GetRenderable();
		RigidBody* GetRigidBody();

		void SetTransform(Node* n){ transform = n; }
		void SetRenderable(Renderable* r){ renderable = r; }
		void SetRigidBody(RigidBody* rb){ rigidBody = rb; }
	private:
		//std::set<Component*> components;
		std::map<std::type_index, Component*> components;
		Node* transform;
		RigidBody* rigidBody;
		Renderable* renderable;
	};

}