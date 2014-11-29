#ifndef __DX11RENDERER_H__
#define __DX11RENDERER_H__

#include "DX11PipelineProcessorConfigurator.h"
#include "IRenderer.h"
#include "List.h"
#include "MeshUtil.h"
#include "SceneObject.h"
#include <D3DX11.h>
#include <dxgidebug.h>
#include <map>

using std::map;
using namespace OGE::scene;
using namespace OGE::resource;

namespace OGE
{
	namespace render
	{
		class DX11Renderer : public IRenderer
		{
		public:
			DX11Renderer(PIPELINE_PROCESSOR processor);
			~DX11Renderer(void);

			bool initialize(HWND hwnd);
			
			void renderQueue(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera);

			void shutdown();
		
		private:

			const DXGI_FORMAT defaultSurfaceFormat;
			ID3D11Device* pDevice;
			ID3D11DeviceContext* pContext;
			IDXGISwapChain* pSwapChain;

			ID3D11RenderTargetView* pDefaultRenderTargetView;
			ID3D11DepthStencilView* pDepthStencilView;
			DX11Texture2D** pRenderingTextures;

			DX11AbstractPipelineProcessor* pPipelineProcessor;
			PIPELINE_PROCESSOR pipelineProcessor;
		};

		inline DX11Renderer::DX11Renderer(PIPELINE_PROCESSOR processor) 
			: defaultSurfaceFormat(DXGI_FORMAT_R8G8B8A8_UNORM), 
			pDevice(nullptr), 
			pContext(nullptr),
			pDefaultRenderTargetView(nullptr),
			pDepthStencilView(nullptr),
			pSwapChain(nullptr),
			pipelineProcessor(processor)
		{
			
		}

		inline DX11Renderer::~DX11Renderer()
		{
			shutdown();
		}

		inline void DX11Renderer::renderQueue(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera)
		{
			pPipelineProcessor->process(
				rQueue,
				rPostprocessQueue, 
				rLights,
				rCamera);
		}
	}
}

#endif

