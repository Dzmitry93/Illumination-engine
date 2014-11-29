#ifndef __DX11PIPELINE_PROCESSOR_H__
#define __DX11PIPELINE_PROCESSOR_H__

#include "DX11AbstractPipelineProcessor.h"
#include "DX11RenderableMesh.h"
#include "DX11PipelineDescriptor.h"
#include "DX11EffectFactory.h"
#include "DX11ShaderManager.h"
#include "DX11MeshFactory.h"
#include "DX11BlendFactory.h"
#include "DX11TextureFactory.h"
#include "DX11SamplerFactory.h"
#include "MathUtils.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Spotlight.h"
#include "Camera.h"
#include "MeshUtil.h"

#include <map>

using namespace OGE::math::MatrixUtils;
using namespace OGE::util;
using std::map;

namespace OGE
{
	namespace render
	{
		struct RendererState
		{
			bool generateMipMaps;
			uint8 maxAnisotropy;
		};

		class DX11RasterizerProcessor : public DX11AbstractPipelineProcessor
		{
		public:
			DX11RasterizerProcessor(
				ID3D11Device* pDevice, 
				ID3D11DeviceContext* pDeviceContext, 
				IDXGISwapChain* pSwapChain,
				ID3D11RenderTargetView* pDefaultRenderTarget,
				ID3D11DepthStencilView* pDepthStencilView,
				DXGI_MODE_DESC& rCurrentMode);
			~DX11RasterizerProcessor(void);

			virtual void start();

			virtual void process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera);

			void setRendererState(const RendererState& state);
			virtual void shutdown();

		private:
			static string ANISOTROPIC_SAMPLER;
			static string POSITION_MAP;
			static string NORMAL_MAP;
			static string DIFFUSE_MAP;
			static string SPECULAR_MAP;
			static string SPECULAR_ALBEDO;
			static string SPECULAR_POWER;

			static Matrix cubeViewMatrix[6];
			static Matrix omnidirectionalProjectionMatrix;

			const uint32 LIGHT_BUFFER_SIZE;
			const uint32 POINT_LIGHT_SHADOW_BUFFER_SIZE;
			const uint32 TRANSFORM_BUFFER_SIZE;
			const uint32 CAMERA_BUFFER_SIZE;

			DX11PipelineDescriptor geometryPass;
			DX11PipelineDescriptor lightPass;
			DX11PipelineDescriptor shadowmapPass;
			DX11PipelineDescriptor boundingBodyPass;

			RendererState rendererState;

			DX11ShaderVariableHelper* pShaderHelper;

			map<uint64, DX11RenderableMesh*> meshCache;
			map<uint64, DX11Texture2D*> textureCache;
			map<BUFFER_FORMAT, ID3D11InputLayout*> inputLayoutCache;

			void* pLightData;
			void* pPointLightShadowData;
			void* pTransformData;
			void* pCameraData;

			ID3D11RenderTargetView* pPositionRTV;
			ID3D11RenderTargetView* pNormalRTV;
			ID3D11RenderTargetView* pDiffuseRTV;
			ID3D11RenderTargetView* pSpecularRTV;

			DX11Texture2D* pPositionMap;
			DX11Texture2D* pNormalMap;
			DX11Texture2D* pDiffuseMap;
			DX11Texture2D* pSpecularMap;

			list<DX11Texture2D*> omnidirectionalShadowmaps;
			list<DX11Texture2D*> omnidirectionalShadowmapsDepth;
			
			list<ID3D11RenderTargetView*> omnidirectionalShadowmapRTVs;
			list<ID3D11DepthStencilView*> omnidirectionalShadowmapDSVs;

			DX11RenderableMesh* pScreenSizeQuad;

			DX11ResourceBuffer* pTransformBuffer;
			DX11ResourceBuffer* pLightBuffer;
			DX11ResourceBuffer* pPointLightShadowBuffer;
			DX11ResourceBuffer* pCameraBuffer;

			ID3D11SamplerState* pAnisoSamplerState;
			BlendState lightingBlendState;

			DX11RenderableMesh* getGPUMesh(const SceneObject<IntrusivePtr<BaseMesh>>& rRenderable);

			void setCameraData(const SceneObject<Camera>& rCamera, DX11EffectResource* pPipelineEffect);

			void setTransformData(
				const Matrix& rWorldMatrix,
				const SceneObject<Camera>& rCamera,
				DX11EffectResource* pPipelineEffect);

			void setLightsData(
				const SceneObject<BaseLight*>& rLight, 
				DX11EffectResource* pPipelineEffect,
				const DX11PipelineDescriptor& pipelineDesc,
				DX11Texture2D* pOmnidirectionalShadowmap);

			void setShadowData(
				const SceneObject<BaseLight*>& rLight,
				const Matrix& rProjection,
				DX11EffectResource* pPipelineEffect,
				const DX11PipelineDescriptor& pipelineDesc);

			void setMaterialsData(
				const DX11RenderableMesh* pMesh,
				DX11EffectResource* pPipelineEffect);

			void setMeshData(const DX11RenderableMesh* pMesh,
				DX11EffectResource* pPipelineEffect);

			void addShadowMap();
			void releaseShadowMaps();
		};

		inline DX11RasterizerProcessor::DX11RasterizerProcessor(
			ID3D11Device* pDevice, 
			ID3D11DeviceContext* pDeviceContext, 
			IDXGISwapChain* pSwapChain,
			ID3D11RenderTargetView* pRenderTargetView,
			ID3D11DepthStencilView* pDepthStencilView,
			DXGI_MODE_DESC& rCurrentMode)
						: DX11AbstractPipelineProcessor(
							pDevice, pDeviceContext, 
							pSwapChain, pRenderTargetView, pDepthStencilView, rCurrentMode),

							LIGHT_BUFFER_SIZE(160),
							POINT_LIGHT_SHADOW_BUFFER_SIZE(464),
							TRANSFORM_BUFFER_SIZE(192),
							CAMERA_BUFFER_SIZE(16)
		{
			this->rendererState.generateMipMaps = false;
			this->rendererState.maxAnisotropy = 16;
			this->pShaderHelper = new DX11ShaderVariableHelper();
			this->pLightData = new uint8[LIGHT_BUFFER_SIZE];
			this->pPointLightShadowData = new uint8[POINT_LIGHT_SHADOW_BUFFER_SIZE];
			this->pTransformData = new uint8[TRANSFORM_BUFFER_SIZE];
			this->pCameraData = new uint8[CAMERA_BUFFER_SIZE];
		}

		inline DX11RasterizerProcessor::~DX11RasterizerProcessor()
		{
			shutdown();
		}

		inline void DX11RasterizerProcessor::setRendererState(const RendererState& state)
		{
			this->rendererState = state;
		}
	}
}

#endif

