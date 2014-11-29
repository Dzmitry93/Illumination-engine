#ifndef __DX11PIPELINE_PROCESSOR_CONFIGURATOR_H__
#define __DX11PIPELINE_PROCESSOR_CONFIGURATOR_H__

#include "DX11RasterizerProcessor.h"
#include "DX11RaytracingProcessor.h"

namespace OGE
{
	namespace render
	{
		enum PIPELINE_PROCESSOR
		{
			RASTERIZER_PIPELINE_PROCESSOR,
			RAYTRACING_PIPELINE_PROCESSOR
		};

		class DX11PipelineProcessorConfigurator
		{
		public:
			static DX11AbstractPipelineProcessor* createPipelineProcessor(
				PIPELINE_PROCESSOR processor,
				ID3D11Device* pDevice, 
				ID3D11DeviceContext* pDeviceContext, 
				IDXGISwapChain* pSwapChain, 
				ID3D11RenderTargetView* pRenderTargetView,
				ID3D11DepthStencilView* pDepthStencilView,
				DXGI_MODE_DESC& rCurrentMode);
		};
	}
}

#endif
