#pragma once

#include "Resource.h"
#include "Material.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <gl/glew.h>
#endif
#include <GLFW/glfw3.h>

namespace Comet
{

	class Shader : public Resource
	{
	public:
		enum ATTR { POS, UVW, NRM, COL, M, V, P };

						Shader(bool make_default = false);
		virtual			~Shader();

		const Shader&	operator=(const Shader& other);

		void			Load(std::string path);
		//
		void			Load(std::string fp, std::string vp);
		void			MakeDefault()
		{

		}
		void			Unload();

		unsigned int	GetProgramId() const { return shaderProgramId; }
		int				GetAttribLocation(ATTR attr) const { return attrloc[attr]; }

	private:
		unsigned int	shaderProgramId;
		int				attrloc[7];	//Attribute locations
	};

}