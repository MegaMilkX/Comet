#pragma once

#include <vector>

#ifndef GLEW_STATIC
#include <gl\glew.h>
#endif

namespace Comet
{

	class GLBuffer
	{
	public:
		enum BufferUsage
		{
			STATIC = GL_STATIC_DRAW,
			DYNAMIC = GL_DYNAMIC_DRAW,
			STREAM = GL_STREAM_DRAW
		};

		GLBuffer(BufferUsage usage = STATIC);
		~GLBuffer();

		unsigned int Id() const { return bufferId; }
		
		template <typename T>
		void Fill(const std::vector<T> &data)
		{
			if (bufferId)
				glDeleteBuffers(1, &bufferId);
			glGenBuffers(1, &bufferId);
			glBindBuffer(GL_ARRAY_BUFFER, bufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * (data.size()), &data[0], bufferUsage);
		}
	private:
		unsigned int bufferId;
		BufferUsage bufferUsage;
	};

}