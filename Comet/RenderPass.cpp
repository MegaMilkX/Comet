#include "RenderPass.h"
#include "ResMan.h"
#include "Renderer.h"
#include "Texture2D.h"

namespace Comet
{

	RenderPass::RenderPass(Renderer* r, RenderPass* parent, Material* mat)
	{
		this->renderer = r;
		this->parent = parent;

		material = mat;

		if (parent)
		{
			glGenFramebuffers(1, &frameBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
			//TODO These parameters may change on window resize or something like that
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 1280);
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 720);

			//Diffuse texture
			CreateImageBuffer("diffuse");
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderer->depthTexture->GetTextureId(), 0);

			GLenum status =	glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
			if (status == GL_FRAMEBUFFER_COMPLETE)
				printf("Framebuffer OK\n");
			else
				printf("Framebuffer is incomplete!\n");

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		else
			frameBuffer = 0;
	}


	RenderPass::~RenderPass()
	{
		std::vector<RenderPass*>::iterator it;
		for (it = passes.begin(); it != passes.end(); it++)
		{
			delete (*it);
		}

		if (parent)
		{
			glDeleteFramebuffers(1, &frameBuffer);
			
			for (int i = 0; i < imageBuffers.size(); i++)
				delete imageBuffers[i];
		}
	}

	RenderPass* RenderPass::CreatePass()
	{
		RenderPass* p = *(passes.insert(passes.end(), new RenderPass(renderer, this)));
		if (material)
			material->SetTexture2D(p->imageBuffers[0], passes.size()-1);
		return p; 
	}

	Texture2D* RenderPass::CreateImageBuffer(std::string name)
	{
		Texture2D* texture = new Texture2D(1280, 720, Texture2D::RGBA);
		imageBuffers.push_back(texture);
		//diffuseTexture = new Texture2D();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureId(), 0);
		imageNames.insert(std::make_pair(name, imageBuffers.size() - 1));
		return texture;
	}

	void RenderPass::SetMaterial(Material* mat)
	{ 
		material = mat;

		std::vector<RenderPass*>::iterator it;
		int i = 0;
		for (it = passes.begin(); it != passes.end(); it++)
		{
			material->SetTexture2D((*it)->imageBuffers[0], i);
			i++;
		}
	}

}