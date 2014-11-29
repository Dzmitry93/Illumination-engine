#ifndef __DX11TEXTURE_FACTORY_H__
#define __DX11TEXTURE_FACTORY_H__

#include "DX11Texture2D.h"
#include "DX11Format.h"
#include "Texture2D.h"
#include "HashUtil.h"
#include "GUIDUtil.h"

#include <string> 
#include <D3DX11.h>

using std::string;
using OGE::resource::Texture2D;
using namespace OGE::util;

namespace OGE
{
	namespace render
	{
		class DX11TextureFactory
		{
		public:
			
			static DX11Texture2D* createTexture2D(
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
				uint32 misc = 0,
				uint32 mipLevels = 1);

			static DX11Texture2D* createRenderTargetTexture(
				ID3D11Device* pDevice,
				uint32 width,
				uint32 height,
				DXGI_FORMAT format);

			static DX11Texture2D* createTexture2D(
				ID3D11Device* pDevice,
				Texture2D* pSrcTexture, 
				uint32 mipLevels = 1);

			static DX11Texture2D* createCubemap(
				ID3D11Device* pDevice,
				uint32 width,
				uint32 height,
				uint32 bindFlags,
				DXGI_FORMAT format,
				D3D11_USAGE usage);

		private :

			static D3D11_TEXTURE2D_DESC createTexture2DDesc(
				uint32 width,
				uint32 height,
				uint32 bindFlags,
				DXGI_FORMAT format,
				D3D11_USAGE usage,
				uint32 arraySize,
				uint32 sampleCout, 
				uint32 sampleQuality,
				uint32 misc,
				uint32 mipLevels = 1);
		};
	}
}

#endif