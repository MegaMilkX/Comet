#include "PostProcess.h"
#include "ResMan.h"
#include "Renderer.h"
#include "Texture2D.h"

namespace Comet
{

	PostProcess::PostProcess(Material* mat)
	{
		material = mat;
	}


	PostProcess::~PostProcess()
	{
	
	}

	void PostProcess::SetOutput(RenderTarget* rt)
	{
		renderTarget = rt;
	}
}