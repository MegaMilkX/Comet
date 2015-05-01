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
	protected:
		Entity*			entity;
	};

}