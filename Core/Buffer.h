#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Format.h"
#include "ReferenceCounter.h"

using OGE::memory::ReferenceCounter;

namespace OGE
{
	namespace resource
	{
		class Buffer : public ReferenceCounter<Buffer>
		{
		public:
			Buffer(const void* pData, uint32 bufferSize, uint32 stride = 0);
			Buffer(uint32 bufferSize, uint32 stride = 0);
			Buffer(const Buffer& rOther);
			Buffer() {};
			~Buffer();

			void readData(void** pDestBuffer, uint32 sizeToRead, uint32 offsetToRead = 0) const;
			void writeData(const void* pSrcData, uint32 sizeToWrite, uint32 offsetToWrite = 0);

			void copy(Buffer* pOther) const;
			
			uint32 getSize() const;
			uint32 getStride() const;

			const void* getPointer() const;
			bool isNULL() const;

			void operator = (const Buffer& rOther);

		protected:
			void* pBuffer;

			uint32 bufferSize;
			uint32 stride;

			void destroyObject();
			void shallowCopy(const Buffer& rBuffer);
		};

		inline Buffer::Buffer(const void* pData, uint32 bufferSize, uint32 stride)
			: bufferSize(bufferSize), stride(stride)
		{
			this->pBuffer = new uint8[bufferSize];
			writeData(pData, bufferSize);
		}

		inline Buffer::Buffer(uint32 bufferSize, uint32 stride) :
			bufferSize(bufferSize), stride(stride)
		{
			if(bufferSize > 0)
			{
				this->pBuffer = new uint8[bufferSize];
			}
			else
			{
				this->pBuffer = NULL;
			}
		}

		inline Buffer::Buffer(const Buffer& rBuffer) 
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline Buffer::~Buffer() { release(); }

		inline void Buffer::destroyObject() 
		{
			safe_release(pBuffer);
		}

		inline void Buffer::operator= (const Buffer& rBuffer) 
		{
			shallowCopy(rBuffer);
			addRef();
		}

		inline uint32 Buffer::getSize() const
		{
			return bufferSize;
		}

		inline uint32 Buffer::getStride() const
		{
			return stride;
		}

		inline const void* Buffer::getPointer() const
		{
			return pBuffer;
		}

		inline bool Buffer::isNULL() const
		{
			return pBuffer == NULL;
		}

		inline void Buffer::readData(void** pDestBuffer, uint32 sizeToRead, uint32 offsetToRead) const
		{

			debug_assert(sizeToRead > 0, "incorrect size to read");
			debug_assert((sizeToRead + offsetToRead) <= bufferSize, "incorrect size to read or offset");
			debug_assert(pDestBuffer != NULL, "incorrect pointer on dest buffer");
			debug_assert(pBuffer != NULL, "dest buffer is null");

			memcpy(*pDestBuffer, (uint8*)pBuffer + offsetToRead, sizeToRead);
		}

		inline void Buffer::writeData(const void* pSrcData, uint32 sizeToWrite, uint32 offsetToWrite)
		{

			debug_assert(sizeToWrite > 0, "incorrect size to read");
			debug_assert((sizeToWrite + offsetToWrite) <= bufferSize, "incorrect size src buffer");
			debug_assert(pSrcData != NULL, "incorrect pointer on src data");
			debug_assert(pBuffer != NULL, "dest buffer is null");

			memcpy((char*)pBuffer + offsetToWrite, pSrcData, sizeToWrite);
		}

		inline void Buffer::copy(Buffer* pOther) const
		{
			debug_assert(pBuffer != NULL, "");
			debug_assert(pOther != NULL, "");

			readData(&pOther->pBuffer, bufferSize);
		}

		inline void Buffer::shallowCopy(const Buffer& rBuffer)
		{
			this->bufferSize = rBuffer.bufferSize;
			this->stride = rBuffer.stride;
			this->pCountReferences = rBuffer.pCountReferences;
			this->pBuffer = rBuffer.pBuffer;
		}
	}
}

#endif