#include "Viewport.h"
#include "Camera.h"

#include "Core.h"

namespace Comet
{

	Viewport::Viewport(Renderer* r)
	{
		//Core::GetInstance()->GetRenderer()->_regViewport(this);

		camera = 0;

		renderMask = 65535;
		//renderMask = 1;
		clearFlags = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;

		renderer = r;
	}


	Viewport::~Viewport()
	{
		
	}

	void Viewport::SetClearDepth(bool val)
	{
		if (val)
			clearFlags |= GL_DEPTH_BUFFER_BIT;
		else
			clearFlags &= ~GL_DEPTH_BUFFER_BIT;
	}

	void Viewport::SetClearColor(bool val)
	{
		if (val)
			clearFlags |= GL_COLOR_BUFFER_BIT;
		else
			clearFlags &= ~GL_COLOR_BUFFER_BIT;
	}

	void Viewport::SetCamera(Camera* cam)
	{
		//if (camera)
		//	camera->viewport = 0;
		camera = cam; 
	}
};