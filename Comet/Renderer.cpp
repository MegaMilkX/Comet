#include "Core.h"
#include "Renderer.h"
#include <algorithm>

namespace Comet
{
	Renderer* Renderer::instance;

	void window_resize_callback(GLFWwindow* wnd, int width, int height)
	{
		Renderer::instance->_updWindowSize(width, height);

	}

	Renderer::Renderer()
	{
		instance = this;
		fullscreen = false;

		renderTarget = 0;

		wWidth = 1280;
		wHeight = 720;
	}


	Renderer::~Renderer()
	{
		if (renderTarget)
			delete renderTarget;
	}

	void Renderer::Init()
	{
		if (!glfwInit())
			return;

		//glfwSetErrorCallback(? ? ? );

		GLFWmonitor* monitor = 0;
		if (fullscreen)
			monitor = glfwGetPrimaryMonitor();

		window = glfwCreateWindow(wWidth, wHeight, "Lava Bulge", monitor, 0);
		glfwSetWindowSizeCallback(window, window_resize_callback);

		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(0);

		//Контекст асинхронной загрузки
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		loading_context = glfwCreateWindow(640, 480, "000", 0, window);
		//

		glewInit();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);



		glEnable(GL_SCISSOR_TEST);

		glEnable(GL_CULL_FACE);
		//glDisable(GL_CULL_FACE);	//TODO: Убрать, куллинг контролируется материалом
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);

		glPointSize(3);

		//////////////////////////////////////////
		renderTarget = new RenderTarget();
		renderTarget->Resize(1280, 720);
	}

	void Renderer::_setZTest(bool val)
	{
		if (ztest == val)
			return;

		if (val)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_NEVER);
		}

		ztest = val;
	}
	void Renderer::_setZWrite(bool val)
	{
		if (zwrite == val)
			return;

		if (val)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

		zwrite = val;
	}

	void Renderer::_setPolyMode(int m)
	{
		if (m == 0)
		{
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		else if (m == 1)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
		}
	}

	bool Renderer::Update()
	{
		if (glfwWindowShouldClose(window))
			return false;

		//Render cameras
		std::set<Camera*>::iterator camIt;
		for (camIt = cameras.begin(); camIt != cameras.end(); camIt++)
			_renderCamera(*camIt);


		glfwSwapBuffers(window);

		glfwPollEvents();
		return true;
	}

	void Renderer::_renderFullscreenQuad(Material* mat)
	{
		_setZTest(false);
		_setZWrite(false);
		_setPolyMode(0);
		glUseProgram(mat->GetShader()->GetProgramId());
		mat->BindTextures();
		mat->glSetUniforms();

		glBegin(GL_TRIANGLES);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
		glEnd();
	}

	void Renderer::_renderCamera(Camera* cam)
	{
		RenderTarget* rt = cam->GetRenderTarget();
		if (!rt)
			return;
		cameraRendering = cam;

		rt->Bind();

		_setZWrite(true);
		_setZTest(true);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::vector<Renderable*> sorted(renderables.begin(), renderables.end());
		std::sort(sorted.begin(), sorted.end(), renderableCompare());
		std::vector<Renderable*>::iterator rIt;
		for (rIt = sorted.begin(); rIt != sorted.end(); rIt++)
		{
			(*rIt)->Update();
			//_render(cam, (*rIt));
			(*rIt)->Render(cam);
		}

		//Go through post-processes
		_renderPostProcess(rt->GetPostProcess());
	}

	void Renderer::_renderPostProcess(PostProcess* pp)
	{
		if (!pp)
			return;

		pp->GetOutput()->Bind();
		_renderFullscreenQuad(pp->GetMaterial());
	}

	void Renderer::_render(Camera* cam, Renderable* r)
	{
		if (!r->GetNode())
			return;
		if (!r->GetMeshData())
			return;
		if (!r->GetMaterial())
			return;
		if (!r->GetMaterial()->GetShader())
			return;

		//TODO: think about submeshes and what to do with them

		Shader* shader = r->GetMaterial()->GetShader();



		_setZTest(r->GetMaterial()->ztest);
		_setZWrite(r->GetMaterial()->zwrite);
		_setPolyMode(r->GetMaterial()->polyMode);

		glUseProgram(r->GetMaterial()->GetShader()->GetProgramId());
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::M), 1, GL_FALSE, (GLfloat*)&(r->GetNode()->GetTransform()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::V), 1, GL_FALSE, (GLfloat*)&(cam->GetView()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::P), 1, GL_FALSE, (GLfloat*)&(cam->GetProjection()));

		r->GetMaterial()->BindTextures();


		r->GetMeshData()->Bind(shader);
		r->GetMeshData()->Render();
	}

	//Переделать в будущем. Пусть метод берет примитивы из менеджера ресурсов, а когда их нет, то записывает их туда же и отдает.
	MeshData* Renderer::GetMeshDataPrimitive(std::string name)
	{
		MeshData* data = 0;

		std::map<std::string, MeshData*>::iterator it;
		it = meshPrimitives.find(name);

		if (it != meshPrimitives.end())
		{
			data = it->second;
		}
		else
		{//Примитив не нашелся, пытаемся создать
			if (name == "plane")
			{
				data = new MeshData();
				//Заполнить мешдату
				std::vector<float> vertices;
				vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
				vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
				vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.0f);
				vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.0f);
				data->FillPosition(vertices);
				std::vector<float> uvw;
				uvw.push_back(0.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
				uvw.push_back(1.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
				uvw.push_back(1.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
				uvw.push_back(0.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
				data->FillUVW(uvw);
				std::vector<unsigned short> indices;
				indices.push_back(0); indices.push_back(1); indices.push_back(3);
				indices.push_back(1); indices.push_back(2); indices.push_back(3);
				data->FillIndices(indices);
			}
		}
		return data;
	}

	void Renderer::_regRenderable(Renderable* r)
	{
		renderables.insert(r);
	}
	void Renderer::_regCamera(Camera* cam)
	{
		cameras.insert(cam);
	}
	void Renderer::_regLight(Light* l)
	{
		lights.insert(l);
	}


	//DEBUG ONLY
	//
};