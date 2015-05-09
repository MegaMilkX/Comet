#include "ParticleEmitter.h"

#include "Core.h"

namespace Comet
{

	ParticleEmitter::ParticleEmitter(std::string resname)
	{
		SetMeshData(resname);

		std::vector<vec3f> posBuf;

		for (int i = 0; i < 30; i++)
		{
			vec3f p((float)rand() / (float)RAND_MAX * 10.0f, (float)rand() / (float)RAND_MAX * 10.0f, (float)rand() / (float)RAND_MAX * 10.0f);
			posBuf.push_back(p);
		}

		for (int i = 0; i < 30; i++)
		{
			float s = ((float)rand()) / (float)RAND_MAX;
			vec3f p(s, s, s);
			scaleBuf.push_back(p);
		}

		positionBuffer.Fill(posBuf);
		scaleBuffer.Fill(scaleBuf);

		time = 0;
	}


	ParticleEmitter::~ParticleEmitter()
	{
	}

	void ParticleEmitter::Render(const Camera *const cam)
	{
		if (!GetNode())
			return;
		if (!GetMeshData())
			return;
		if (!GetMaterial())
			return;
		if (!GetMaterial()->GetShader())
			return;

		time += Core::GetInstance()->GetDt();

		for (int i = 0; i < 30; i++)
		{
			scaleBuf[i].x = sin(time + i) * 0.5f + 0.5f;
			scaleBuf[i].y = sin(time + i) * 0.5f + 0.5f;
			scaleBuf[i].z = sin(time + i) * 0.5f + 0.5f;
		}
		scaleBuffer.Fill(scaleBuf);

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

		//TODO:
		//Bind instance buffer
		//and stuff
		//i dunno
		glEnableVertexAttribArray(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instancePosition"));
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer.Id());
		glVertexAttribPointer(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instancePosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instancePosition"), 1);

		glEnableVertexAttribArray(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instanceScale"));
		glBindBuffer(GL_ARRAY_BUFFER, scaleBuffer.Id());
		glVertexAttribPointer(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instanceScale"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(glGetAttribLocation(mat->GetShader()->GetProgramId(), "instanceScale"), 1);

		meshData->Bind(mat->GetShader());
		meshData->RenderInstanced(30);
	}
}