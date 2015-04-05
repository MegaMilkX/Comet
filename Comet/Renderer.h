#pragma once

#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Viewport.h"
#include "Renderable.h"
#include "Mesh.h"
#include "Landscape.h"
#include "TextRenderable.h"
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

	struct renderableCompare
	{
		//На этот момент ни у одного из рендераблов еще нет материала
		bool operator() (Renderable* a, Renderable* b) const
		{
			Material *mata, *matb;
			mata = a->GetMaterial();
			matb = b->GetMaterial();
			if (mata==0) return true;
			if (matb==0) return false;

			return (mata->GetOrder() < matb->GetOrder());
		}
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Init();
		bool Update();

		Viewport* CreateViewport();

		Node* GetRoot(){ return rootNode; }
		std::set<Viewport*> GetViewports(){ return viewports; }
		GLFWwindow* GetWindow(){ return window; }
		GLFWwindow* GetSecondContext(){ return loading_context; }
		unsigned int GetWindowWidth(){ return wWidth; }
		unsigned int GetWindowHeight(){ return wHeight; }
		void _updWindowSize(int width, int height){ wWidth = width; wHeight = height; }

		Camera* GetCurrentCamera(){ return cameraRendering; }

		MeshData* GetMeshDataPrimitive(std::string name);


		static Renderer* instance;

		friend Core;
		friend Camera;
		friend Viewport;
		friend Light;
		friend Renderable;
	private:
		Core* core;

		Node* rootNode;
		std::set<Viewport*> viewports;
		std::set<Renderable*> renderables;
		std::set<Camera*> cameras;
		Camera* cameraRendering;
		std::set<Light*> lights;

		std::map<std::string, MeshData*> meshPrimitives;

		GLFWwindow* window;
		unsigned int wWidth, wHeight;
		bool fullscreen;
		GLFWwindow* loading_context;

		bool ztest;
		bool zwrite;

		void _setZTest(bool val);
		void _setZWrite(bool val);

		void _regViewport(Viewport* vp);
		void _regRenderable(Renderable* r);
		void _regCamera(Camera* cam);
		void _regLight(Light* l);

		void _render(Camera* cam, Renderable* r);
		void _renderNodeUnsorted(Camera* cam, Node* node);
	};

};