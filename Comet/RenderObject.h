#pragma once

namespace Comet
{
	class Node;

	class RenderObject
	{
	public:
		RenderObject();
		virtual ~RenderObject();

		Node* GetNode(){return node;}

		friend Node;
	protected:
		Node* node;
	};

};