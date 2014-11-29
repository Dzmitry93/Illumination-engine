#include "DX11TextureFactory.h"
#include <fstream>

using namespace std;

namespace OGE
{
	namespace render
	{
		D3D11_TEXTURE2D_DESC DX11TextureFactory::createTexture2DDesc(
				uint32 width,
				uint32 height,
				uint32 bindFlags,
				DXGI_FORMAT format,
				D3D11_USAGE usage,
				uint32 arraySize,
				uint32 sampleCout, 
				uint32 sampleQuality,
				uint32 misc,
				uint32 mipLevels)
		{
			D3D11_TEXTURE2D_DESC textureDesc;

			// create texture desc
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.ArraySize = arraySize;
			textureDesc.Format = format;
			textureDesc.ArraySize = arraySize;
			textureDesc.MipLevels = mipLevels;
			textureDesc.SampleDesc.Count = sampleCout;
			textureDesc.SampleDesc.Quality = sampleQuality;
			textureDesc.BindFlags |= bindFlags;
			textureDesc.MiscFlags = misc;

			if(usage == D3D11_USAGE_DEFAULT)
			{
				textureDesc.Usage = D3D11_USAGE_DEFAULT; 
				textureDesc.CPUAccessFlags = 0;
			}
			if(usage == D3D11_USAGE_DYNAMIC)
			{
				textureDesc.Usage = D3D11_USAGE_DYNAMIC; 
				textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			}
			if(usage == D3D11_USAGE_IMMUTABLE)
			{
				textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
				textureDesc.CPUAccessFlags = 0;
			}
			if(usage == D3D11_USAGE_STAGING)
			{
				textureDesc.Usage = D3D11_USAGE_STAGING; 
				textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}

			return textureDesc;
		}

		DX11Texture2D* DX11TextureFactory::createCubemap(
				ID3D11Device* pDevice,
				uint32 width,
				uint32 height,
				uint32 bindFlags,
				DXGI_FORMAT format,
				D3D11_USAGE usage)
		{
			return createTexture2D(
				pDevice,
				width,
				height,
				bindFlags,
				format,
				usage,
				6, 1, 0, nullptr, D3D11_RESOURCE_MISC_TEXTURECUBE);
		}

		DX11Texture2D* DX11TextureFactory::createTexture2D(
				ID3D11Device* pDevice,
				uint32 width,
				uint32 height,
				uint32 bindFlags,
				DXGI_FORMAT format,
				D3D11_USAGE usage,
				uint32 arraySize,
				uint32 sampleCout, 
				uint32 sampleQuality,
				void* pTexture,
				uint32 misc,
				uint32 mipLevels)
		{
			D3D11_SUBRESOURCE_DATA* pTextureData = new D3D11_SUBRESOURCE_DATA[arraySize];
			ID3D11Texture2D* pDX11Texture = NULL;
			D3D11_TEXTURE2D_DESC textureDesc;
			string textureName = "texture2D" + GUIDUtil::generateRandomGUID();
			uint64 guid = HashUtil::getHash(textureName);

			textureDesc = createTexture2DDesc(
				width,
				height,
				bindFlags,
				format,
				usage,
				arraySize,
				sampleCout,
				sampleQuality, 
				misc,
				mipLevels);

				// set texture data
			for(uint32 i = 0; i < arraySize; i++)
			{
				pTextureData[i].pSysMem = pTexture;
				pTextureData[i].SysMemPitch = width * 4;
				pTextureData[i].SysMemSlicePitch = width * height * 4;
			}
			
			HRESULT result;
			if(pTexture != NULL)
			{
				result = pDevice->CreateTexture2D(&textureDesc, pTextureData, &pDX11Texture);
			}
			else
			{
				result = pDevice->CreateTexture2D(&textureDesc, NULL, &pDX11Texture);
			}

			delete[] pTextureData;
			
			debug_assert(!FAILED(result),"cannot create texture 2D");

			DX11Texture2D* pRenderTexture2D = new DX11Texture2D(pDX11Texture);
			pRenderTexture2D->setGUID(guid);
			pRenderTexture2D->setResourceName(textureName);

			return pRenderTexture2D;
		}

		DX11Texture2D* DX11TextureFactory::createRenderTargetTexture(
				ID3D11Device* pDevice,
				uint32 width,
				uint32 height,
				DXGI_FORMAT format)
		{
			return createTexture2D(pDevice, 
					width,
					height,
					D3D11_BIND_RENDER_TARGET,
					format,
					D3D11_USAGE_DEFAULT,
					1, 1, 0, nullptr); 
		}

		DX11Texture2D* DX11TextureFactory::createTexture2D(
				ID3D11Device* pDevice,
				Texture2D* pSrcTexture,
				uint32 mipLevels)
		{
			DX11Texture2D* pDX11Texture = NULL;
			ID3D11Resource* pDXTexture2D = NULL;

			HRESULT result = D3DX11CreateTextureFromFileA(pDevice, 
					pSrcTexture->getResourcePath().c_str(),
					NULL, NULL, &pDXTexture2D, NULL); 
		
			debug_assert(!FAILED(result), "cannot create texture");
			pDX11Texture = new DX11Texture2D((ID3D11Texture2D*)pDXTexture2D);
			pDX11Texture->setGUID(pSrcTexture->getGUID());
			pDX11Texture->setResourceName(pSrcTexture->getResourceName());

			return pDX11Texture;
		}
	}
}