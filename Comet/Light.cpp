#include "Light.h"

#include "Core.h"

namespace Comet
{

	Light::Light()
	{
		Core::GetInstance()->GetRenderer()->_regLight(this);
	}


	Light::~Light()
	{
	}

};