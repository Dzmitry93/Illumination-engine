#include "DX11RaytracingProcessor.h"
#include "PointLight.h"

namespace OGE
{
	namespace render
	{
		void DX11RaytracingProcessor::process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera)
		{
		/*	KdtreeFactory kdtreeFactory;
 			pTree = (TriangleKdtree*) kdtreeFactory.createTrianglesKdtree(
				rQueue, rCamera,
				Vector(1000.0f, 1000.0f, 1000.0f), 
				Vector(0.0f, 0.0f, 0.0f), 20.0f, 0, 5);
			lights = rLights;

			Vector topLeftPoint;
			Vector bottomRightPoint;
			Vector rOrigin = rCamera.getSceneObject().getWorldSpacePosition();

			calculateRayRange(rCamera.getSceneObject(), topLeftPoint, bottomRightPoint);
			
			float xStep = (bottomRightPoint - topLeftPoint).x / 1366.0f;
			float yStep = (bottomRightPoint - topLeftPoint).y / 768.0f;

			Vector origin;
			Vector dir;

			for(uint32 y = 0; y < 768; y++)
			{
				Vector endPoint = topLeftPoint;

				endPoint.y += yStep * y;

				for(uint32 x = 0; x < 1366; x++)
				{
					endPoint.x += xStep;
					dir = endPoint-rOrigin;
					dir.normalize();

					pData[y * 1366 + x] = traceRay(rOrigin, dir).value();
				}
			} 
			 
			float color[] = 
			{
				0.0f, 0.0f, 1.0f, 1.0f
			};	

			pFrameTexture->updateData(pDeviceContext, pData, 1366 * 4);
			pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
			pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			pDeviceContext->Draw(pScreenMesh->getCountVertices(), 0);
			pSwapChain->Present(0, 0);

			while(true) {}
			   */
		}

		ARGBColor DX11RaytracingProcessor::traceRay(const Vector& rOrigin, const Vector& rDir)
		{
			/*IntersectionResult result;
			result.d = -1.0f;
			ARGBColor color(0.0f, 0.0f, 0.0f, 0.0f);
			Vector ambientLight(0.1f, 0.1f, 0.1f, 0.0f);
			pTree->traverseRay(rOrigin, rDir, result);

			if(result.d > 0.1f)
			{
				ConstListIterator<SceneObject<BaseLight*>> it = lights.citerator();

				Vector intersectionPos = rOrigin + rDir * result.d;
				IntrusivePtr<Material> pMaterial = result.triangle.pMaterial;

				while(it.hasNext())
				{
					PointLight* pLight = (PointLight*)it.next().getSceneObject();
					const Vector& lightPosition = pLight->getLightPosition();

					Vector shadowRay = intersectionPos - lightPosition;
					float d = shadowRay.length();
					shadowRay.normalize();

					IntersectionResult shadowResult;
					shadowResult.d = -1.0f;

					Vector diffuseLight = pLight->getDiffuseColor().RGBA();
					

					pTree->traverseRay(lightPosition, shadowRay, shadowResult);
					Vector material = result.triangle.pMaterial->getDiffueColor().RGBA();
					
					float cosine = 0.0f;
					float lawValue = 0.0f;
					Vector normal = 
						result.triangle.n1 * (1 - result.w) + 
						result.triangle.n2 * (1 - result.v) +
						result.triangle.n3 * (1 - result.u); 
					normal.normalize();

					if((shadowResult.d - d) < 0.001f && (shadowResult.d - d) > -0.001f)
					{
						const QuadraticAttenuationLaw& law = pLight->getAttenuationLaw();
						lawValue = 1.0f / (law.Kc + law.Kl * d + law.Kq * d * d);
						cosine = -Vector::DotProduct(normal, shadowRay);
					}

					color += ARGBColor(0.0f, 
							diffuseLight.x * material.x * lawValue * cosine + ambientLight.x * material.x, 
							diffuseLight.y * material.y * lawValue * cosine + ambientLight.y * material.y, 
							diffuseLight.z * material.z * lawValue * cosine + ambientLight.z * material.z);

					float reflection = pMaterial->getReflection();
					if(reflection > 0.0f)
					{
						Vector reflectedRay = Vector::ReflectedVector(rDir, normal);
						Vector origin = intersectionPos + reflectedRay * 0.1f;
						color +=  traceRay(origin, reflectedRay) * reflection;
					}  
				} 
			}

			color += ARGBColor(0.0f, 
							ambientLight.x, 
							ambientLight.y, 
							ambientLight.z);
			return color;	*/
			return ARGBColor();
		}

		void DX11RaytracingProcessor::start()
		{

			if(pFrameTexture != NULL)
			{
				pFrameTexture->release();
			}
			if(pEffect != NULL)
			{
				pEffect->release();
			}

			pFrameTexture = DX11TextureFactory::createTexture2D(pDevice, 
					1366, 768, D3D11_BIND_SHADER_RESOURCE,
					DXGI_FORMAT_R8G8B8A8_UNORM,
					D3D11_USAGE_DEFAULT,
					1, 1, 0, NULL);

			debug_assert(pFrameTexture != NULL, "cannot create frame texture");
			pSamplerState = DX11SamplerFactory::createDefaultSamplerState(pDevice, 4);

			pData = new uint32[1366 * 768];
			for(uint32 i =0 ; i < 768; i++)
			{
				for(uint32 j = 0; j < 1366; j++)
				{
					pData[i * 1366 + j] = 0;
				}
			}

			DX11PipelineDescriptor desc;
			desc.generalFlags.setFlag(PIPELINE_GENERAL_STATE::TEXTURE, true);
			pEffect = DX11ShaderManager::getInstance()->getPipelineShader(pDevice, desc);

			debug_assert(pEffect != NULL, "cannot create effect");
			pEffect->setTexture("screenspaceTexture", pFrameTexture->getTexture2DSRV(pDevice));
			pEffect->setSampler("screenspaceSampler", pSamplerState);
			pEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, pDeviceContext);

			Viewport viewport;
			viewport.width = 1366.0f;
			viewport.height = 768.0f;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			viewport.topLeftX = 0.0f;
			viewport.topLeftY = 0.0f;

//			pScreenMesh = MeshUtil::createScreenQuad(viewport);
			DX11MeshFactory meshFactory;
			DX11RenderableMesh* pMesh =  meshFactory.createRenderableMesh(pDevice, pScreenMesh);

			DX11ResourceBuffer* pVB = pMesh->getVerticesBuffer();
			ID3D11Buffer* pD3DVertexBuffers[1] = { pVB->D3DBuffer() };

			UINT stride[1] = { Format::getSize(pScreenMesh->getVertexFormat()) };
			UINT offset[1] = { 0 };

			// create input layout
			D3DX11_PASS_SHADER_DESC passShaderDesc;
			D3DX11_EFFECT_SHADER_DESC shaderDesc;
			pEffect->getPointer()->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetVertexShaderDesc(&passShaderDesc);
			passShaderDesc.pShaderVariable->GetShaderDesc(0, &shaderDesc);

			void* pBytecode = (void*)shaderDesc.pBytecode;
			uint32 bytecodeLength = shaderDesc.BytecodeLength;

			pInputLayout = DX11Format::createInputLayout(pDevice, pVB->getFormat(), 
					pBytecode, bytecodeLength);

			pDeviceContext->IASetVertexBuffers(0, 1, pD3DVertexBuffers, stride, offset);
			pDeviceContext->IASetInputLayout(pInputLayout);
			pDeviceContext->IASetPrimitiveTopology(DX11Format::getDX11Topology(pMesh->getPrimitiveTopology()));
		}

		void DX11RaytracingProcessor::shutdown()
		{
			if(pFrameTexture != NULL)
			{
				pFrameTexture->release();
			}
			if(pEffect != NULL)
			{
				pEffect->release();
			}
			if(pSamplerState != NULL)
			{
				pSamplerState->Release();
			}
			safe_release(pData);
		}

		void DX11RaytracingProcessor::calculateRayRange(const Camera* pCamera, 
			Vector& rTopLeftRay, Vector& rBottomRightRay)
		{
			Vector origin = pCamera->getWorldSpacePosition();
			Vector dir = pCamera->getViewDirection();
			float focalLength = pCamera->getFocalLength();
			float aspectRatio = pCamera->getAspectRatio();

			Vector endPoint = origin + dir * focalLength ;
			Vector xDir(1.0f * aspectRatio, 0.0f, 0.0f);
			Vector yDir(0.0f, 1.0f, 0.0f);

			rTopLeftRay = endPoint - xDir + yDir;
			rBottomRightRay = endPoint + xDir - yDir;
		}	 
	}	 
}