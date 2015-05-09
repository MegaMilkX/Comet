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

		//TODO: Maybe this doesn't need to be a template method
		template<typename T>
		T*				AddComponent(T* comp);
		//But this certainly does
		template<typename T>
		T*				GetComponent() const;

		//TODO: Change these when components change
		Node*			GetTransform() const { return transform; }
		Renderable*		GetRenderable() const { return renderable; }
		RigidBody*		GetRigidBody() const { return rigidBody; }

		void			SetTransform(Node* n);
		void			SetRenderable(Renderable* r);
		void			SetRigidBody(RigidBody* rb);

		//# Don't use it
		void			_setTransform(Node* n){ transform = n; }
		//# Don't use it
		void			_setRenderable(Renderable* r){ renderable = r; }
		//# Don't use it
		void			_setRigidBody(RigidBody* rb){ rigidBody = rb; }
	private:
		std::map<std::type_index, Component*> components;
		Node*			transform;
		RigidBody*		rigidBody;
		Renderable*		renderable;
	};

	//TODO: Maybe this doesn't need to be template method
	template<typename T>
	T* Entity::AddComponent(T* comp)
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
		comp->SetEntity(this);

		std::map<std::type_index, Component*>::const_iterator it;
		for (it = components.begin(); it != components.end(); ++it)
		{it->second->BadEntity();}

		return comp;
	}

	template<typename T>
	T* Entity::GetComponent() const
	{
		std::map<std::type_index, Component*>::const_iterator it
			= components.find(std::type_index(typeid(T)));
		if (it != components.end())
			return (T*)(it->second);
		else
			return 0;
	}
}