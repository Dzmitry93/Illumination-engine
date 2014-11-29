#ifndef __DX11ABSTRACT_PIPELINE_PROCESSOR_H__
#define __DX11ABSTRACT_PIPELINE_PROCESSOR_H__

#include "IPipelineProcessor.h"
#include <D3D11.h>

namespace OGE
{
	namespace render
	{
		class DX11AbstractPipelineProcessor : public IPipelineProcessor
		{
		public:
			DX11AbstractPipelineProcessor(
				ID3D11Device* pDevice, 
				ID3D11DeviceContext* pDeviceContext, 
				IDXGISwapChain* pSwapChain,
				ID3D11RenderTargetView* pDefaultRenderTarget,
				ID3D11DepthStencilView* pDepthStencilView,
				DXGI_MODE_DESC& rCurrentMode);

			~DX11AbstractPipelineProcessor();

			virtual void start() = 0;
			
			virtual void process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera) = 0;

			virtual void shutdown() = 0;

		protected:
			ID3D11Device* pDevice;
			ID3D11DeviceContext* pDeviceContext;

			IDXGISwapChain* pSwapChain;
			ID3D11RenderTargetView* pRenderTargetView;
			ID3D11DepthStencilView* pDepthStencilView;
			DXGI_MODE_DESC currentMode;
		};

		inline DX11AbstractPipelineProcessor::DX11AbstractPipelineProcessor(
			ID3D11Device* pDevice, 
			ID3D11DeviceContext* pDeviceContext, 	
			IDXGISwapChain* pSwapChain, 
			ID3D11RenderTargetView* pRenderTargetView,
			ID3D11DepthStencilView* pDepthStencilView,
			DXGI_MODE_DESC& rCurrentMode)
					: pDevice(pDevice), 
					pDeviceContext(pDeviceContext), 
					pSwapChain(pSwapChain), 
					pRenderTargetView(pRenderTargetView),
					pDepthStencilView(pDepthStencilView),
					currentMode(rCurrentMode)
		{
		}

		inline DX11AbstractPipelineProcessor::~DX11AbstractPipelineProcessor()
		{
		}
	}
}

#endif