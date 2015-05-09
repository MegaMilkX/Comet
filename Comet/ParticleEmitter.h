#pragma once

#include "Renderable.h"

namespace Comet
{

	class ParticleEmitter : public Renderable
	{
	public:
		ParticleEmitter(std::string resname);
		~ParticleEmitter();

		void Render(const Camera *const cam);
	private:
		GLBuffer positionBuffer;
		GLBuffer scaleBuffer;

		//temp
		std::vector<vec3f> scaleBuf;
		float time;
	};

}