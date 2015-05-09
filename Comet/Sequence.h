#pragma once

#include "Resource.h"

#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>

#include "math3f.h"

namespace Comet
{

	//Animation sequence resource
	class Sequence : public Resource
	{
	public:
		Sequence();
		~Sequence();

		void Load(std::string path);
		void Unload();
	private:
	};

}