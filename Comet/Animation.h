#pragma once

#include <vector>

#include "Component.h"

#include "AnimTrack.h"

namespace Comet
{

	class Animation : public Component
	{
	public:
		Animation();
		~Animation();

		template<typename T>
		AnimTrack<T>* CreateTrack(T* valueptr, std::string name = "empty")
		{
			AnimTrack<T>* track = new AnimTrack<T>(valueptr);
			tracks.push_back(track);
			return track;
		}

		void Update(const float &dt);
	private:
		std::vector<GenericAnimTrack*> tracks;
		float length;
		float time;
	};

}