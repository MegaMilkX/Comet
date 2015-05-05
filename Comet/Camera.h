#pragma once

#include "RenderObject.h"
#include <math3f.h>
#include "RenderTarget.h"

namespace Comet
{

	class Camera : public RenderObject
	{
	public:
		Camera();
		~Camera();

		void Perspective(float fov, float aspect, float zNear, float zFar);
		void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

		mat4f GetProjection() const;
		mat4f GetView() const;

		void SetRenderTarget(RenderTarget* rt);
		RenderTarget* GetRenderTarget() const { return renderTarget; }

		friend RenderTarget;
	private:
		mat4f projection;
		mat4f view;

		float fov;
		float zNear, zFar;
		float aspectW, aspectH;
		bool dirty;

		RenderTarget* renderTarget;
	};

};