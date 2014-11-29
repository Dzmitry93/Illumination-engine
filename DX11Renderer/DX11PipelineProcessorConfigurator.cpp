#include "DX11PipelineProcessorConfigurator.h"

namespace OGE
{
	namespace render
	{
		DX11AbstractPipelineProcessor*  DX11PipelineProcessorConfigurator::createPipelineProcessor(
				PIPELINE_PROCESSOR processor, 
				ID3D11Device* pDevice, 
				ID3D11DeviceContext* pDeviceContext, 
				IDXGISwapChain* pSwapChain, 
				ID3D11RenderTargetView* pRenderTargetView,
				ID3D11DepthStencilView* pDepthStencilView,
				DXGI_MODE_DESC& rCurrentMode)
		{
			DX11AbstractPipelineProcessor* pProcessor = NULL;

			switch(processor)
			{
				case RASTERIZER_PIPELINE_PROCESSOR : pProcessor = new DX11RasterizerProcessor(
														 pDevice, pDeviceContext, 
														 pSwapChain, pRenderTargetView,
														 pDepthStencilView,
														 rCurrentMode); break;

				case RAYTRACING_PIPELINE_PROCESSOR : pProcessor = new DX11RaytracingProcessor(
														 pDevice, pDeviceContext,
														 pSwapChain, pRenderTargetView,
														 pDepthStencilView,
														 rCurrentMode); break;
			};

			debug_assert(pProcessor != NULL, "pipeline processor is NULL");
			return pProcessor;
		}
	}
}
