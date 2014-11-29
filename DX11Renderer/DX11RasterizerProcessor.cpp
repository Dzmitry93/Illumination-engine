#include "DX11RasterizerProcessor.h"

namespace OGE
{
	namespace render
	{
		string DX11RasterizerProcessor::ANISOTROPIC_SAMPLER = "AnisoSampler";
		string DX11RasterizerProcessor::POSITION_MAP = "PositionMap";
		string DX11RasterizerProcessor::NORMAL_MAP = "NormalMap";
		string DX11RasterizerProcessor::DIFFUSE_MAP = "DiffuseMap";
		string DX11RasterizerProcessor::SPECULAR_MAP = "SpecularMap";
		string DX11RasterizerProcessor::SPECULAR_ALBEDO = "SpecularAlbedo";
		string DX11RasterizerProcessor::SPECULAR_POWER = "SpecularPower";

		Matrix DX11RasterizerProcessor::cubeViewMatrix[6] = 
		{
			Matrix(Vector(0.0f, 0.0f, -1.0f), Vector(0.0f, 1.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f)),
			Matrix(Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), Vector(-1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f)),

			Matrix(Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f)),
			Matrix(Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f)),

			Matrix(Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f)),
			Matrix(Vector(-1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 0.0f, -1.0f), Vector(0.0f, 0.0f, 0.0f, 1.0f))
		};

		Matrix DX11RasterizerProcessor::omnidirectionalProjectionMatrix = 
			MatrixUtils::CreateProjectionMatrixLH(90, 1.0f, 1.0f, 1000.0f);

		void DX11RasterizerProcessor::process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera)
		{
			CreateViewMatrixRH(Vector(0.0f, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
			CreateViewMatrixLH(Vector(0.0f, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
			float color[] = 
			{
				0.0f, 0.0f, 0.0f, 0.0f
			};

			float positionValue[] = 
			{
				-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f
			};

			pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
			pDeviceContext->ClearRenderTargetView(pPositionRTV, positionValue);
			pDeviceContext->ClearRenderTargetView(pNormalRTV, color);
			pDeviceContext->ClearRenderTargetView(pDiffuseRTV, color);
			pDeviceContext->ClearRenderTargetView(pSpecularRTV, color);

			pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			ID3D11RenderTargetView* rtvs[] = {
				pPositionRTV,
				pNormalRTV,
				pDiffuseRTV,
				pSpecularRTV
			};

			ID3D11RenderTargetView* emptyRTVS[] = {
				nullptr, 
				nullptr,
				nullptr, 
				nullptr
			};

			pDeviceContext->OMSetRenderTargets(4, rtvs, pDepthStencilView);
 
			// geometry pass
			DX11EffectResource* pGeometryPassEffect = DX11ShaderManager::getInstance()->getPipelineShader(pDevice, geometryPass);
			pGeometryPassEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);
			
			ConstListIterator<SceneObject<IntrusivePtr<BaseMesh>>> renderableIt = rQueue.citerator();

			while(renderableIt.hasNext())
			{
				

				const SceneObject<IntrusivePtr<BaseMesh>>& rSceneObject = renderableIt.next();
				const Matrix& rWorldMatrix = rSceneObject.getWorldMatrix();

				DX11RenderableMesh* pCurrentMesh = getGPUMesh(rSceneObject);
			
				setTransformData(rWorldMatrix, rCamera, pGeometryPassEffect);
				setMaterialsData(pCurrentMesh, pGeometryPassEffect);
				setMeshData(pCurrentMesh, pGeometryPassEffect);

				pDeviceContext->DrawIndexed(pCurrentMesh->getCountIndexes(), 0, 0); 

				pGeometryPassEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);

			
			}

			// shadow pass
			list<ID3D11DepthStencilView*>::iterator dsvIt = omnidirectionalShadowmapDSVs.begin();
			list<ID3D11DepthStencilView*>::iterator dsvEnd = omnidirectionalShadowmapDSVs.end();

			list<ID3D11RenderTargetView*>::iterator rtvIt = omnidirectionalShadowmapRTVs.begin();
			list<ID3D11RenderTargetView*>::iterator rtvEnd = omnidirectionalShadowmapRTVs.end();

			while(dsvIt != dsvEnd) 
			{
				pDeviceContext->ClearDepthStencilView(*dsvIt, D3D11_CLEAR_DEPTH, 1.0f, 0);
				pDeviceContext->ClearRenderTargetView(*rtvIt, color);

				rtvIt++;
				dsvIt++;
			}

			pDeviceContext->OMSetRenderTargets(4, emptyRTVS, pDepthStencilView);

			List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>::cit lightIt = rLights.citerator();

			if(rLights.size() > omnidirectionalShadowmapRTVs.size()) 
			{
				uint32 lightsCount = rLights.size();
				uint32 rtvSize = omnidirectionalShadowmapRTVs.size();

				while(lightsCount > rtvSize) 
				{
					addShadowMap();
					lightsCount--;
				}
			}

			dsvIt = omnidirectionalShadowmapDSVs.begin();
			rtvIt = omnidirectionalShadowmapRTVs.begin();

			while(lightIt.hasNext())
			{
				const pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>& rLightData = lightIt.next();
				const SceneObject<BaseLight*>& rLightObject = rLightData.first;
			
				LIGHT_TYPE lightType = rLightObject.getSceneObject()->getLightType();

				shadowmapPass.lightFlags.clear();

				if(lightType == LIGHT_TYPE_POINT)
				{
					shadowmapPass.lightFlags.setFlag(POINT_LIGHTS, true);	
					pDeviceContext->OMSetRenderTargets(
						1, &(*rtvIt),
						*dsvIt);

					D3D11_VIEWPORT viewport;
					viewport.Width = 1536;
					viewport.Height = 1536;
					viewport.MinDepth = 0.0f;
					viewport.MaxDepth = 1.0f;
					viewport.TopLeftX = 0;
					viewport.TopLeftY = 0;

					pDeviceContext->RSSetViewports( 1, &viewport );
				}
				else if(lightType == LIGHT_TYPE_DIRECTIONAL)
				{
					shadowmapPass.lightFlags.setFlag(DIRECTIONAL_LIGTHS, true);
				} else
				{
					shadowmapPass.lightFlags.setFlag(SPOTLIGHTS, true);
				}																			  

				DX11EffectResource* pShadowmapPassEffect = 
					DX11ShaderManager::getInstance()->getPipelineShader(pDevice, shadowmapPass);
				pShadowmapPassEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);
			
				setShadowData(
					rLightObject, 
					rCamera.getSceneObject().getProjectionMatrix(), 
					pShadowmapPassEffect, shadowmapPass);

				map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>::const_iterator meshIt = rLightData.second.cbegin();
				map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>::const_iterator meshEnd = rLightData.second.cend();
				
				while(meshIt != meshEnd)
				{
					const SceneObject<IntrusivePtr<BaseMesh>>& rSceneObject = meshIt->second;

					DX11RenderableMesh* pCurrentMesh = getGPUMesh(rSceneObject);
					const Matrix& rWorldMatrix = rSceneObject.getWorldMatrix();

					setTransformData(rWorldMatrix, rCamera, pShadowmapPassEffect);
					setMeshData(pCurrentMesh, pShadowmapPassEffect);

					pDeviceContext->DrawIndexed(pCurrentMesh->getCountIndexes(), 0, 0); 

					meshIt++;
					pShadowmapPassEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);
				}

				rtvIt++;
				dsvIt++;
			}

			// light pass
			pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
			pDeviceContext->OMSetBlendState(
				lightingBlendState.pBlendState, 
				nullptr, 
				lightingBlendState.SampleMask);	

			D3D11_VIEWPORT viewport;
			viewport.Width = 1368;
			viewport.Height = 768;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			pDeviceContext->RSSetViewports( 1, &viewport );

			lightIt = rLights.citerator();

			list<DX11Texture2D*>::reverse_iterator shadowMapIt = omnidirectionalShadowmaps.rbegin();

			while(lightIt.hasNext())
			{
				const SceneObject<BaseLight*>& rLightObject = lightIt.next().first;
				LIGHT_TYPE lightType = rLightObject.getSceneObject()->getLightType();

				lightPass.lightFlags.clear();

				if(lightType == LIGHT_TYPE_POINT)
				{
					lightPass.lightFlags.setFlag(POINT_LIGHTS, true);	
				}
				else if(lightType == LIGHT_TYPE_DIRECTIONAL)
				{
					lightPass.lightFlags.setFlag(DIRECTIONAL_LIGTHS, true);
				} else
				{
					lightPass.lightFlags.setFlag(SPOTLIGHTS, true);
				}

				DX11EffectResource* pLightPassEffect = DX11ShaderManager::getInstance()->getPipelineShader(pDevice, lightPass);
				pLightPassEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);

				pLightPassEffect->setTexture(POSITION_MAP, pPositionMap->getTexture2DSRV(pDevice));
				pLightPassEffect->setTexture(NORMAL_MAP, pNormalMap->getTexture2DSRV(pDevice));
				pLightPassEffect->setTexture(DIFFUSE_MAP, pDiffuseMap->getTexture2DSRV(pDevice));
				pLightPassEffect->setTexture(SPECULAR_MAP, pSpecularMap->getTexture2DSRV(pDevice));

				setCameraData(rCamera, pLightPassEffect);
				setMeshData(pScreenSizeQuad, pLightPassEffect);

				pLightPassEffect->setSampler(ANISOTROPIC_SAMPLER, pAnisoSamplerState);

				setLightsData(rLightObject, pLightPassEffect, lightPass, *shadowMapIt);	
				pDeviceContext->Draw(pScreenSizeQuad->getCountVertices(), 0); 

				shadowMapIt++;
			}

			pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
			
			// postprocess draw
			DX11EffectResource* pPostprocessEffect = DX11ShaderManager::getInstance()->getPipelineShader(pDevice, boundingBodyPass);
			pPostprocessEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);

			ConstListIterator<SceneObject<IntrusivePtr<BaseMesh>>> postIt = rPostprocessQueue.citerator();

			while(postIt.hasNext())
			{
				const SceneObject<IntrusivePtr<BaseMesh>>& rSceneObject = postIt.next();
				const Matrix& rWorldMatrix = rSceneObject.getWorldMatrix();
				
				DX11RenderableMesh* pCurrentMesh = getGPUMesh(rSceneObject);

				setTransformData(rWorldMatrix, rCamera, pPostprocessEffect);
				setMeshData(pCurrentMesh, pPostprocessEffect);

				pDeviceContext->Draw(pCurrentMesh->getCountVertices(), 0);
			}

			pSwapChain->Present(0, 0);
		}

		DX11RenderableMesh* DX11RasterizerProcessor::getGPUMesh(const SceneObject<IntrusivePtr<BaseMesh>>& rSceneObject)
		{
			const IntrusivePtr<BaseMesh>& rObject = rSceneObject.getSceneObject();
			DX11RenderableMesh* pCurrentMesh = nullptr;

			map<uint64, DX11RenderableMesh*>::iterator meshIt = meshCache.find(rObject->getGUID());

			if(meshIt != meshCache.end())
			{
				pCurrentMesh = meshIt->second;
			}
			else
			{
				pCurrentMesh = DX11MeshFactory::createRenderableMesh(pDevice, rObject);
				meshCache.insert(std::pair<uint64, DX11RenderableMesh*>(pCurrentMesh->getGUID(), pCurrentMesh));
			}

			return pCurrentMesh;
		}

		void DX11RasterizerProcessor::start()
		{	
			// create maps for deffered shading
			pNormalMap = DX11TextureFactory::createTexture2D(pDevice,
				currentMode.Width,
				currentMode.Height,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				D3D11_USAGE_DEFAULT,
				1, 1, 0, nullptr);

			pPositionMap = DX11TextureFactory::createTexture2D(pDevice,
				currentMode.Width,
				currentMode.Height,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				D3D11_USAGE_DEFAULT,
				1, 1, 0, nullptr);

			pDiffuseMap = DX11TextureFactory::createTexture2D(pDevice,
				currentMode.Width,
				currentMode.Height,
				D3D11_BIND_SHADER_RESOURCE  | D3D11_BIND_RENDER_TARGET,
				DXGI_FORMAT_R8G8B8A8_SNORM,
				D3D11_USAGE_DEFAULT,
				1, 1, 0, nullptr);

			pSpecularMap = DX11TextureFactory::createTexture2D(pDevice,
				currentMode.Width,
				currentMode.Height,
				D3D11_BIND_SHADER_RESOURCE  | D3D11_BIND_RENDER_TARGET,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				D3D11_USAGE_DEFAULT,
				1, 1, 0, nullptr);

			pDevice->CreateRenderTargetView(
					pNormalMap->getPointer(),
					nullptr,
					&pNormalRTV);

			pDevice->CreateRenderTargetView(
					pPositionMap->getPointer(),
					nullptr,
					&pPositionRTV);

			pDevice->CreateRenderTargetView(
					pDiffuseMap->getPointer(),
					nullptr,
					&pDiffuseRTV);

			pDevice->CreateRenderTargetView(
					pSpecularMap->getPointer(),
					nullptr,
					&pSpecularRTV);

			// create constant buffers
			pLightBuffer = DX11BufferFactory::createConstantBuffer(pDevice, 
				D3D11_USAGE_DYNAMIC,
				pLightData, 
				LIGHT_BUFFER_SIZE);

			pPointLightShadowBuffer = DX11BufferFactory::createConstantBuffer(pDevice,
				D3D11_USAGE_DYNAMIC,
				pPointLightShadowData,
				POINT_LIGHT_SHADOW_BUFFER_SIZE);

			pTransformBuffer = DX11BufferFactory::createConstantBuffer(pDevice, 
				D3D11_USAGE_DYNAMIC, 
				pTransformData, 
				TRANSFORM_BUFFER_SIZE);

			pCameraBuffer = DX11BufferFactory::createConstantBuffer(pDevice, 
				D3D11_USAGE_DYNAMIC, 
				pCameraData, 
				CAMERA_BUFFER_SIZE);

		   // create default anisotropic sampler
			pAnisoSamplerState = 
				DX11SamplerFactory::createDefaultSamplerState(pDevice, rendererState.maxAnisotropy);

			// create mesh for screen rendering
			D3D11_VIEWPORT viewport;
			UINT num[] = {1};
			pDeviceContext->RSGetViewports( num, &viewport);  

			BaseMesh* pMesh = MeshUtil::createScreenQuad(viewport.MinDepth);

			pScreenSizeQuad = DX11MeshFactory::createRenderableMesh(pDevice, IntrusivePtr<BaseMesh>(pMesh));
			pMesh->release();

			//create default blend state
			lightingBlendState = DX11BlendFactory::createLightingBlendState(pDevice);

			// create pipeline descriptors
			geometryPass.generalFlags.setFlag(DEFFERED_SHADING_GEOMETRY_PASS, true);
			lightPass.generalFlags.setFlag(DEFFERED_SHADING_LIGHT_PASS, true);
			shadowmapPass.generalFlags.setFlag(SHADOW_MAPPING_PASS, true);
			boundingBodyPass.generalFlags.setFlag(BOUNDING_BODY, true);
		}

		void DX11RasterizerProcessor::addShadowMap() 
		{
			// create texcube for omnidirectional lights
			DX11Texture2D* pShadowMap = 
				DX11TextureFactory::createCubemap(
					pDevice, 
					1536,
					1536,
					D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
					DXGI_FORMAT_R32_FLOAT,
					D3D11_USAGE_DEFAULT);

			DX11Texture2D* pShadowMapDepth = 
				DX11TextureFactory::createCubemap(
					pDevice, 
					1536,
					1536,
					D3D11_BIND_DEPTH_STENCIL,
					DXGI_FORMAT_D32_FLOAT,
					D3D11_USAGE_DEFAULT);

			ID3D11RenderTargetView* pShadowRTV;
			ID3D11DepthStencilView* pShadowDSV;

			D3D11_RENDER_TARGET_VIEW_DESC shadowRTVdesc;

			shadowRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			shadowRTVdesc.Format = DXGI_FORMAT_R32_FLOAT;
			shadowRTVdesc.Texture2DArray.FirstArraySlice = 0;
			shadowRTVdesc.Texture2DArray.ArraySize = 6;
			shadowRTVdesc.Texture2DArray.MipSlice = 0;

			pDevice->CreateRenderTargetView(
					pShadowMap->getPointer(), 
					&shadowRTVdesc,
					&pShadowRTV);

			D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSVdesc;
			ZeroMemory(&shadowDSVdesc, sizeof(shadowDSVdesc));

			shadowDSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
			shadowDSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			shadowDSVdesc.Texture2DArray.FirstArraySlice = 0;
			shadowDSVdesc.Texture2DArray.ArraySize = 6;
			shadowDSVdesc.Texture2DArray.MipSlice = 0;

			pDevice->CreateDepthStencilView(
				pShadowMapDepth->getPointer(), 
				&shadowDSVdesc,
				&pShadowDSV); 
														   
			omnidirectionalShadowmaps.push_back(pShadowMap);
			omnidirectionalShadowmapsDepth.push_back(pShadowMapDepth);
			
			omnidirectionalShadowmapRTVs.push_back(pShadowRTV);
			omnidirectionalShadowmapDSVs.push_back(pShadowDSV);
		}

		void DX11RasterizerProcessor::releaseShadowMaps() 
		{
			list<DX11Texture2D*>::iterator it = omnidirectionalShadowmaps.begin();
			list<DX11Texture2D*>::iterator end = omnidirectionalShadowmaps.end();

			while(it != end) 
			{
				DX11Texture2D* pTexture = *it;
				safe_resource_release(pTexture);
			}

			it = omnidirectionalShadowmapsDepth.begin();
			end = omnidirectionalShadowmapsDepth.end();

			while(it != end) 
			{
				DX11Texture2D* pTexture = *it;
				safe_resource_release(pTexture);
			}

			list<ID3D11RenderTargetView*>::iterator rit = omnidirectionalShadowmapRTVs.begin();
			list<ID3D11RenderTargetView*>::iterator rend = omnidirectionalShadowmapRTVs.end();

			while(rit != rend) 
			{
				ID3D11RenderTargetView* pView = *rit;
				safe_dx_resource_release(pView);
			}

			rit = omnidirectionalShadowmapRTVs.begin();
			rend = omnidirectionalShadowmapRTVs.end();

			while(rit != rend) 
			{
				ID3D11RenderTargetView* pView = *rit;
				safe_dx_resource_release(pView);
			}
		}

		void DX11RasterizerProcessor::shutdown()
		{
			safe_dx_resource_release(pAnisoSamplerState);
			safe_resource_release(pCameraBuffer);
			safe_resource_release(pLightBuffer);
			safe_resource_release(pPointLightShadowBuffer);
			safe_resource_release(pTransformBuffer);

			safe_dx_resource_release(pNormalRTV);
			safe_dx_resource_release(pPositionRTV);
			safe_dx_resource_release(pDiffuseRTV);
			safe_dx_resource_release(pSpecularRTV);

			safe_resource_release(pNormalMap);
			safe_resource_release(pPositionMap);
			safe_resource_release(pDiffuseMap);
			safe_resource_release(pSpecularMap);

			safe_release(pShaderHelper);
			safe_release(pLightData);
			safe_release(pPointLightShadowData);
			safe_release(pTransformData);
			safe_release(pCameraData);

			map<uint64, DX11RenderableMesh*>::iterator meshIt = meshCache.begin();
			map<uint64, DX11RenderableMesh*>::iterator meshEnd = meshCache.end();

			map<uint64, DX11Texture2D*>::iterator textureIt = textureCache.begin();
			map<uint64, DX11Texture2D*>::iterator textureEnd = textureCache.end();

			map<BUFFER_FORMAT, ID3D11InputLayout*>::iterator inputLayoutIt = inputLayoutCache.begin();
			map<BUFFER_FORMAT, ID3D11InputLayout*>::iterator inputLayoutEnd = inputLayoutCache.end();

			while(meshIt != meshEnd)
			{
				safe_resource_release(meshIt->second);
				++meshIt;
			}

			while(textureIt != textureEnd)
			{
				safe_resource_release(textureIt->second);
				++textureIt;
			}

			while(inputLayoutIt != inputLayoutEnd)
			{
				safe_dx_resource_release(inputLayoutIt->second);
				++inputLayoutIt;
			}

			releaseShadowMaps();
		}

		void DX11RasterizerProcessor::setCameraData(
			const SceneObject<Camera>& rCamera, 
			DX11EffectResource* pPipelineEffect)
		{
			Vector viewDir =  rCamera.getSceneObject().getViewDirection(); 
			viewDir *= rCamera.getWorldMatrix();

			memcpy(pCameraData, viewDir.data, 16);
			pCameraBuffer->writeData(pDeviceContext, pCameraData, CAMERA_BUFFER_SIZE);

			pPipelineEffect->setCBuffer(
				pShaderHelper->getCBufferName(SHADER_CBUFFER::SHADER_CBUFFER_CAMERA),
				pCameraBuffer->D3DBuffer());
		}

		void DX11RasterizerProcessor::setTransformData(
				const Matrix& rWorldMatrix,
				const SceneObject<Camera>& rCamera,
				DX11EffectResource* pPipelineEffect)
		{
			Matrix wvp;

			void* pTempPointer = (void*) pTransformData;
			memcpy(pTempPointer, &rWorldMatrix.data, 64);
			pTempPointer = ((uint8*) pTempPointer + 64);

			Matrix::MultiplyMatrix(wvp, rWorldMatrix, rCamera.getWorldMatrix());
			Matrix::MultiplyMatrix(wvp, wvp, rCamera.getSceneObject().getViewMatrix());

			memcpy(pTempPointer, &wvp.data, 64);
			pTempPointer = ((uint8*) pTempPointer + 64);

			Matrix::MultiplyMatrix(wvp, wvp, rCamera.getSceneObject().getProjectionMatrix());

			memcpy(pTempPointer, &wvp.data, 64);

			pTransformBuffer->writeData(pDeviceContext, pTransformData, TRANSFORM_BUFFER_SIZE);

			pPipelineEffect->setCBuffer(
				pShaderHelper->getCBufferName(SHADER_CBUFFER::SHADER_CBUFFER_TRANSFORM), 
				pTransformBuffer->D3DBuffer());
		}

		void DX11RasterizerProcessor::setLightsData(
				const SceneObject<BaseLight*>& rLight, 
				DX11EffectResource* pPipelineEffect,
				const DX11PipelineDescriptor& pipelineDesc,
				DX11Texture2D* pOmnidirectionalShadowmap)
		{

			BaseLight* pBaseLight = rLight.getSceneObject();

			void* tempPointer = (uint8*) pLightData;
			Vector color = pBaseLight->getDiffuseColor().ARGB();

			// set diffuse color data
			memcpy(tempPointer, color.data, 16);
			tempPointer = (uint8*)tempPointer + 16;
			// set specular color data
			memcpy(tempPointer, color.data, 16);
			tempPointer = (uint8*)tempPointer + 16;

			if(pBaseLight->getLightType() == LIGHT_TYPE::LIGHT_TYPE_SPOTLIGHT)
			{
				Spotlight* pLight = (Spotlight*) pBaseLight;

				float falloffAngle = pLight->getFalloffAngle();
				float falloffExp = pLight->getFalloffExponent();
					
				// set falloff angle
				memcpy(tempPointer, (void*)&falloffAngle, 4);
				tempPointer = (uint8*)tempPointer + 4;
				// set falloff exponent
				memcpy(tempPointer, (void*)&falloffExp, 4);

				// set attenuation law
				memcpy(tempPointer, (void*)&pLight->getAttenuationLaw(), 12);
				tempPointer = (uint8*)tempPointer + 16;

				// set light position
				memcpy(tempPointer, pLight->getLightPosition().data, 16);
				tempPointer = (uint8*)tempPointer + 16;

				// set light direction
				memcpy(tempPointer, pLight->getDirection().data, 16);
				tempPointer = (uint8*)tempPointer + 16;
			}	
			else
			if(pBaseLight->getLightType() == LIGHT_TYPE::LIGHT_TYPE_POINT )
			{
				PointLight* pLight = (PointLight*) pBaseLight;

				tempPointer = (uint8*) tempPointer + 16;

				// set attenuation law
				memcpy(tempPointer, (void*)&pLight->getAttenuationLaw(), 16);
				tempPointer = (uint8*) tempPointer + 16;

				// set light position
				memcpy(tempPointer, pLight->getLightPosition().data, 16);
				tempPointer = (uint8*) tempPointer + 32;

				memcpy(tempPointer, &omnidirectionalProjectionMatrix, 64);

				pPipelineEffect->setTexture(
					"ShadowMap",
					pOmnidirectionalShadowmap->getCubemapSRV(pDevice));
			}
			else
			if(pBaseLight->getLightType() == LIGHT_TYPE::LIGHT_TYPE_DIRECTIONAL )
			{
				DirectionalLight* pLight = (DirectionalLight*) pBaseLight;

				tempPointer = (uint8*) tempPointer + 36;

				// set light direction
				memcpy(tempPointer, pLight->getDirection().data, 16);
			}
				
			pLightBuffer->writeData(pDeviceContext, pLightData, LIGHT_BUFFER_SIZE);

			pPipelineEffect->setCBuffer(
				pShaderHelper->getCBufferName(SHADER_CBUFFER::SHADER_CBUFFER_LIGHTS),
				pLightBuffer->D3DBuffer());
		}

		void DX11RasterizerProcessor::setShadowData(
				const SceneObject<BaseLight*>& rLight,
				const Matrix& rProjection,
				DX11EffectResource* pPipelineEffect,
				const DX11PipelineDescriptor& pipelineDesc)
		{
			if(pipelineDesc.lightFlags.testFlag(PIPELINE_LIGHT_STATE::POINT_LIGHTS))
			{
				DampedLight* pDampedLight =  (DampedLight*) rLight.getSceneObject();
				Matrix lightWorldMatrix = Matrix::asTranslation(-pDampedLight->getLightPosition());

				void* tempPtr = pPointLightShadowData;

				memcpy(tempPtr, &omnidirectionalProjectionMatrix, 64);
				tempPtr = (uint8*)tempPtr + 64;

				for(uint32 i = 0; i < 6; i++)
				{
					Matrix m = lightWorldMatrix * Matrix::asTranspose(cubeViewMatrix[i]);
					
					memcpy(tempPtr, &m, 64);
					tempPtr = (uint8*)tempPtr + 64;
				}

				memcpy(tempPtr, &pDampedLight->getLightPosition(), 16);
			}

			pPointLightShadowBuffer->writeData(
				pDeviceContext,
				pPointLightShadowData,
				POINT_LIGHT_SHADOW_BUFFER_SIZE);

			pPipelineEffect->setCBuffer(
				pShaderHelper->getCBufferName(SHADER_CBUFFER_SHADOW),
				pPointLightShadowBuffer->D3DBuffer());
		}

		void DX11RasterizerProcessor::setMaterialsData(
				const DX11RenderableMesh* pMesh,
				DX11EffectResource* pPipelineEffect)
		{
			// set diffuse texture

			DX11Texture2D* pDiffuseTexture = nullptr;
			DX11Texture2D* pNormalTexture = nullptr;

			IntrusivePtr<Texture2D> pSrcTexture = pMesh->getMaterial()->getDiffuseTexture();
			uint64 guid = pSrcTexture->getGUID();
			map<uint64, DX11Texture2D*>::iterator textureIt = textureCache.find(guid);
			
			if(textureIt == textureCache.end())
			{
				pDiffuseTexture = DX11TextureFactory::createTexture2D(pDevice, pSrcTexture.getData());
				if(!pDiffuseTexture->isNULL())
				{
					textureCache.insert(std::pair<uint64, DX11Texture2D*>(guid, pDiffuseTexture));
				}
			}
			else
			{
				pDiffuseTexture = textureIt->second;
			}
					
			pPipelineEffect->setTexture(DIFFUSE_MAP, pDiffuseTexture->getTexture2DSRV(pDevice));

			// set normal map
			pSrcTexture = pMesh->getMaterial()->getNormalTexture();
			guid = pSrcTexture->getGUID();
			textureIt = textureCache.find(guid);
			
			if(textureIt == textureCache.end())
			{
				pNormalTexture = DX11TextureFactory::createTexture2D(pDevice, pSrcTexture.getData());
				if(!pNormalTexture->isNULL())
				{
					textureCache.insert(std::pair<uint64, DX11Texture2D*>(guid, pNormalTexture));
				}
			}
			else
			{
				pNormalTexture = textureIt->second;
			}
					
			pPipelineEffect->setTexture(NORMAL_MAP, pNormalTexture->getTexture2DSRV(pDevice));
			
			// set specular params
			pPipelineEffect->setVectorByName(SPECULAR_ALBEDO, pMesh->getMaterial()->getSpecularAlbedo().RGBA());
			pPipelineEffect->setFloat(SPECULAR_POWER, pMesh->getMaterial()->getSpecularPower());

			// set anisotropic filtering
			pPipelineEffect->setSampler(ANISOTROPIC_SAMPLER, pAnisoSamplerState);
		}

		void DX11RasterizerProcessor::setMeshData(
				const DX11RenderableMesh* pMesh,
				DX11EffectResource* pPipelineEffect)
		{
			DX11ResourceBuffer* pVB = pMesh->getVerticesBuffer();
			DX11ResourceBuffer* pIB = pMesh->getIndexesBuffer();

			ID3D11Buffer* pD3DVertexBuffers[1] = { pVB->D3DBuffer() };
			ID3D11Buffer* pD3DIndexBuffer = pIB->D3DBuffer();

			UINT stride[1] = { Format::getSize(pMesh->getVertexFormat()) };
			UINT offset[1] = { 0 };
		   
			// create input layout
			D3DX11_PASS_SHADER_DESC passShaderDesc;
			D3DX11_EFFECT_SHADER_DESC shaderDesc;

			pPipelineEffect->getPointer()->GetTechniqueByIndex(0)->
				GetPassByIndex(0)->GetVertexShaderDesc(&passShaderDesc);
			passShaderDesc.pShaderVariable->GetShaderDesc(0, &shaderDesc);

			void* pBytecode = (void*)shaderDesc.pBytecode;
			uint32 bytecodeLength = shaderDesc.BytecodeLength;

			ID3D11InputLayout* pCurrentInputLayout = NULL;
			map<BUFFER_FORMAT, ID3D11InputLayout*>::iterator it = inputLayoutCache.find(pVB->getFormat());

			if(it != inputLayoutCache.end())
			{
				pCurrentInputLayout = it->second;
			}
			else
			{
				pCurrentInputLayout = DX11Format::createInputLayout(pDevice, pVB->getFormat(), 
					pBytecode, bytecodeLength);
				inputLayoutCache.insert(std::pair<BUFFER_FORMAT, ID3D11InputLayout*>(pVB->getFormat(), pCurrentInputLayout));
			}

			pDeviceContext->IASetVertexBuffers(0, 1, pD3DVertexBuffers, stride, offset);
			if(!pIB->isEmpty())
			{
				pDeviceContext->IASetIndexBuffer(pD3DIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
			}

			pDeviceContext->IASetInputLayout(pCurrentInputLayout);
			pDeviceContext->IASetPrimitiveTopology(DX11Format::getDX11Topology(pMesh->getPrimitiveTopology()));
		}
	}
}

