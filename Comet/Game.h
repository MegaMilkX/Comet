#pragma once

#include "Core.h"

namespace Comet
{
	class Core;

	class Game
	{
	public:
		Game();
		virtual ~Game();

		virtual void Init() = 0;
		virtual void Update() = 0;

		friend Core;
	protected:
		Core* core;
	};

};