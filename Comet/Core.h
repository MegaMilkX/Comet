#pragma once

#include "Renderer.h"
#include "Game.h"

#include "ResMan.h"

#include "UserConsole.h"

namespace Comet
{
	class Game;

	class Core
	{
	public:
		Core();
		~Core();

		static Core* GetInstance(){ return instance; }

		void Init();
		void Reset();
		
		template<class T>
		void SetGame()
		{
			if (game)
				delete game;
			game = new T();
			game->core = this;
			game->Init();
		}

		Renderer* GetRenderer(){ return renderer; }
		Game* GetGame(){ return game; }

		void LoadScene(std::string path);

		friend Game;
	private:
		void _loop();

		static Core* instance;

		Renderer* renderer;
		Game* game;

		UserConsole* userConsole;
	};

};