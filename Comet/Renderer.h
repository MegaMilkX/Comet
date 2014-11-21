#pragma once

#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Viewport.h"
#include "Renderable.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "Node.h"

namespace Comet
{
	class Node;
	class Renderable;
	class Core;
	class Camera;
	class Light;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Init();
		bool Update();

		Viewport* CreateViewport();

		GLFWwindow* GetWindow(){ return window; }

		friend Core;
		friend Camera;
		friend Viewport;
		friend Mesh;
		friend Light;
	private:
		Node* rootNode;
		std::set<Viewport*> viewports;
		std::set<Renderable*> renderables;
		std::set<Camera*> cameras;
		std::set<Light*> lights;
		//std::vector<Material*> materials;

		GLFWwindow* window;
		bool fullscreen;

		/*TEMPORARY*/
		GLuint shaderProgramId;
		/**/

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

		void _regViewport(Viewport* vp);
		void _regRenderable(Renderable* r);
		void _regCamera(Camera* cam);
		void _regLight(Light* l);

		void _render(Camera* cam, Renderable* r);
	};

};