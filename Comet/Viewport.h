#pragma once

namespace Comet
{
	class Camera;
	class Renderer;

	class Viewport
	{
	public:
		Viewport(Renderer* r);
		~Viewport();

		void SetCamera(Camera* cam);
		Camera* GetCamera(){ return camera; }

		float GetWidth(){ return width; }
		float GetHeight(){ return height; }
		float GetLeft(){ return x; }
		float GetBottom(){ return y; }

		void SetRect(float l, float b, float w, float h){ x = l; y = b; width = w; height = h; }
		void SetColor(float r, float g, float b, float a = 1.0f){ this->r = r; this->g = g; this->b = b; this->a = a; }

		void SetRenderMask(unsigned int rm){ renderMask = rm; }
		unsigned int GerRenderMask(){ return renderMask; }

		void SetClearDepth(bool val);
		void SetClearColor(bool val);

		Renderer* GetRenderer(){return renderer;}

		friend Camera;
		friend Renderer;
	private:
		Renderer* renderer;

		Camera* camera;
		float width, height;
		float x, y;

		unsigned int renderMask;
		unsigned int clearFlags;

		float r, g, b, a;
	};

};