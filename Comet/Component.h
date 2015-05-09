#pragma once

namespace Comet
{
	class Node;
	class Component
	{
	public:
						Component();
		virtual			~Component();

		virtual void	SetNode(Node* e);
	protected:
		Node*			node;
	};

}