#pragma once

#include "Renderer.h"
#include "Physics.h"
#include "ResMan.h"

#include "Animation.h"

#include <sys/utime.h>

#include <tinyxml.h>

#include <stack>

namespace Comet
{

	class Core
	{
	public:
								Core();
		virtual					~Core();

		static Core*			GetInstance(){ return instance; }

		virtual void			Init();
		virtual void			PostInit(){}
		virtual bool			Update();
		void					Reset();

		Node*					SceneRoot() { return &sceneRoot; }

		Animation*				CreateAnimation();
		Animation*				CreateAnimation(std::string resourcename);

		//Commented out for the time being.
		//void					ReadGraphFile(std::string path, Node* node);

		Renderer*				GetRenderer() const { return renderer; }
		Physics*				GetPhysics() const { return physics; }

		MeshData*				GetMeshDataPrimitive(std::string name);
		double					GetDt() const { return dt; }
	
	protected:
		void					_preUpdate();
		bool					_postUpdate();

		Node					sceneRoot;

		double					time0 = 0,
								time1 = 0;
		double					dt;			//Delta Time

		static Core*			instance;

		Renderer*				renderer;
		void*					audio;
		Physics*				physics;

		std::set<Animation*>	animations;
	private:
		//void					_digestXmlElement(TiXmlElement* elem);
	};

};