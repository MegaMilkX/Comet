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

		wWidth = 1280;
		wHeight = 720;
	}


	Renderer::~Renderer()
	{
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

		window = glfwCreateWindow(wWidth, wHeight, "Rockit", monitor, 0);
		glfwSetWindowSizeCallback(window, window_resize_callback);

		glfwMakeContextCurrent(window);

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

	bool Renderer::Update()
	{
		if (glfwWindowShouldClose(window))
			return false;

		std::set<Viewport*>::iterator vIt;
		for (vIt = viewports.begin(); vIt != viewports.end(); vIt++)
		{
			glViewport(wWidth*(*vIt)->GetLeft(), wHeight*(*vIt)->GetBottom(), wWidth*((*vIt)->GetLeft() + (*vIt)->GetWidth()), wHeight*((*vIt)->GetBottom() + (*vIt)->GetHeight()));
			glScissor(wWidth*(*vIt)->GetLeft(), wHeight*(*vIt)->GetBottom(), wWidth*((*vIt)->GetLeft() + (*vIt)->GetWidth()), wHeight*((*vIt)->GetBottom() + (*vIt)->GetHeight()));

			_setZWrite(true);
			glClearColor((*vIt)->r, (*vIt)->g, (*vIt)->b, (*vIt)->a);
			glClear((*vIt)->clearFlags);

			Camera* cam = (*vIt)->GetCamera();

			if (cam)
			{
				cameraRendering = cam;
				std::vector<Renderable*> sorted(renderables.begin(), renderables.end());
				std::sort(sorted.begin(), sorted.end(), renderableCompare());

				std::vector<Renderable*>::iterator rIt;
				for (rIt = sorted.begin(); rIt != sorted.end(); rIt++)
				{
					(*rIt)->Update();
					_render(cam, (*rIt));
				}
			}
		}

		//И без сортировки

		//И без отсечения

		glfwSwapBuffers(window);
		glfwPollEvents();

		return true;
	}

	Viewport* Renderer::CreateViewport()
	{
		Viewport* vp = new Viewport();
		vp->width = 1;
		vp->height = 1;
		vp->x = 0;
		vp->y = 0;
		vp->renderer = this;
		return vp;
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

		////////////////////////////////////////////////////////////////////////////////////////////////
		//Setting up data arrays (these are only one of each for a renderable) and for all its submeshes
		////////////////////////////////////////////////////////////////////////////////////////////////
		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_POS)
		{
			glEnableVertexAttribArray(0);	//координаты
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetPosBuffer());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
		}
		
		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_UVW)
		{
			glEnableVertexAttribArray(1);	//uvw
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetUVWBuffer());
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_NOR)
		{
			glEnableVertexAttribArray(2);	//нормали
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetNormBuffer());
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_COL)
		{
			glEnableVertexAttribArray(3);	//цвет
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetColBuffer());
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		//TODO: Iterate through MeshData's submeshes in most effective way and render each of them
		//If a submesh has no material attached to it, fall back to MeshData's material
		//If meshdata has only one big mesh it will be stored in the 0th submesh
		//////////////////////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < r->GetMeshData()->GetNumSubMeshes(); i++)
		{
			SubMeshData* subMesh = r->GetMeshData()->GetSubMesh(i);

			//Render now
		}

		_setZTest(r->GetMaterial()->ztest);
		_setZWrite(r->GetMaterial()->zwrite);

		glUseProgram(r->GetMaterial()->GetShader()->GetProgramId());
		glUniformMatrix4fv(glGetUniformLocation(r->GetMaterial()->GetShader()->GetProgramId(), "_m"), 1, GL_FALSE, glm::value_ptr(r->GetNode()->GetTransform()));
		glUniformMatrix4fv(glGetUniformLocation(r->GetMaterial()->GetShader()->GetProgramId(), "_v"), 1, GL_FALSE, glm::value_ptr(cam->GetView()));
		glUniformMatrix4fv(glGetUniformLocation(r->GetMaterial()->GetShader()->GetProgramId(), "_p"), 1, GL_FALSE, glm::value_ptr(cam->GetProjection()));

		//Текстурки
		for (int i = 0; i < r->GetMaterial()->GetTextures().size(); i++)
		{
			if (!(r->GetMaterial()->GetTextures()[i]->IsReady()))
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
				continue;
			}

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, r->GetMaterial()->GetTextures()[i]->GetTextureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glUniform1i(glGetUniformLocation(r->GetMaterial()->GetShader()->GetProgramId(), "mainTexture"), i);
		}

		r->GetMaterial()->glSetUniforms();

		GLenum primType = 0;
		unsigned long nv = 0;
		switch (r->GetMeshData()->GetPrimitiveType())
		{
		case MeshData::POINT: primType = GL_POINTS; break;
		case MeshData::LINE: primType = GL_LINES; break;
		case MeshData::TRIANGLE: primType = GL_TRIANGLES; break;
		case MeshData::TRISTRIP: primType = GL_TRIANGLE_STRIP; nv = r->GetMeshData()->GetNumVerts() + 1; break;
		default: nv = r->GetMeshData()->GetNumVerts(); break;
		}

		if (r->GetMeshData()->GetFaceBuffer())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->GetMeshData()->GetFaceBuffer());

			glDrawElements(primType, r->GetMeshData()->GetNumFaces() * 3, GL_UNSIGNED_SHORT, 0);
		}
		else
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

	void Renderer::_regViewport(Viewport* vp)
	{
		viewports.insert(vp);
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