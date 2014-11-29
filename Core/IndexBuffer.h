#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "Buffer.h"

namespace OGE
{
	namespace resource
	{
		class IndexBuffer : public Buffer
		{
		public:
			IndexBuffer(const void* pData, uint32 bufferSize, uint32 stride = 0, BUFFER_FORMAT indexFormat = FORMAT_UNKNOW);
			IndexBuffer(uint32 bufferSize, uint32 stride = 0, BUFFER_FORMAT indexFormat = FORMAT_UNKNOW);
			IndexBuffer(const IndexBuffer& rBuffer);
			~IndexBuffer();

			BUFFER_FORMAT getIndexFormat() const;

			void operator = (const IndexBuffer& rBuffer);

		private: 

			BUFFER_FORMAT indexFormat;
			void shallowCopy(const IndexBuffer& rBuffer);
		};

		inline IndexBuffer::IndexBuffer(const void* pData, uint32 bufferSize, uint32 stride, BUFFER_FORMAT indexFormat)
			: Buffer(pData, bufferSize, stride), indexFormat(indexFormat)
		{
		}

		inline IndexBuffer::IndexBuffer(uint32 bufferSize, uint32 stride, BUFFER_FORMAT indexFormat)
			: Buffer(bufferSize, stride), indexFormat(indexFormat)
		{
		}

		inline IndexBuffer::IndexBuffer(const IndexBuffer& rBuffer)
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline IndexBuffer::~IndexBuffer() 
		{
			destroyObject();
		}

		inline void IndexBuffer::operator= (const IndexBuffer& rBuffer) 
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline BUFFER_FORMAT IndexBuffer::getIndexFormat() const
		{
			return indexFormat;
		}

		inline void IndexBuffer::shallowCopy(const IndexBuffer& rBuffer)
		{
			Buffer::shallowCopy(rBuffer);
			this->indexFormat = rBuffer.indexFormat;
		}
	}
}

#endif