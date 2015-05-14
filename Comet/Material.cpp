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

#include "Core.h"

namespace Comet
{

	Material::Material()
	{
		shaderProgram = 0;
		zwrite = true;
		ztest = true;
		order = 0;

		subViewportRect = vec4f(0, 0, 0, 0);

		ignoreParentPos = false;
		ignoreParentScale = false;
		polyMode = FILL;
	}

	Material::Material(std::string shader, std::string texture0) : Material()
	{
		SetShader(shader);
		if (texture0 != "")
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
		SetTexture2D(Resources().Get<Texture2D>(resname), layer);
	}

	void Material::SetTexture2D(Texture2D* tex, unsigned char layer)
	{
		if (textures.size() >= layer)
		{
			textures.resize(layer + 1);
		}

		textures[layer] = tex;

		std::vector<unsigned int> smpl;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			smpl.push_back(i);
		}
		glUseProgram(GetShader()->GetProgramId());
		glUniform1iv(glGetUniformLocation(GetShader()->GetProgramId(), "textures"), smpl.size(), (GLint*)(smpl.data()));
	}

	void Material::SetShader(std::string resname)
	{
		SetShader(Resources().Get<Shader>(resname));
	}

	void Material::SetShader(Shader* shad)
	{
		shaderProgram = shad;
	}

	void Material::BindTextures()
	{
		for (unsigned int i = 0; i < GetTextures().size(); i++)
		{
			if (!(GetTextures()[i]->IsReady()))
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
				continue;
			}

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, GetTextures()[i]->GetTextureId());
			
		}
	}

	void Material::SwitchRenderMode()
	{
		Core::GetInstance()->GetRenderer()->_setZTest(ztest);
		Core::GetInstance()->GetRenderer()->_setZWrite(zwrite);
		Core::GetInstance()->GetRenderer()->_setPolyMode(polyMode);
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
			else if (it->second.typeId == typeid(vec3f).hash_code())
			{
				glUniform3f(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), (*(vec3f*)(it->second.val)).x, (*(vec3f*)(it->second.val)).y, (*(vec3f*)(it->second.val)).z);
			}
			else if (it->second.typeId == typeid(vec4f).hash_code())
			{
				glUniform4f(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), (*(vec4f*)(it->second.val)).x, (*(vec4f*)(it->second.val)).y, (*(vec4f*)(it->second.val)).z, (*(vec4f*)(it->second.val)).w);
			}
			else if (it->second.typeId == typeid(mat4f).hash_code())
			{
				//TODO: Check
				glUniformMatrix4fv(glGetUniformLocation(GetShader()->GetProgramId(), it->first.c_str()), 1, GL_FALSE, (GLfloat*)&(*(mat4f*)(it->second.val)));
			}
		}
	}

	void Material::SetSubViewportRect(float left, float bottom, float right, float top)
	{
		subViewportRect = vec4f(left, bottom, right, top);
	}
}