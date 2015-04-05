#include "Texture2D.h"

#include "Renderer.h"

#include <mutex>

namespace Comet
{
	std::mutex tex2DMutex;
	void _loadTexture2D(Texture2D* texture2D, std::string path)
	{
		tex2DMutex.lock();
		glfwMakeContextCurrent(Renderer::instance->GetSecondContext());
		
		unsigned int texId = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		if (0 == texId)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}

		unsigned int width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (GLint*)&width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&height);

		texture2D->SetDimensions(width, height);
		texture2D->SetTextureId(texId);
		texture2D->IsReady(true);

		glfwMakeContextCurrent(0);
		tex2DMutex.unlock();
	}

	Texture2D::Texture2D()
	{
	}


	Texture2D::~Texture2D()
	{
		Unload();
	}

	void Texture2D::Load(std::string path)
	{
		/*
		texId = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y);
		if (0 == texId)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (GLint*)&width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&height);
		*/
		std::thread thread(_loadTexture2D, this, path);
		thread.detach();
	}

	//TODO: Сейчас метод принимает только текстуры с ОДНИМ каналом
	void Texture2D::LoadFromMemory(unsigned char* data, unsigned int w, unsigned int h)	//С разными форматами надо подрочить
	{
		glGenBuffers(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		width = w;
		height = h;

		IsReady(true);
	}

	void Texture2D::Unload()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texId);
	}

	
};