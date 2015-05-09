#include "GLBuffer.h"

namespace Comet
{

	GLBuffer::GLBuffer(BufferUsage usage) : bufferUsage(usage), bufferId(0)
	{
	}


	GLBuffer::~GLBuffer()
	{
		glDeleteBuffers(1, &bufferId);
	}

}