#pragma once
#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "Buffer.h"

namespace OGE
{
	namespace resource
	{
		class VertexBuffer : public Buffer
		{
		public:
			VertexBuffer(const void* pData, uint32 bufferSize, uint32 stride = 0, BUFFER_FORMAT vertexFormat = FORMAT_UNKNOW);
			VertexBuffer(uint32 bufferSize, uint32 stride = 0, BUFFER_FORMAT vertexFormat = FORMAT_UNKNOW);
			VertexBuffer(const VertexBuffer& rBuffer);
			~VertexBuffer();

			BUFFER_FORMAT getFormat() const;

			void operator = (const VertexBuffer& rBuffer);

		private:
			BUFFER_FORMAT vertexFormat;

			void shallowCopy(const VertexBuffer& rBuffer);
		};

		inline VertexBuffer::VertexBuffer(const void* pData, uint32 bufferSize, uint32 stride, BUFFER_FORMAT vertexFormat)
			: Buffer(pData, bufferSize, stride), vertexFormat(vertexFormat)
		{
		}

		inline VertexBuffer::VertexBuffer(uint32 bufferSize, uint32 stride, BUFFER_FORMAT vertexFormat)
			: Buffer(bufferSize, stride), vertexFormat(vertexFormat)
		{
		}

		inline VertexBuffer::VertexBuffer(const VertexBuffer& rBuffer)
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline VertexBuffer::~VertexBuffer() 
		{
			destroyObject();
		}

		inline void VertexBuffer::operator= (const VertexBuffer& rBuffer) 
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline BUFFER_FORMAT VertexBuffer::getFormat() const
		{
			return vertexFormat;
		}

		inline void VertexBuffer::shallowCopy(const VertexBuffer& rBuffer)
		{
			Buffer::shallowCopy(rBuffer);
			this->vertexFormat = rBuffer.vertexFormat;
		}
	}
}

#endif