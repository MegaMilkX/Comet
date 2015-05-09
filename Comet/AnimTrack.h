#pragma once

#include <set>
#include "Keyframe.h"

namespace Comet
{
	class GenericAnimTrack
	{
	public:
		virtual void Evaluate(float time) = 0;
	};

	template<typename T>
	class AnimTrack : public GenericAnimTrack
	{
	public:
		AnimTrack(T* vptr){ valueptr = vptr; }
		~AnimTrack(){}

		void CreateKeyframe(const float t, const T &val)
		{
			timeline.insert(Keyframe<T>(t, val));
		}

		void Evaluate(float time)
		{
			//TODO
			//(*valueptr) = 0; 
		}
	private:
		std::set<Keyframe<T>> timeline;
		T* valueptr;
	};

	void AnimTrack<float>::Evaluate(float time)
	{
		if (timeline.size() == 0)
			return;

		const Keyframe<float> *a, *b;
		a = &*timeline.begin();
		b = &*timeline.begin();
		std::set<Keyframe<float>>::iterator it;
		for (it = timeline.begin(); it != timeline.end(); ++it)
		{
			if ((*it).gettime() < time)
			{
				a = &(*it);
				b = a;
			}
			else
			{
				b = &(*it);
				break;
			}
		}
		
		float t = b->gettime() - time;
		float interval = b->gettime() - a->gettime();
		float weight = t / interval;

		(*valueptr) = a->getvalue() + (b->getvalue() - a->getvalue()) * weight;
	}

}