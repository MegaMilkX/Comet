#include "RenderTarget.h"

#include "Renderer.h"

namespace Comet
{
	RenderTarget::RenderTarget()
	{
		framebuffer = 0;
		camera = 0;
		depthBuffer = 0;
		postProcess = 0;
	}

	RenderTarget::RenderTarget(unsigned int w, unsigned int h)
	{
		width = w; height = h;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
		glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);

		//CreateImageBuffer("diffuse");
		depthBuffer = new Texture2D(width, height, Texture2D::DEPTH24);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureId(), 0);

		postProcess = 0;

		camera = 0;
	}


	RenderTarget::~RenderTarget()
	{
		glDeleteFramebuffers(1, &framebuffer);

		delete depthBuffer;

		for (int i = 0; i < imageBuffers.size(); i++)
			delete imageBuffers[i];

		if (camera)
			camera->renderTarget = 0;
	}

	Texture2D* RenderTarget::CreateImageBuffer(std::string name, Texture2D::TEXFMT fmt)
	{
		if (!framebuffer)
		{
			printf("[RT][X] You can't add buffers to the default render target.\n");
			return 0;
		}
		Texture2D* texture = new Texture2D(width, height, fmt, GL_FLOAT);
		imageBuffers.push_back(texture);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + imageBuffers.size() - 1, GL_TEXTURE_2D, texture->GetTextureId(), 0);
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + imageBuffers.size() - 1);
		imageNames.insert(std::make_pair(name, imageBuffers.size() - 1));
		
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());

		//Update post process textures if they're there

		if (postProcess)
		{
			int i;
			for (i = 0; i < imageBuffers.size(); i++)
			{
				postProcess->GetMaterial()->SetTexture2D(imageBuffers[i], i);
			}
			postProcess->GetMaterial()->SetTexture2D(depthBuffer, i);
		}
		return texture;
	}

	PostProcess* RenderTarget::CreatePostProcess()
	{
		return 0;
	}
	PostProcess* RenderTarget::CreatePostProcess(Material* mat)
	{
		postProcess = new PostProcess(mat);

		int i;
		for (i = 0; i < imageBuffers.size(); i++)
		{
			mat->SetTexture2D(imageBuffers[i], i);
		}
		mat->SetTexture2D(depthBuffer, i);

		return postProcess;
	}

	Texture2D* RenderTarget::GetImageBuffer(std::string name)
	{
		std::map<std::string, unsigned int>::iterator it = imageNames.find(name);
		if (it != imageNames.end())
		{
			return imageBuffers[it->second];
		}
		else
			return 0;
	}
	
	bool RenderTarget::IsOK()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);

		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status == GL_FRAMEBUFFER_COMPLETE)
			return true;
		else
			return false;
	}

	void RenderTarget::Bind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);			
		glViewport(0, 0, Width(), Height());
	}

	void RenderTarget::Resize(unsigned int w, unsigned int h)
	{
		width = w; height = h;
		if (framebuffer)
		{
			//TODO Resize fbo and its textures
		}
	}
}