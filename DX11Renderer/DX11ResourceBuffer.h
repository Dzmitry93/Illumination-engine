#ifndef __D3DRESOURCE_BUFFER_H__
#define __D3DRESOURCE_BUFFER_H__

/*
======================================================================

	This file involves class that consists general functionality of 
	buffers so as VertexBuffer and IndexBuffer

======================================================================
*/ 

#include "DX11RenderResource.h"
#include "DX11Format.h"
#include <D3D11.h>

namespace OGE
{
	namespace render
	{
		class DX11ResourceBuffer : public DX11RenderResource
		{
		public:
			DX11ResourceBuffer(ID3D11Buffer* pBuffer, BUFFER_FORMAT vertexFormat);
			~DX11ResourceBuffer();

			void readData(ID3D11DeviceContext* pContext, uint32 offsetToRead, uint32 sizeToRead, void** pDestBuffer);
			void updateData(ID3D11DeviceContext* pContext, void* pData, uint32 size);
			void writeData(ID3D11DeviceContext* pContext, void* pData, uint32 size);
			void copy(ID3D11DeviceContext* pContext, DX11ResourceBuffer* pBuffer);

			ID3D11ShaderResourceView* createSRV(ID3D11Device* pDevice);
			ID3D11UnorderedAccessView* createUAV(ID3D11Device* pDevice);

			bool isShaderResource() const;
			bool isUnorderedAcces() const;
			bool isEmpty() const;

			D3D11_USAGE getResourceUsage() const;
			ID3D11Buffer* D3DBuffer() const;

			uint32 getSize() const;
			uint32 getStride() const;

			BUFFER_FORMAT getFormat() const;

			void release();

		protected:
			ID3D11Buffer* pBuffer;
			D3D11_BUFFER_DESC desc;

			BUFFER_FORMAT vertexFormat;
		};

		inline DX11ResourceBuffer::DX11ResourceBuffer(ID3D11Buffer* pBuffer, BUFFER_FORMAT vertexFormat) : 
					pBuffer(pBuffer), vertexFormat(vertexFormat) 
		{
			if(pBuffer != NULL)
			{
				pBuffer->GetDesc(&desc);
			}
		}

		inline DX11ResourceBuffer::~DX11ResourceBuffer() 
		{
			release();
		}

		inline ID3D11Buffer* DX11ResourceBuffer::D3DBuffer() const
		{
			return pBuffer;
		}

		inline D3D11_USAGE DX11ResourceBuffer::getResourceUsage() const
		{
			return desc.Usage;
		}

		inline bool DX11ResourceBuffer::isShaderResource() const
		{
			return (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0;
		}

		inline bool DX11ResourceBuffer::isUnorderedAcces() const
		{
			return (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0;
		}

		inline bool DX11ResourceBuffer::isEmpty() const
		{
			return pBuffer == NULL;
		}
		
		inline uint32 DX11ResourceBuffer::getSize() const
		{
			return desc.ByteWidth;
		}

		inline uint32 DX11ResourceBuffer::getStride() const
		{
			if(desc.StructureByteStride)
			{
				return desc.StructureByteStride;
			}
			else
			{
				return Format::getSize(vertexFormat);
			}
		}

		inline BUFFER_FORMAT DX11ResourceBuffer::getFormat() const
		{
			return vertexFormat;
		}

		inline void DX11ResourceBuffer::release()
		{
			if(pBuffer != NULL)
			{
				pBuffer->Release();
				pBuffer = NULL;
			}
		}
	}
}

#endif

