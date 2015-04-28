#pragma once

#include <vector>
#include <string>
#include "Shader.h"
#include "Material.h"

namespace Comet
{
	class Renderer;
	class RenderTarget;
	class PostProcess
	{
	public:
		PostProcess(Material* mat);
		~PostProcess();

		//Set the output render target.
		void SetOutput(RenderTarget* rt);
		//
		RenderTarget* GetOutput(){ return renderTarget; }
		//
		Material* GetMaterial(){ return material; }

		friend Renderer;
	private:
		Material* material;
		RenderTarget* renderTarget;
	};

}