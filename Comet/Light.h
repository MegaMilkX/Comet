#pragma once

#include "RenderObject.h"

namespace Comet
{
	enum LIGHTTYPE
	{
		OMNI,
		SPOT,
		DIRECTIONAL
	};

	class Light : public RenderObject
	{
	public:
		Light();
		~Light();
	};

};