#pragma once

#include <string>

namespace Comet
{

	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		virtual void Load(std::string path) = 0;
		virtual void Unload() = 0;
	private:

	};

};