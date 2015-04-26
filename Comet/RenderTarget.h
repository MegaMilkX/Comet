#pragma once

#include "Texture2D.h"
#include <string>
#include <vector>
#include <map>

namespace Comet
{

	class RenderTarget
	{
	public:
		RenderTarget();
		RenderTarget(unsigned int w, unsigned int h);
		~RenderTarget();

		Texture2D* CreateImageBuffer(std::string name);
		bool IsOK();

		void Bind();
	private:
		unsigned int framebuffer;
		std::vector<Texture2D*> imageBuffers;
		std::map<std::string, unsigned int> imageNames;

		unsigned int width, height;
	};

}