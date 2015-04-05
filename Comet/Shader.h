#pragma once

#include "Resource.h"
#include "Material.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Comet
{

	class Shader : public Resource
	{
	public:
		Shader();
		virtual ~Shader();

		void Load(std::string path);
		//
		void Load(std::string fp, std::string vp);
		void Unload();

		unsigned int GetProgramId(){ return shaderProgramId; }

	private:
		unsigned int shaderProgramId;
	};

}