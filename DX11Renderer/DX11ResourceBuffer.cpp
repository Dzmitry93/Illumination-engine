#include "DX11ResourceBuffer.h"

namespace OGE
{
	namespace render
	{
		void DX11ResourceBuffer::readData(ID3D11DeviceContext* pContext, uint32 offsetToRead, uint32 sizeToRead, void** pDestBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE data;

			if(sizeToRead == 0)
			{
				sizeToRead = desc.ByteWidth;
			}

			debug_assert(desc.Usage & D3D11_USAGE_STAGING, "Buffer is not staging");
			debug_assert((sizeToRead + offsetToRead) <= desc.ByteWidth, "incorrect size to read or offset");

			HRESULT result = pContext->Map(pBuffer, 0, D3D11_MAP_READ, 0, &data);

			debug_assert(!FAILED(result), "fail to map resource");

			memcpy(*pDestBuffer, data.pData, sizeToRead);

			pContext->Unmap(pBuffer, 0);	
		}

		void DX11ResourceBuffer::writeData(ID3D11DeviceContext* pContext, void* pSrcData, uint32 size)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData;
	
			debug_assert((desc.Usage & D3D11_USAGE_STAGING) || (desc.Usage & D3D11_USAGE_DYNAMIC), 
				"Buffer is not dynamic or staging");
			debug_assert(desc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE, "resource is not writable");
			debug_assert(size <= desc.ByteWidth && size > 0, "incorrect size src data");
			debug_assert(pSrcData != NULL, "incorrect pointer on src data");
			debug_assert(pBuffer != NULL, "dest buffer is null");
			D3D11_MAP mapType = D3D11_MAP::D3D11_MAP_WRITE;

			if(desc.Usage == D3D11_USAGE_DYNAMIC)
			{
				mapType = D3D11_MAP_WRITE_DISCARD;
			}

			HRESULT result = pContext->Map(pBuffer, 0, mapType, 
				0, &mappedData);

			debug_assert(!FAILED(result), "failed resource mapping");

			memcpy(mappedData.pData, pSrcData, size);
			pContext->Unmap(pBuffer, 0);
		}

		void DX11ResourceBuffer::updateData(ID3D11DeviceContext* pContext, void* pSrcData, uint32 size)
		{
			debug_assert(pSrcData != NULL && size > 0, "incorrect params for update buffer data");
			debug_assert(pBuffer != NULL, "dest buffer is null");

			pContext->UpdateSubresource(pBuffer, 0, NULL, pSrcData, size, 0);
		}

		void DX11ResourceBuffer::copy(ID3D11DeviceContext* pContext, DX11ResourceBuffer* pResourceBuffer)
		{
			ID3D11Buffer* pDstBuffer = pResourceBuffer->D3DBuffer();

			debug_assert(pBuffer != NULL, "src buffer is null");
			debug_assert(pDstBuffer != NULL, "dst buffer is null");	

			pContext->CopyResource(pDstBuffer, this->pBuffer);
		}

		ID3D11UnorderedAccessView* DX11ResourceBuffer::createUAV(ID3D11Device* pDevice)
		{
			ID3D11UnorderedAccessView* pUAV = NULL;
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;

			ZeroMemory(&uavDesc, sizeof(uavDesc));

			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
			
			pDevice->CreateUnorderedAccessView(pBuffer, &uavDesc, &pUAV);
			debug_assert(pUAV != NULL, "cannot create unordered access view");
			
			return pUAV;
		}

		ID3D11ShaderResourceView* DX11ResourceBuffer::createSRV(ID3D11Device* pDevice)
		{
			ID3D11ShaderResourceView* pSRV = NULL;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.ElementOffset= 0;
			srvDesc.Buffer.ElementWidth = desc.ByteWidth / desc.StructureByteStride;
			
			pDevice->CreateShaderResourceView(pBuffer, &srvDesc, &pSRV);
			debug_assert(pSRV != NULL, "cannot create shader resource view");

			return pSRV;
		}
	}
}