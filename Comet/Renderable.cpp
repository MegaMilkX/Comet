#include "Renderable.h"

#include "Core.h"

namespace Comet
{

	Renderable::Renderable()
	{
		meshData = 0;
		textures.clear();

		Core::GetInstance()->GetRenderer()->_regRenderable(this);
	}


	Renderable::~Renderable()
	{
	}

	void Renderable::SetMeshData(std::string resname)
	{
		meshData = ResMan::GetInstance()->GetResource<MeshData>(resname);
	}

	void Renderable::SetMaterial(std::string resname)
	{
		mat = ResMan::GetInstance()->GetResource<Material>(resname);
	}

	void Renderable::SetTexture2D(Texture2D* tex, unsigned char layer)
	{
		if (textures.size() >= layer)
		{
			textures.resize(layer + 1);
		}

		textures[layer] = tex;
	}

	void Renderable::Render(const Camera *const cam)
	{
		if (!GetNode())
			return;
		if (!GetMeshData())
			return;
		if (!GetMaterial())
			return;
		if (!GetMaterial()->GetShader())
			return;

		//TODO: think about submeshes and what to do with them

		Shader* shader = GetMaterial()->GetShader();

		mat->SwitchRenderMode();
		mat->glSetUniforms();
		//Do something with these
		glUseProgram(GetMaterial()->GetShader()->GetProgramId());
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::M), 1, GL_FALSE, (GLfloat*)&(GetNode()->GetTransform()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::V), 1, GL_FALSE, (GLfloat*)&(cam->GetView()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::P), 1, GL_FALSE, (GLfloat*)&(cam->GetProjection()));
		//-----------------------
		mat->BindTextures();

		meshData->Bind(mat->GetShader());
		meshData->Render();
	}
	
};