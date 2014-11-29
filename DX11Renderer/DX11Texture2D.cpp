#include "DX11Texture2D.h"

namespace OGE
{
	namespace render
	{

		void  DX11Texture2D::writeData(ID3D11DeviceContext* pContext, void* pData, uint32 size)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData;
			D3D11_TEXTURE2D_DESC desc;
			pTexture->GetDesc(&desc);

			debug_error((desc.Usage | D3D11_USAGE_STAGING) || (desc.Usage | D3D11_USAGE_DYNAMIC), 
				"Buffer is not dynamic or staging");
			debug_error(pData != NULL, "incorrect pointer on data");
			debug_error(pTexture != NULL, "dest buffer is null");

			HRESULT result = pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 
				0, &mappedData);

			debug_error(!FAILED(result), "fail to map resource");

			memcpy(mappedData.pData, pData, size);
			pContext->Unmap(pTexture, 0);
		}

		void  DX11Texture2D::updateData(ID3D11DeviceContext* pContext, void* pData, uint32 rowPitch)
		{
			debug_error(pData != NULL && rowPitch > 0, "incorrect params for update buffer data");
			debug_error(pTexture != NULL, "dest buffer is null");

			pContext->UpdateSubresource(pTexture, 0, NULL, pData, rowPitch, 0);
		}

		void DX11Texture2D::copy(ID3D11DeviceContext* pContext, DX11Texture2D* pDstTexture)
		{
			ID3D11Texture2D* pDstBuffer = pDstTexture->getPointer();
			D3D11_TEXTURE2D_DESC srcTextureDesc;
			D3D11_TEXTURE2D_DESC dstTextureDesc;

			desc(&srcTextureDesc);
			pDstTexture->desc(&dstTextureDesc);

			debug_error(pTexture != NULL, "src buffer is null");
			debug_error(pDstBuffer != NULL, "dst buffer is null");	
//			debug_error(srcTextureDesc.Format == dstTextureDesc.Format, "incompatable formats");
			debug_error(srcTextureDesc.Width == dstTextureDesc.Width &&
				srcTextureDesc.Height == dstTextureDesc.Height, "incompatable size");

			pContext->CopyResource(pDstBuffer, this->pTexture);
		}

		ID3D11ShaderResourceView* DX11Texture2D::createTexture2DSRV(ID3D11Device* pDevice) 
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			pTexture->GetDesc(&textureDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;	
			ID3D11ShaderResourceView* pSRV = NULL;
			D3D11_TEX2D_SRV text2DSrv;
			text2DSrv.MipLevels = textureDesc.MipLevels;
			text2DSrv.MostDetailedMip = 0;

			desc.Format = textureDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D = text2DSrv;

			HRESULT result = pDevice->CreateShaderResourceView(pTexture, &desc, &pSRV);

			debug_error(!FAILED(result), "fail to create texture srv");
			return pSRV;
			
		}

		ID3D11UnorderedAccessView* DX11Texture2D::createTexture2DUAV(ID3D11Device* pDevice)
		{
			ID3D11UnorderedAccessView* pUAV = NULL;

			D3D11_TEXTURE2D_DESC textureDesc;
			pTexture->GetDesc(&textureDesc);

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			uavDesc.Format = textureDesc.Format;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = 0;

			HRESULT result = pDevice->CreateUnorderedAccessView(pTexture, &uavDesc, &pUAV);

			debug_error(!FAILED(result), "fail to create texture uav");

			return pUAV;
		}

		ID3D11ShaderResourceView* DX11Texture2D::createTexture2DArraySRV(ID3D11Device* pDevice)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			pTexture->GetDesc(&textureDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;	
			ID3D11ShaderResourceView* pSRV = NULL;
			D3D11_TEX2D_SRV text2DSrv;

			text2DSrv.MipLevels = textureDesc.MipLevels;
			text2DSrv.MostDetailedMip = 0;

			desc.Format = textureDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE;
			desc.Texture2D = text2DSrv;
	
			HRESULT result = pDevice->CreateShaderResourceView(pTexture, &desc, &pSRV);

			debug_error(!FAILED(result), "Cannot create shader resource view for 2d texture array");

			return pSRV;
		}
	}
}