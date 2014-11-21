#include "Camera.h"

#include "Viewport.h"

#include "Core.h"

namespace Comet
{

	Camera::Camera()
	{
		Core::GetInstance()->GetRenderer()->_regCamera(this);

		fov = 95.0f;
		zNear = 0.1f;
		zFar = 10000.0f;
		aspectW = 4;
		aspectH = 3;

		viewport = 0;
	}


	Camera::~Camera()
	{
	}

	void Camera::SetViewport(Viewport* vp)
	{
		viewport = vp;
		vp->SetCamera(this);
	}

	glm::mat4 Camera::GetProjection()
	{
		return projection;
	}
	glm::mat4 Camera::GetView()
	{
		if (node)
		{
			return glm::inverse(node->GetTransform());
		}
		else
		{
			return glm::inverse(glm::mat4(1.0f));
		}
	}

	void Camera::Perspective(float fov, float aspect, float zNear, float zFar)
	{
		projection = glm::perspective(fov, aspect, zNear, zFar);
	}
	void Camera::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		projection = glm::ortho(left, right, bottom, top, zNear, zFar);
	}

};