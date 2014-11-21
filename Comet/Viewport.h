#pragma once

namespace Comet
{
	class Camera;

	class Viewport
	{
	public:
		Viewport();
		~Viewport();

		void SetCamera(Camera* cam){ camera = cam; }
		Camera* GetCamera(){ return camera; }

	private:
		Camera* camera;
	};

};