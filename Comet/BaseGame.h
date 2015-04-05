#pragma once

#include "Core.h"

namespace Comet
{

	class BaseGame : public Core
	{
	public:
		BaseGame();
		virtual ~BaseGame();

		void PostInit();
		bool Update();
	private:
		Viewport* viewport;
		double mx, my;
		double px, py;

		Node* node2;
	};

}