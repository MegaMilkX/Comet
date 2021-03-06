#include "Core.h"



namespace Comet
{
	Core* Core::instance;

	Core::Core() : dt(0)
	{
		instance = this;

		renderer = new Renderer();
		renderer->core = this;

		physics = new Physics();

		dt = 0;

		sceneRoot.SetName("SceneRoot");
	}


	Core::~Core()
	{
		delete physics;
		delete renderer;
	}

	void Core::Init()											//������������� ����� ---------------------------------------------------
	{
		renderer->Init();

		//Loading and/or creating default resources
		//Such as font shader and placeholder textures

		Shader* fontShader = new Shader();
		Resources().Set("ShaderFontScreenDefault", fontShader);
		//fontShader->Load("data\\shaders\\test.glsl");
		fontShader->Load(
			"#version 330 core\n"
			"// Interpolated values from the vertex shaders\n"
			"in vec3 fragmentColor;\n"
			"in vec3 texCoord;\n"
			"uniform sampler2D mainTexture;\n"
			"// Ouput data\n"
			"out vec4 color;\n"
			"void main(){\n"
			"	// Output color = color specified in the vertex shader,\n"
			"	// interpolated between all 3 surrounding vertices\n"
			"	color = vec4(1.0, 1.0, 1.0, texture2D(mainTexture, texCoord.xy).a);\n"
			"	//color = vec4(texCoord.xy, 0.5f, 1);\n"
			"}\n",
			"#version 330 core\n"
			"// Input vertex data, different for all executions of this shader.\n"
			"layout(location = 0) in vec3 	vertexPosition_modelspace;\n"
			"layout(location = 3) in vec3 	vertexColor;\n"
			"layout(location = 2) in vec3 	vertexNormal;\n"
			"layout(location = 1) in vec3 	uvw;\n"
			"// Output data ; will be interpolated for each fragment.\n"
			"out vec3 fragmentColor; out vec3 texCoord;\n"
			"// Values that stay constant for the whole mesh.\n"
			"uniform mat4 _m;\n"
			"uniform mat4 _v;\n"
			"uniform mat4 _p;\n"
			"uniform float ScreenWidth;\n"
			"uniform float ScreenHeight;\n"
			"uniform float time;\n"
			"mat4 Ortho(float bottom, float left, float right, float top){\n"
			"mat4 m;\n"
			"m[0].x = 2.0 / (right - left);\n"
			"m[1].y = 2.0 / (top - bottom); \n"
			"m[3].w = 1.0; \n"
			"m[2].z = -1.0; \n"
			"m[3].x = -((right+left)/(right-left)); \n"
			"m[3].y = -((top+bottom)/(top-bottom)); \n"
			"m[3].z = 0; \n"
			"return m;\n"
			"}\n"
			"void main(){\n"
			"mat4 _pp = Ortho(0, 0, ScreenWidth, ScreenHeight);\n"
			"vec3 pos = vertexPosition_modelspace;\n"
			"//pos.y = pos.y+cos(time+pos.x*0.3)*10.0;\n"
			"gl_Position = (_pp * _m) * vec4(pos, 1);\n "//rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
			"fragmentColor = vertexColor;\n"
			"//fragmentColor = vertexPosition_modelspace;\n"
			"texCoord = uvw;\n"
			"}\n");


		MeshData* meshRectDefault = new MeshData();
		Resources().Set("MeshRectDefault", meshRectDefault);
		std::vector<float> vertices;
		vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
		vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
		vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
		meshRectDefault->FillPosition(vertices);
		std::vector<float> uvw;
		uvw.push_back(0.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
		uvw.push_back(1.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
		uvw.push_back(1.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
		uvw.push_back(0.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
		meshRectDefault->FillUVW(uvw);
		//std::vector<unsigned short> indices;
		//indices.push_back(0); indices.push_back(1); indices.push_back(3);
		//indices.push_back(1); indices.push_back(2); indices.push_back(3);
		//meshRectDefault->FillIndices(indices);
		meshRectDefault->SetPrimitiveType(MeshData::PrimitiveType::TRISTRIP);

		printf("[OK]Ready.\n");
		
	}

	bool Core::Update()
	{
		_preUpdate();
		return _postUpdate();
	}

	void Core::Reset()											//����� ������������ ��� ����������� � ��� ������ -----------------------
	{
	}

	void Core::_preUpdate()
	{
		physics->Update();
	}

	bool Core::_postUpdate()
	{
		bool val = renderer->Update();
		time1 = glfwGetTime();
		dt = time1 - time0;
		time0 = glfwGetTime();

		return val;
	}

	/*
	void Core::ReadGraphFile(std::string path, Node* node)
	{
		TiXmlDocument doc(path.c_str());
		bool isOk = doc.LoadFile();
		if (!isOk)
			return;

		TiXmlElement* rootElem = doc.FirstChildElement();
		if (!rootElem)
			return;

		//We got us a root element, now it's time to fill the scene!
		_digestXmlElement(rootElem);



		entityStack.empty();
	}*/

	//DON'T DELETE that, but rewrite. It has some useful code.
	/*
	void Core::_digestXmlElement(TiXmlElement* elem)
	{
		for (TiXmlElement* e = elem->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			//printf(e->Value()); printf("\n");

			if (std::string(e->Value()) == "entity")
			{
				Entity* ent = CreateEntity();
				entityStack.push(ent);
				_digestXmlElement(e);
				entityStack.pop();
			}
			else if (std::string(e->Value()) == "transform")
			{
				Entity* ent = entityStack.top();
				ent->AddComponent(renderer->GetRoot()->CreateNode());
				TiXmlElement* pos = e->FirstChildElement("position");
				const char* attr;
				float fx = 0, fy = 0, fz = 0, fw = 0;
				if (pos)
				{
					attr = pos->Attribute("x");
					if (attr) fx = std::stof(std::string(attr));
					attr = pos->Attribute("y");
					if (attr) fy = std::stof(std::string(attr));
					attr = pos->Attribute("z");
					if (attr) fz = std::stof(std::string(attr));
					ent->GetComponent<Node>()->SetPosition(vec3f(fx, fy, fz));
				}
				TiXmlElement* rot = e->FirstChildElement("rotation");
				if (rot)
				{
					attr = rot->Attribute("x");
					if (attr) fx = std::stof(std::string(attr));
					attr = rot->Attribute("y");
					if (attr) fy = std::stof(std::string(attr));
					attr = rot->Attribute("z");
					if (attr) fz = std::stof(std::string(attr));
					attr = rot->Attribute("w");
					if (attr) fw = std::stof(std::string(attr));
					ent->GetComponent<Node>()->SetRotation(quat(fx, fy, fz, fw));
				}
				TiXmlElement* scale = e->FirstChildElement("scale");
				if (scale)
				{
					attr = scale->Attribute("x");
					if (attr) fx = std::stof(std::string(attr));
					attr = scale->Attribute("y");
					if (attr) fy = std::stof(std::string(attr));
					attr = scale->Attribute("z");
					if (attr) fz = std::stof(std::string(attr));
					ent->GetComponent<Node>()->Scale(vec3f(fx, fy, fz));
				}
			}
			else if (std::string(e->Value()) == "mesh")
			{
				Entity* ent = entityStack.top();
				ent->AddComponent(new Mesh("data\\models\\sphere.xyz"));
				//Check xml mesh for material
				ent->GetComponent<Mesh>()->SetMaterial(new Material("data\\shaders\\sphere.glsl", "data\\textures\\2014-11-20-688057.jpeg"));
			}
			else if (std::string(e->Value()) == "rigidbody")
			{
				Entity* ent = entityStack.top();
				btCollisionShape* shape = new btSphereShape(1);
				ent->AddComponent(physics->CreateRigidBody(shape, 0.0f));
			}
			else if (std::string(e->Value()) == "light") //TODO
			{
				Entity* ent = entityStack.top();
				//ent->AddComponent();
			}
			else if (std::string(e->Value()) == "camera")
			{
				Entity* ent = entityStack.top();
				ent->AddComponent(new Camera());
				ent->GetComponent<Camera>()->Perspective(95.0f, 1280.0f / 720.0f, 0.001f, 1000.0f);
			}
		}
	}*/
};