#include "Camera.h"

#include "Core.h"

namespace Comet
{

	Camera::Camera()
	{
		Core::GetInstance()->GetRenderer()->_regCamera(this);
		RenderTarget* rt = Core::GetInstance()->GetRenderer()->GetRenderTarget();
		if (rt->camera == 0)
			SetRenderTarget(Core::GetInstance()->GetRenderer()->GetRenderTarget());

		Perspective(95.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);
	}


	Camera::~Camera()
	{
		if (renderTarget)
			renderTarget->camera = 0;
	}

	void Camera::SetRenderTarget(RenderTarget* rt)
	{
		if (renderTarget)
			renderTarget->camera = 0;
		renderTarget = rt;
		renderTarget->camera = this;
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