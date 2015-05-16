#pragma once

#include "Resource.h"
#include "Texture2D.h"
#include "Shader.h"
#include <vector>
#include <map>
#include <typeinfo>

#include "Node.h"

namespace Comet
{
	class Renderer;
	class Shader;

	struct Param
	{
		size_t	typeId;
		void*	val;
	};

	class Material : public Resource
	{
	public:
		enum POLYMODE {	FILL, WIRE };

								Material(bool make_default = false);
								Material(std::string shader, std::string texture0 = "");
		virtual					~Material();		

		void					Load(std::string path);
		void					MakeDefault()
		{

		}
		void					Unload();

		Shader*					GetShader(){ return shaderProgram; }

		void					SetTexture2D(std::string resname, unsigned char layer = 0);
		void					SetTexture2D(Texture2D* tex, unsigned char layer = 0);
		void					SetShader(std::string resname);
		void					SetShader(Shader* shad);
		void					SetZWrite(bool val){ zwrite = val; }
		void					SetZTest(bool val){ ztest = val; }
		void					SetPolygonMode(POLYMODE mode){ polyMode = mode; }
		void					SetSubViewportRect(float left, float bottom, float right, float top);
		void					SetOrder(int o){ order = o; }
		void					IgnoreParentScale(bool val){ ignoreParentScale = val; }
		void					IgnoreParentPos(bool val){ ignoreParentPos = val; }
		
		void					BindTextures();
		void					SwitchRenderMode();
		void					glSetUniforms();

		int						GetOrder(){ return order; }

		std::vector<Texture2D*> GetTextures(){ return textures; }

		template<typename T>
		void					SetParameter(std::string name, T value);

		friend Renderer;
	private:
		Shader*					shaderProgram;
		std::vector<Texture2D*> textures;
		std::map<std::string, Param> parameters; //Параметры материала любого типа
		bool					zwrite;
		bool					ztest;
		int						order;
		vec4f					subViewportRect;
		bool					ignoreParentPos;
		bool					ignoreParentScale;
		POLYMODE				polyMode;
	};

	template<typename T>
	void Material::SetParameter(std::string name, T value)
	{
		std::map<std::string, Param>::iterator it;
		it = parameters.find(name);
		if (it == parameters.end()) //Добавить параметр
		{
			Param param;
			it = parameters.insert(std::make_pair(name, param)).first;
			it->second.typeId = typeid(T).hash_code();
			it->second.val = new T(value);
		}
		else //Обновить параметр
		{
			((T*)(it->second.val))[0] = value;
		}

		glUseProgram(shaderProgram->GetProgramId());
		glSetUniforms();
	}
}