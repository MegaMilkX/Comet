#pragma once

namespace Comet
{
	class Entity;
	class Component
	{
	public:
						Component();
		virtual			~Component();

		virtual void	SetEntity(Entity* e);
		//Tell the component that its entity was changed
		virtual void	BadEntity();
	protected:
		Entity*			entity;
		//Indicates if parent entity component setup was changed in any way
		bool			entityBad;
	};

}