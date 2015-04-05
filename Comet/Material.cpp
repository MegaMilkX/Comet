//
//
//	TODO: Сделать загрузку материала из файла (имя шейдера, текстуры, параметры)
//
//

#include "ResMan.h"

#include "Material.h"
#include "Renderer.h"

#include "Shader.h"

#include <iostream>
#include <sstream>

namespace Comet
{

	Material::Material()
	{
		shaderProgram = 0;
		zwrite = true;
		ztest = true;
		order = 0;

		subViewportRect = glm::vec4(0, 0, 0, 0);

		ignoreParentPos = false;
		ignoreParentScale = false;
	}

	Material::Material(std::string shader, std::string texture0) : Material()
	{
		SetShader(shader);
		SetTexture2D(texture0, 0);
	}


	Material::~Material()
	{
		Unload();
	}

	void Material::Load(std::string path)
	{
		std::string MatFileString;
		std::ifstream fileStream(path, std::ios::in);
		if (fileStream.is_open())
		{
			std::string Line = "";
			while (getline(fileStream, Line))
				MatFileString += "\n" + Line;
			fileStream.close();
		}
		else
		{
			printf("[ER] Impossible to open material \"%s\".\n", path);
			getchar();
			return;
		}
		/*
		std::istringstream iss(MatFileString);
		std::string token;
		while (std::getline(iss, token, '\n'))
		{
			std::cout << token << std::endl;
			token.erase(std::remove_if(token.begin(), token.end(), ' '), token.end());
			token.erase(std::remove_if(token.begin(), token.end(), '\n'), token.end());
			std::cout << token << std::endl;
		}
		*/
		//Shader* s = ResMan::GetInstance()->GetResource<Shader>("data\\shaders\\test.glsl");
		//shaderProgramId = s->shaderProgramId;

		isReady = true;
	}

	void Material::Unload()
	{

	}

	void Material::SetTexture2D(std::string resname, unsigned char layer)
	{
		SetTexture2D(ResMan::GetInstance()->GetResource<Texture2D>(resname), layer);
	}

	void Material::SetTexture2D(Texture2D* tex, unsigned char layer)
	{
		if (textures.size() >= layer)
		{
			textures.resize(layer + 1);
		}

		textures[layer] = tex;
	}

	void Material::SetShader(std::string resname)
	{
		SetShader(ResMan::GetInstance()->GetResource<Shader>(resname));
	}

	void Material::SetShader(Shader* shad)
	{
		shaderProgram = shad;
	}

	void Material::glSetUniforms()
	{
		std::map<std::string, Param>::iterator it;
		for (it = parameters.begin(); it != parameters.end(); it++)
		{
			if (it->second.typeId == typeid(int).hash_code())
			{
				glUniform1i(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), *(int*)(it->second.val));
			}
			else if (it->second.typeId == typeid(float).hash_code())
			{
				glUniform1f(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), *(float*)(it->second.val));
			}
			else if (it->second.typeId == typeid(glm::vec3).hash_code())
			{
				glUniform3f(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), (*(glm::vec3*)(it->second.val)).x, (*(glm::vec3*)(it->second.val)).y, (*(glm::vec3*)(it->second.val)).z);
			}
			else if (it->second.typeId == typeid(glm::vec4).hash_code())
			{
				glUniform4f(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), (*(glm::vec4*)(it->second.val)).x, (*(glm::vec4*)(it->second.val)).y, (*(glm::vec4*)(it->second.val)).z, (*(glm::vec4*)(it->second.val)).w);
			}
			else if (it->second.typeId == typeid(glm::mat4).hash_code())
			{
				glUniformMatrix4fv(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), 1, GL_FALSE, glm::value_ptr(*(glm::mat4*)(it->second.val)));
			}
		}
	}

	void Material::SetSubViewportRect(float left, float bottom, float right, float top)
	{
		subViewportRect = glm::vec4(left, bottom, right, top);
	}
}