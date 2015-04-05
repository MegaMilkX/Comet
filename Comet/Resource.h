#pragma once

#include <string>

#include <thread>
#include <mutex>

namespace Comet
{

	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		virtual void Load(std::string path) = 0;
		virtual void Unload();

		bool IsReady(){ return isReady; }
		void IsReady(bool val){ isReady = val; }
	protected:
		bool isReady;
		std::mutex readyMutex;
	};

};