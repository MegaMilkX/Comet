#include "RenderTarget.h"

#include "Renderer.h"

namespace Comet
{
	RenderTarget::RenderTarget()
	{
		framebuffer = 0;
	}

	RenderTarget::RenderTarget(unsigned int w, unsigned int h)
	{
		width = w; height = h;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
		glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);

		//CreateImageBuffer("diffuse");
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderer->depthTexture->GetTextureId(), 0);
	}


	RenderTarget::~RenderTarget()
	{
		glDeleteFramebuffers(1, &framebuffer);

		for (int i = 0; i < imageBuffers.size(); i++)
			delete imageBuffers[i];
	}

	Texture2D* RenderTarget::CreateImageBuffer(std::string name)
	{
		Texture2D* texture = new Texture2D(width, height, Texture2D::RGBA);
		imageBuffers.push_back(texture);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + imageBuffers.size() - 1, GL_TEXTURE_2D, texture->GetTextureId(), 0);
		imageNames.insert(std::make_pair(name, imageBuffers.size() - 1));
		return texture;
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
	}
}