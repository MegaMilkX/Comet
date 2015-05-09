#include "Node.h"
#include "RenderObject.h"

#include <sstream>

namespace Comet
{

	RenderObject::RenderObject()
	{
		node = 0;
		std::stringstream ss;
		ss.clear();
		ss << "object_" << this;

		name = ss.str();
	}


	RenderObject::~RenderObject()
	{
	}

};