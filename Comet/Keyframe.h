#pragma once

namespace Comet
{

	template<typename T>
	class Keyframe
	{
	public:
		Keyframe(){}
		Keyframe(const float &t, const T &val)
		{
			time = t;
			value = val;
		}
		~Keyframe(){}

		bool operator<(const Keyframe<T> &other) const
		{
			return (time < other.time);
		}

		float gettime() const { return time; }
		float getvalue() const { return value; }
	private:
		float time;
		T value;
	};
	/*
	template<typename T>
	bool operator<(const Keyframe<T> &a, const Keyframe<T> &b)
	{
		return (a.gettime() < b.gettime());
	}
	*/
}