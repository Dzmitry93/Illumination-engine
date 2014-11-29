#include "DX11BlendFactory.h"

namespace OGE
{
	namespace render
	{
		BlendState DX11BlendFactory::createLightingBlendState(ID3D11Device* pDevice)
		{
			BlendState state;
			ID3D11BlendState* pBlendState = nullptr;

			D3D11_BLEND_DESC blendDesc;

			ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
			blendDesc.AlphaToCoverageEnable = false;
			blendDesc.IndependentBlendEnable = false;

			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL		 ;

			pDevice->CreateBlendState(&blendDesc, &pBlendState);
			debug_error(pBlendState != nullptr, "cannot create blend state for lighting pass");

			state.pBlendState = pBlendState;
			state.blendFactor = nullptr;
			state.SampleMask = 0xffffffff;

			return state;
		}
	}
}