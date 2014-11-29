#ifndef __SYSTEM_BUFFER_FACTORY_H__
#define __SYSTEM_BUFFER_FACTORY_H__

#include "Buffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace OGE
{
	namespace resource
	{
		class BufferFactory
		{
		public:
			
			static Buffer* createBuffer(
				const void* pData,
				uint32 bufferSize,
				uint32 stride);

			static VertexBuffer* createVertexBuffer(
				BUFFER_FORMAT format,
				const void* pData,
				uint32 bufferSize,
				uint32 stride);

			static IndexBuffer* createIndexBuffer(
				BUFFER_FORMAT format,
				const void* pData,
				uint32 bufferSize,
				uint32 stride);

			static Buffer* createBuffer(const Buffer* pBuffer);
			static VertexBuffer* createBuffer(const VertexBuffer* pBuffer);
			static IndexBuffer* createBuffer(const IndexBuffer* pBuffer);

		};

		inline Buffer* BufferFactory::createBuffer(
				const void* pData,
				uint32 bufferSize,
				uint32 stride)
		{
			return  new Buffer(pData,
				bufferSize,
				stride);
		}

		inline Buffer* BufferFactory::createBuffer(const Buffer* pBuffer)
		{
			return createBuffer(
				pBuffer->getPointer(), 
				pBuffer->getSize(), 
				pBuffer->getStride());
		}

		inline VertexBuffer* BufferFactory::createVertexBuffer(
				BUFFER_FORMAT format,
				const void* pData,
				uint32 bufferSize,
				uint32 stride)
		{
			return new VertexBuffer(pData, bufferSize, stride, format);
		}

		inline IndexBuffer* BufferFactory::createIndexBuffer(
				BUFFER_FORMAT format,
				const void* pData,
				uint32 bufferSize,
				uint32 stride)
		{
			return new IndexBuffer(pData, bufferSize, stride, format);
		}

		inline VertexBuffer* BufferFactory::createBuffer(const VertexBuffer* pBuffer)
		{
			return createVertexBuffer(
				pBuffer->getFormat(),
				pBuffer->getPointer(),
				pBuffer->getSize(),
				pBuffer->getStride());
		}

		inline IndexBuffer* BufferFactory::createBuffer(const IndexBuffer* pBuffer)
		{
			return createIndexBuffer(
				pBuffer->getIndexFormat(),
				pBuffer->getPointer(),
				pBuffer->getSize(),
				pBuffer->getStride());
		}
	}
}

#endif