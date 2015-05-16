#pragma once

#include "res_ptr.h"

#include <set>
#include <map>
#include <stack>
#include "Resource.h"
#include "MeshData.h"
#include "Texture2D.h"
#include "Font.h"
#include "Animation.h"

#include <thread>

namespace Comet
{

	class ResMan
	{
	public:
		ResMan();
		~ResMan();

		void Initialize();
		void Destroy();

		template<class T>
		T* Get(std::string path)
		{
			std::map<std::string, Resource*>::iterator it;
			it = resources.find(path);

			if (it != resources.end())	//������ ��� ����, ������ ������������
			{
				return (T*)(it->second);
			}
			else						//���� ���������
			{
				//printf("Loading \"%s\". Param stack is empty:%i\n", path.c_str(), loadParamStack.empty());
				T* res = new T();
				((Resource*)res)->Load(path);
				resources.insert(std::make_pair(path, res));
				//printf("Loaded \"%s\". Param stack is empty:%i\n", path.c_str(), loadParamStack.empty());
				return res;
			}
		}

		void Set(std::string name, Resource* res)
		{
			std::map<std::string, Resource*>::iterator it;
			it = resources.find(name);

			if (it != resources.end())
			{//������� ������
				delete it->second;
				resources.erase(it);
			}

			//�������� ������
			resources.insert(std::make_pair(name, res));
		}

		std::stack<int>* GetParamStack(){ return &loadParamStack; }

	private:
		std::stack<int> loadParamStack;
		std::map<std::string, Resource*> resources;
	};


	ResMan &Resources();
};