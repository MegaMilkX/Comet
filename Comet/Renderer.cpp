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
		rootNode = new Node();
		fullscreen = false;

		renderTarget = 0;

		wWidth = 1280;
		wHeight = 720;
	}


	Renderer::~Renderer()
	{
		if (renderTarget)
			delete renderTarget;
		delete rootNode;
	}

	void Renderer::Init()
	{
		if (!glfwInit())
			return;

		//glfwSetErrorCallback(? ? ? );

		GLFWmonitor* monitor = 0;
		if (fullscreen)
			monitor = glfwGetPrimaryMonitor();

		window = glfwCreateWindow(wWidth, wHeight, "Super Peppermint", monitor, 0);
		glfwSetWindowSizeCallback(window, window_resize_callback);

		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
			_render(cam, (*rIt));
		}

		//Go through post-processes
		_renderPostProcess(rt->GetPostProcess());

		//_renderFullscreenQuad(0);
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

		//////////////////////////////////////////////////////////////////////////////////////////
		//Rendering
		//TODO: Iterate through MeshData's submeshes in most effective way and render each of them
		//If a submesh has no material attached to it, fall back to MeshData's material
		//If meshdata has only one big mesh it will be stored in the 0th submesh
		//////////////////////////////////////////////////////////////////////////////////////////





		////////////////////////////////////////////////////////////////////////////////////////////////
		//Setting up data arrays (these are only one of each for a renderable) and for all its submeshes
		////////////////////////////////////////////////////////////////////////////////////////////////
		Shader* shader = r->GetMaterial()->GetShader();
		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_POS)
		{
			glGetAttribLocation(0, "vertexPosition_modelspace");
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::POS));	//координаты
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetPosBuffer());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::POS), 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, 0);
		}
		
		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_UVW)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::UVW));	//uvw
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetUVWBuffer());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::UVW), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_NOR)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::NRM));	//нормали
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetNormBuffer());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::NRM), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_COL)
		{
			glEnableVertexAttribArray(shader->GetAttribLocation(Shader::COL));	//цвет
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetColBuffer());
			glVertexAttribPointer(shader->GetAttribLocation(Shader::COL), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Setting Render primitive type for the whole mesh
		GLenum primType = 0;
		unsigned long nv = 0;
		switch (r->GetMeshData()->GetPrimitiveType())
		{
		case MeshData::POINT: primType = GL_POINTS; break;
		case MeshData::LINE: primType = GL_LINES; break;
		case MeshData::TRIANGLE: primType = GL_TRIANGLES; break;
		case MeshData::TRISTRIP: primType = GL_TRIANGLE_STRIP; nv = r->GetMeshData()->GetNumVerts() + 1; break;
		default:  break;
		}

		nv = r->GetMeshData()->GetNumVerts();

		_setZTest(r->GetMaterial()->ztest);
		_setZWrite(r->GetMaterial()->zwrite);
		_setPolyMode(r->GetMaterial()->polyMode);

		glUseProgram(r->GetMaterial()->GetShader()->GetProgramId());
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::M), 1, GL_FALSE, glm::value_ptr(r->GetNode()->GetTransform()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::V), 1, GL_FALSE, glm::value_ptr(cam->GetView()));
		glUniformMatrix4fv(shader->GetAttribLocation(Shader::P), 1, GL_FALSE, glm::value_ptr(cam->GetProjection()));

		r->GetMaterial()->BindTextures();

		if (r->GetMeshData()->GetFaceBuffer())
		{
			if (r->GetMeshData()->GetNumSubMeshes() > 0)
			{
				for (int i = 0; i < r->GetMeshData()->GetNumSubMeshes(); i++)
				{
					SubMeshData* subMesh = r->GetMeshData()->GetSubMesh(i);

					//Render now
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->GetMeshData()->GetFaceBuffer());
					glDrawElements(primType, subMesh->faces() * 3, GL_UNSIGNED_SHORT, (void*)(subMesh->offset()));
				}
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->GetMeshData()->GetFaceBuffer());
				glDrawElements(primType, r->GetMeshData()->GetNumFaces() * 3, GL_UNSIGNED_SHORT, 0);
			}
		}
		else // No indices - no submeshes. TODO At least for now
		{
			glDrawArrays(primType, 0, nv);
		}
	}

	void Renderer::_renderNodeUnsorted(Camera* cam, Node* node)
	{
		std::set<RenderObject*>::iterator nIt;
		for (nIt = node->objects.begin(); nIt != node->objects.end(); nIt++)
		{
			if ((*nIt)->IsRenderable())
			{
				_render(cam, (Renderable*)(*nIt));
			}
		}

		std::set<Node*> nodes = node->GetNodes();
		std::set<Node*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			_renderNodeUnsorted(cam, (*it));
		}
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
};