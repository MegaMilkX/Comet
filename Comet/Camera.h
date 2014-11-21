#pragma once

#include "RenderObject.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Viewport.h"

namespace Comet
{

	class Camera : public RenderObject
	{
	public:
		Camera();
		~Camera();

		void Perspective(float fov, float aspect, float zNear, float zFar);
		void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

		glm::mat4 GetProjection();
		glm::mat4 GetView();

		void SetViewport(Viewport* vp);
		Viewport* GetViewport(){ return viewport; }
	private:
		glm::mat4 projection;
		glm::mat4 view;

		float fov;
		float zNear, zFar;
		float aspectW, aspectH;
		bool dirty;

		Viewport* viewport;
	};

};