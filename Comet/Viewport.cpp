#include "Viewport.h"
#include "Camera.h"

#include "Core.h"

namespace Comet
{

	Viewport::Viewport()
	{
		Core::GetInstance()->GetRenderer()->_regViewport(this);

		camera = 0;
	}


	Viewport::~Viewport()
	{
	}

};