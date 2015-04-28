#pragma once

#include "Texture2D.h"
#include "PostProcess.h"
#include <string>
#include <vector>
#include <map>

namespace Comet
{
	class Camera;

	//Serves as canvas for rendering by world cameras or a post-process. May contain more than one color buffer. (Basically a wrapper over an FBO)
	class RenderTarget
	{
	public:
		RenderTarget();
		RenderTarget(unsigned int w, unsigned int h);
		~RenderTarget();

		//Add an image buffer
		Texture2D* CreateImageBuffer(std::string name, Texture2D::TEXFMT fmt = Texture2D::RGBA);
		//Create and apply a post-effect
		PostProcess* CreatePostProcess();
		PostProcess* CreatePostProcess(Material* mat);
		//Get root post-process
		PostProcess* GetPostProcess(){ return postProcess; }
		//Get custom image buffer that was added earlier
		Texture2D* GetImageBuffer(std::string name);
		//Get depth buffer texture
		Texture2D* GetDepthBuffer(){ return depthBuffer; }
		//Check if this render target is usable
		bool IsOK();
		//Resize render target
		void Resize(unsigned int w, unsigned int h);
		//Render target width
		unsigned int Width(){ return width; }
		//Render target height
		unsigned int Height(){ return height; }

		//Use only for rendering
		void Bind();

		friend Camera;
	private:
		unsigned int framebuffer;
		std::vector<Texture2D*> imageBuffers;
		std::vector<unsigned int> drawBuffers;
		Texture2D* depthBuffer;
		std::map<std::string, unsigned int> imageNames;

		unsigned int width, height;

		PostProcess* postProcess;

		Camera* camera;
	};

}