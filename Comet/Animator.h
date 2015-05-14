#pragma once

#include "Component.h"
#include "Animation.h"

#include <functional>

namespace Comet
{

	class Animator : public Component
	{
	public:
		Animator();
		~Animator();

		void SetAnimation(Animation* a)
		{
			anim = a;

			//TODO: parse animation and add animators to the child nodes
		}

		void AddTime(float t)
		{
			if (time >= anim->GetLength())
				time = 0;
			time += t;
			_updateData();
		}
		void SetTime(float t)
		{ 
			t -= (t / anim->GetLength()) * (t / anim->GetLength());
			time = t;
			_updateData();
		}
	private:
		void _updateData();

		float time;
		Animation* anim;
	};

}