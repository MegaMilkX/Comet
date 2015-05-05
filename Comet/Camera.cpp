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

	mat4f Camera::GetProjection() const
	{
		return projection;
	}
	mat4f Camera::GetView() const
	{
		if (node)
		{
			return inverse(node->GetTransform());
		}
		else
		{
			return inverse(mat4f(1.0f));
		}
	}

	void Camera::Perspective(float fov, float aspect, float zNear, float zFar)
	{
		projection = perspective(fov, aspect, zNear, zFar);
	}
	void Camera::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		projection = ortho(left, right, bottom, top, zNear, zFar);
	}

};