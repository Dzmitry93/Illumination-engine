#ifndef __D3DBUFFER_FACTORY_H__
#define __D3DBUFFER_FACTORY_H__

#include "IntrusivePtr.h"
#include "DX11ResourceBuffer.h"

using OGE::memory::IntrusivePtr;
using namespace OGE::resource;

namespace OGE
{
	namespace render
	{
		enum BUFFER_TYPE
		{
			BUFFER_TYPE_VERTEX_BUFFER,
			BUFFER_TYPE_INDEX_BUFFER,
			BUFFER_TYPE_CBUFFER,
			BUFFER_TYPE_STRUCTURED_BUFFER
		};

		class DX11BufferFactory
		{
		public:
			
			// create directx buffers methods
			static DX11ResourceBuffer* createIndexBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE usage,
				const void* pData,
				uint32 width);

			static DX11ResourceBuffer* createVertexBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE usage,
				BUFFER_FORMAT format,
				const void* pData,
				uint32 width);

			static DX11ResourceBuffer* createConstantBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE usage,
				const void* pData,
				uint32 width);

			static DX11ResourceBuffer* createStructuredBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE usage,
				BUFFER_FORMAT format,
				const void* pData,
				uint32 width,
				uint32 stride,
				uint32 misc);

			// create directx buffer from existing buffer but with new resource usage
			static DX11ResourceBuffer* createStagingBuffer(
				ID3D11Device* pDevice,
				BUFFER_FORMAT format,
				uint32 width,
				uint8 stride,
				uint32 misc);

			static D3D11_BUFFER_DESC createBufferDesc(
				BUFFER_TYPE bufferType,
				D3D11_USAGE usage,
				BUFFER_FORMAT format,
				uint32 width,
				uint8 stride,
				uint32 misc);
		};
	}
}

#endif
