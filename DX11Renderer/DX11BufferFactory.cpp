#include "DX11BufferFactory.h"

namespace OGE
{
	namespace render
	{
		DX11ResourceBuffer* DX11BufferFactory::createIndexBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE usage,
				const void* pData,
				uint32 width)
		{
			
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA initialData;
			ID3D11Buffer* pResource = NULL;
			DX11ResourceBuffer* pBuffer = NULL;

			if(pData == NULL)
			{
				error_msg("indexes initial data cannot be a null");
				return NULL;
			}

			bufferDesc = createBufferDesc(
				BUFFER_TYPE_INDEX_BUFFER,
				usage,
				FORMAT_UNKNOW,
				width,
				sizeof(uint32),
				0);

			initialData.pSysMem = pData;

			HRESULT result = pDevice->CreateBuffer(&bufferDesc, &initialData, &pResource);

			if(FAILED(result))
			{
				error_msg("cannot create index buffer");
				return NULL;
			}

			pBuffer = new DX11ResourceBuffer(pResource, INDEX_32);

			return pBuffer;
		}

		DX11ResourceBuffer* DX11BufferFactory::createVertexBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE resourceUsage,
				BUFFER_FORMAT format,
				const void* pData,
				uint32 width)
		{
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA initialData;
			ID3D11Buffer* pResource = NULL;
			DX11ResourceBuffer* pBuffer = NULL;

			if(pData == NULL)
			{
				error_msg("vertices initial data cannot be a null");
				return NULL;
			}

			bufferDesc = createBufferDesc(
				BUFFER_TYPE_VERTEX_BUFFER,
				resourceUsage,
				format,
				width,
				Format::getSize(format), 0);

			initialData.pSysMem = pData;

			HRESULT result = pDevice->CreateBuffer(&bufferDesc, &initialData, &pResource);
			if(FAILED(result))
			{
				error_msg("cannot create vertex buffer");
			}

			pBuffer = new DX11ResourceBuffer(pResource, format);

			return pBuffer;
		}

		DX11ResourceBuffer* DX11BufferFactory::createConstantBuffer(
				ID3D11Device* pDevice,
				D3D11_USAGE resourceUsage,
				const void* pData,
				uint32 width)
		{
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA initialData;
			ID3D11Buffer* pResource = NULL;
			DX11ResourceBuffer* pBuffer = NULL;
			HRESULT result = S_OK;

			bufferDesc = createBufferDesc(
				BUFFER_TYPE_CBUFFER,
				resourceUsage,
				FORMAT_UNKNOW,
				width,
				0, 0);

			if(pData != NULL)
			{
				ZeroMemory(&initialData, sizeof(D3D11_SUBRESOURCE_DATA));
				initialData.pSysMem = pData;
				result = pDevice->CreateBuffer(&bufferDesc, &initialData, &pResource);
			}
			else
			{
				result = pDevice->CreateBuffer(&bufferDesc, NULL, &pResource);
			}

			if(FAILED(result))
			{
				error_msg("cannot create constant buffer");
			}

			pBuffer = new DX11ResourceBuffer(pResource, FORMAT_UNKNOW);

			return pBuffer;
		}

		DX11ResourceBuffer* DX11BufferFactory::createStructuredBuffer(
			ID3D11Device* pDevice,
			D3D11_USAGE resourceUsage,
			BUFFER_FORMAT format,
			const void* pData,
			uint32 width,
			uint32 stride,
			uint32 misc)
		{
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA initialData;
			ID3D11Buffer* pResource = NULL;
			DX11ResourceBuffer* pBuffer = NULL;

			if(pData == NULL)
			{
				error_msg("structures buffer initial data cannot be a null");
				return NULL;
			}

			bufferDesc = createBufferDesc(
				BUFFER_TYPE_STRUCTURED_BUFFER,
				resourceUsage,
				format,
				width,
				stride,
				misc);

			initialData.pSysMem = pData;

			HRESULT result = pDevice->CreateBuffer(&bufferDesc, &initialData, &pResource);
			if(FAILED(result))
			{
				error_msg("cannot create structure buffer");
				return NULL;
			}

			pBuffer = new DX11ResourceBuffer(pResource, format);

			return pBuffer;
		}

		D3D11_BUFFER_DESC DX11BufferFactory::createBufferDesc(
				BUFFER_TYPE bufferType,
				D3D11_USAGE usage,
				BUFFER_FORMAT format,
				uint32 width,
				uint8 stride,
				uint32 misc)
		{
			D3D11_BUFFER_DESC desc;
			
			ZeroMemory(&desc, sizeof(desc));

			if(usage == D3D11_USAGE_DEFAULT)
			{
				desc.CPUAccessFlags = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
			}
			if(usage == D3D11_USAGE_DYNAMIC)
			{
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				desc.Usage = D3D11_USAGE_DYNAMIC;
			}
			if(usage == D3D11_USAGE_STAGING)
			{
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
				desc.Usage = D3D11_USAGE_STAGING;
			}
			if(usage == D3D11_USAGE_IMMUTABLE)
			{
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}

			if(!(usage == D3D11_USAGE_STAGING))
			{
				switch(bufferType)
				{
					case BUFFER_TYPE::BUFFER_TYPE_STRUCTURED_BUFFER : 
						if(misc & D3D11_BIND_SHADER_RESOURCE)
						{
							desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
						}
						if(misc & D3D11_BIND_UNORDERED_ACCESS)
						{
							desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS; 
						}

						desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
						desc.StructureByteStride = stride; break;

					case BUFFER_TYPE::BUFFER_TYPE_VERTEX_BUFFER :
						desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
				
					case BUFFER_TYPE::BUFFER_TYPE_INDEX_BUFFER :
						desc.BindFlags = D3D11_BIND_INDEX_BUFFER; break;

					case BUFFER_TYPE::BUFFER_TYPE_CBUFFER :
						desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
				};
			}

			desc.ByteWidth = width;

			return desc;
		}


		DX11ResourceBuffer* DX11BufferFactory::createStagingBuffer(
				ID3D11Device* pDevice,
				BUFFER_FORMAT format,
				uint32 width,
				uint8 stride,
				uint32 misc)
		{
			DX11ResourceBuffer* pBuffer= createStructuredBuffer(
				pDevice,
				D3D11_USAGE_STAGING,
				format,
				NULL, 
				width,
				stride,
				misc);
				
			return pBuffer;
		}
	}
}