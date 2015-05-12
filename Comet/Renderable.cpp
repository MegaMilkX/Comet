#include "Renderable.h"

#include "Core.h"

namespace Comet
{

	Renderable::Renderable()
	{
		meshData = 0;

		Core::GetInstance()->GetRenderer()->_regRenderable(this);
	}


	Renderable::~Renderable()
	{
	}

	void Renderable::SetMeshData(std::string resname)
	{
		meshData = Resources().Get<MeshData>(resname);
	}

	void Renderable::SetMaterial(std::string resname)
	{
		mat = Resources().Get<Material>(resname);
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