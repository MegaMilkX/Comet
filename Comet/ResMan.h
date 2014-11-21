#pragma once

#include <set>
#include <map>
#include "Resource.h"
#include "MeshData.h"
#include "Texture2D.h"

namespace Comet
{

	class ResMan
	{
	public:
		ResMan();
		~ResMan();

		static void Initialize();
		static void Destroy();
		static ResMan* GetInstance(){ return instance; }

		template<class T>
		T* GetResource(std::string path)
		{
			std::map<std::string, Resource*>::iterator it;
			it = resources.find(path);

			if (it != resources.end())	//Ресурс уже есть, отдаем существующий
			{
				return (T*)(it->second);
			}
			else						//Надо загружать
			{
				T* res = new T();
				((Resource*)res)->Load(path);
				resources.insert(std::make_pair(path, res));
				return res;
			}
		}

	private:
		static ResMan* instance;

		std::map<std::string, Resource*> resources;
	};



};