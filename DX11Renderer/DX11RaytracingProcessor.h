#ifndef __DX11RAYTRACING_PROCESSOR_H__
#define __DX11RAYTRACING_PROCESSOR_H__

#include "DX11AbstractPipelineProcessor.h"
#include "DX11TextureFactory.h"
#include "DX11ShaderManager.h"
#include "DX11SamplerFactory.h"
#include "DX11MeshFactory.h"
#include "KdtreeFactory.h"
#include "MeshUtil.h"

namespace OGE
{
	namespace render
	{
		struct Color
		{
			float r;
			float g;
			float b;
			float a;
		};

		class DX11RaytracingProcessor : public DX11AbstractPipelineProcessor
		{
		public:
			DX11RaytracingProcessor(
				ID3D11Device* pDevice, 
				ID3D11DeviceContext* pDeviceContext, 
				IDXGISwapChain* pSwapChain, 
				ID3D11RenderTargetView* pRenderTargetView,
				ID3D11DepthStencilView* pDepthStencilView,
				DXGI_MODE_DESC& rCurrentMode);

			~DX11RaytracingProcessor();
			
			virtual void start();

			virtual void process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera);
			
			virtual void shutdown();

		private :
			DX11Texture2D* pFrameTexture;
			ID3D11SamplerState* pSamplerState;
			ID3D11InputLayout* pInputLayout;
			DX11EffectResource* pEffect;
			BaseMesh* pScreenMesh;
			uint32* pData;

			TriangleKdtree* pTree;
			List<SceneObject<BaseLight*>> lights;
		
			ARGBColor traceRay(const Vector& rOrigin, const Vector& rDir);
			void calculateRayRange(const Camera* pCamera, Vector& rTopLeftRay, Vector& rBottomRightRay);
		};

		inline DX11RaytracingProcessor::DX11RaytracingProcessor(
			ID3D11Device* pDevice, 
			ID3D11DeviceContext* pDeviceContext, 
			IDXGISwapChain* pSwapChain,
			ID3D11RenderTargetView* pRenderTargetView,
			ID3D11DepthStencilView* pDepthStencilView,
			DXGI_MODE_DESC& rCurrentMode)
				: DX11AbstractPipelineProcessor(
					pDevice, pDeviceContext,
					pSwapChain, pRenderTargetView,
					pDepthStencilView,
					rCurrentMode)
		{
			this->pFrameTexture = NULL;
			this->pEffect = NULL;
			this->pScreenMesh = NULL;
			this->pInputLayout = NULL;
			this->pData = NULL;
		}

		inline DX11RaytracingProcessor::~DX11RaytracingProcessor()
		{
			shutdown();
		}
	}
}

#endif

