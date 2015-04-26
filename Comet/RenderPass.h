#pragma once

#include <vector>
#include <string>
#include "Shader.h"
#include "Material.h"

namespace Comet
{
	class Renderer;
	class RenderPass
	{
	public:
		RenderPass(Renderer* r, RenderPass* parent, Material* mat = 0);
		~RenderPass();

		RenderPass* CreatePass();
		Texture2D* CreateImageBuffer(std::string name);
		void SetMaterial(Material* mat);

		friend Renderer;
	private:
		Material* material;
		std::vector<RenderPass*> passes;
		std::vector<Texture2D*> imageBuffers;
		std::map<std::string, unsigned int> imageNames;
		unsigned int frameBuffer;

		RenderPass* parent;
		Renderer* renderer;
	};

}