#include "Animation.h"

namespace Comet
{

	Animation::Animation()
	{
		time = 0;
		length = 2.0f;
	}


	Animation::~Animation()
	{
	}

	void Animation::Update(const float &dt)
	{
		if (time > length)
			time = 0.0f;

		for (int i = 0; i < tracks.size(); ++i)
		{
			tracks[i]->Evaluate(time);
		}
		time += dt;
	}
}