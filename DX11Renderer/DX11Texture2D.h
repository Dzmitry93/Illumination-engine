#ifndef __DX11TEXTURE2D_H__
#define __DX11TEXTURE2D_H__

#include "DX11Format.h"
#include "DX11RenderResource.h"
#include <d3d11.h>

namespace OGE
{
	namespace render
	{
		class DX11Texture2D : public DX11RenderResource
		{
		public:
			DX11Texture2D(ID3D11Texture2D* pTexture);
			virtual ~DX11Texture2D();

			uint32 getWidth() const;
			uint32 getHeight() const;

			void updateData(ID3D11DeviceContext* pContext, void* pData, uint32 size);
			void writeData(ID3D11DeviceContext* pContext, void* pData, uint32 size);
			void copy(ID3D11DeviceContext* pContext, DX11Texture2D* pTexture);

			bool desc(D3D11_TEXTURE2D_DESC* pDesc) const;
			ID3D11Texture2D* getPointer() const;

			ID3D11ShaderResourceView* getTexture2DSRV(ID3D11Device* pDevice);
			ID3D11ShaderResourceView* getCubemapSRV(ID3D11Device* pDevice);
			ID3D11UnorderedAccessView* getTexture2DUAV(ID3D11Device* pDevice);
			
			bool isShaderResource() const; 
			bool isCubemap() const;
			bool isUnorderedAccess() const;
			bool isNULL() const;

			uint32 getArraySize() const;

			void release();

		private:
			ID3D11Texture2D* pTexture;
			ID3D11ShaderResourceView* pSRV;
			ID3D11ShaderResourceView* pCubeSRV;
			ID3D11UnorderedAccessView* pUAV;

			ID3D11ShaderResourceView* createTexture2DSRV(ID3D11Device* pDevice);
			ID3D11UnorderedAccessView* createTexture2DUAV(ID3D11Device* pDevice);
			ID3D11ShaderResourceView* createTexture2DArraySRV(ID3D11Device* pDevice);
		};

		inline DX11Texture2D::DX11Texture2D(ID3D11Texture2D* pTexture)
		{
			this->pTexture  = pTexture;
		}

		inline DX11Texture2D::~DX11Texture2D()
		{
			release();
		}	

		inline void DX11Texture2D::release()
		{
			safe_dx_resource_release(pTexture);
			safe_dx_resource_release(pSRV);
			safe_dx_resource_release(pUAV);
		}

		inline ID3D11Texture2D* DX11Texture2D::getPointer() const
		{
			return pTexture;
		}

		inline uint32 DX11Texture2D::getWidth() const
		{
			D3D11_TEXTURE2D_DESC* pDesc = nullptr;
			pTexture->GetDesc(pDesc);
			return pDesc->Width;
		}

		inline uint32 DX11Texture2D::getHeight() const
		{
			D3D11_TEXTURE2D_DESC* pDesc = nullptr;
			pTexture->GetDesc(pDesc);
			return pDesc->Height;
		}

		inline bool DX11Texture2D::desc(D3D11_TEXTURE2D_DESC* pDesc) const
		{
			if(pTexture != nullptr)
			{
				pTexture->GetDesc(pDesc);
				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool DX11Texture2D::isShaderResource() const
		{
			D3D11_TEXTURE2D_DESC* pDesc = nullptr;
			pTexture->GetDesc(pDesc);
			return (pDesc->BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0;
		}
		
		//inline bool DX11Texture2D::is

		inline bool DX11Texture2D::isUnorderedAccess() const
		{
			D3D11_TEXTURE2D_DESC* pDesc = nullptr;
			pTexture->GetDesc(pDesc);
			return (pDesc->BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0;
		}

		inline uint32 DX11Texture2D::getArraySize() const
		{
			D3D11_TEXTURE2D_DESC* pDesc = nullptr;
			pTexture->GetDesc(pDesc);
			return (pDesc->ArraySize);
		}

		inline bool DX11Texture2D::isNULL() const
		{
			return pTexture == nullptr;
		}

		inline ID3D11ShaderResourceView* DX11Texture2D::getTexture2DSRV(ID3D11Device* pDevice)
		{
			if(pSRV == nullptr)
			{
				pSRV = createTexture2DSRV(pDevice);
			}
			return pSRV;
		}

		inline ID3D11ShaderResourceView* DX11Texture2D::getCubemapSRV(ID3D11Device* pDevice)
		{
			if(pCubeSRV == nullptr)
			{
				pCubeSRV = createTexture2DArraySRV(pDevice);
			}
			return pCubeSRV;
		}

		inline ID3D11UnorderedAccessView* DX11Texture2D::getTexture2DUAV(ID3D11Device* pDevice)
		{
			if(pUAV == nullptr)
			{
				pUAV = createTexture2DUAV(pDevice);
			}
			return pUAV;
		}
	}
}

#endif 

