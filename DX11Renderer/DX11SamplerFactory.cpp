#include "DX11SamplerFactory.h"

namespace OGE
{
	namespace render
	{
		ID3D11SamplerState* DX11SamplerFactory::createDefaultSamplerState(ID3D11Device* pDevice, uint8 maxAnisotropy)
		{
			ID3D11SamplerState* pSamplerState = NULL;
			D3D11_SAMPLER_DESC samplerDesc;

			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MaxAnisotropy = maxAnisotropy;
			samplerDesc.MinLOD = -FLT_MAX;
			samplerDesc.MaxLOD = FLT_MAX;
			samplerDesc.MipLODBias = 0.0f;

			pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);

			return pSamplerState;
		}
	}
}