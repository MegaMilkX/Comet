#include "Renderer.h"

namespace Comet
{

	Renderer::Renderer()
	{
		rootNode = new Node();
		fullscreen = false;
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

		window = glfwCreateWindow(1280, 720, "", monitor, 0);

		glfwMakeContextCurrent(window);

		glewInit();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glEnable(GL_SCISSOR_TEST);

		/*TEMPORARY*/
		shaderProgramId = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		
	}

	bool Renderer::Update()
	{
		if (glfwWindowShouldClose(window))
			return false;

		std::set<Viewport*>::iterator vIt;
		for (vIt = viewports.begin(); vIt != viewports.end(); vIt++)
		{
			glViewport(0, 0, 1280, 720);
			glScissor(0, 0, 1280, 720);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if ((*vIt)->GetCamera())
			{
				std::set<Renderable*>::iterator rIt;
				for (rIt = renderables.begin(); rIt != renderables.end(); rIt++)
				{
					_render((*vIt)->GetCamera(), *rIt);
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
		return new Viewport();
	}

	void Renderer::_render(Camera* cam, Renderable* r)
	{
		if (!r->GetNode())
			return;
		glUseProgram(shaderProgramId);
		glm::mat4 mvp = cam->GetProjection() * cam->GetView() * r->GetNode()->GetTransform();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "_mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_POS)
		{
			glEnableVertexAttribArray(0);	//координаты
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetPosBuffer());
			glVertexAttribPointer(glGetAttribLocation(shaderProgramId, "vertexPosition_modelspace"), 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
		}
		
		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_UVW)
		{
			glEnableVertexAttribArray(1);	//uvw
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetUVWBuffer());
			glVertexAttribPointer(glGetAttribLocation(shaderProgramId, "uvw"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_NOR)
		{
			glEnableVertexAttribArray(2);	//нормали
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetNormBuffer());
			glVertexAttribPointer(glGetAttribLocation(shaderProgramId, "vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (r->GetMeshData()->GetVertexAttribLayout() & VATTR_COL)
		{
			glEnableVertexAttribArray(3);	//цвет
			glBindBuffer(GL_ARRAY_BUFFER, r->GetMeshData()->GetColBuffer());
			glVertexAttribPointer(glGetAttribLocation(shaderProgramId, "vertexColor"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Текстурки
		for (int i = 0; i < r->GetTextures().size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, r->GetTextures()[i]->GetTextureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glUniform1i(glGetUniformLocation(shaderProgramId, "mainTexture"), i);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->GetMeshData()->GetFaceBuffer());
		
		glDrawElements(GL_TRIANGLES, r->GetMeshData()->GetNumFaces()*3, GL_UNSIGNED_SHORT, 0);
	}

	GLuint Renderer::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
	{
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()){
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else{
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()){
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}



		GLint Result = GL_FALSE;
		int InfoLogLength;



		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0){
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0){
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}



		// Link the program
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0){
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
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