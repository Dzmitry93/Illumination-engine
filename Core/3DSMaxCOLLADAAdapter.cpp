#include "3DSMaxCOLLADAAdapter.h"

namespace OGE
{
	namespace fileSystem
	{
		void Max13COLLADAAdapter::loadScene(const string& path, List<OGEMaterialData>& materials, 
				List<OGEModelData*>& models, OGESceneData& sceneData)
		{
			COLLADADocument* pCOLLADAData = pReader->loadScene(path);
			CLibraryGeometries* pLibraryGeometries = pCOLLADAData->pLibraryGeometries;
			CLibraryEffects* pLibraryEffects = pCOLLADAData->pLibraryEffects;
			CLibraryMaterial* pLibraryMaterials = pCOLLADAData->pLibraryMaterials;
			CLibraryImages* pLibraryImages = pCOLLADAData->pLibraryImages;
			CLibraryVisualScenes* pLibraryVisualsScenes = pCOLLADAData->pLibraryVisualScenes;
			CLibraryCameras* pLibraryCameras = pCOLLADAData->pLibraryCameras;
			CLibraryLights* pLibraryLights = pCOLLADAData->pLibraryLights;
			sceneData.pRoot = NULL;

			List<OGELightData*> lights;
			List<OGECameraData*> cameras;

			createMaterials(pLibraryEffects, pLibraryImages, pLibraryMaterials, materials);
			createGeometry(pLibraryGeometries, models);
			createCameras(pLibraryCameras, cameras);
			createLights(pLibraryLights, lights);
			createScene(pLibraryVisualsScenes, &sceneData, models, lights, cameras);

			releaseCOLLADAData(pCOLLADAData);
		}

		void Max13COLLADAAdapter::createScene(CLibraryVisualScenes* pVisualsScenesLibrary, 
											OGESceneData* pSceneData,
											List<OGEModelData*>& models,
											List<OGELightData*>& rLightsData, 
											List<OGECameraData*>& rCameraData)
		{
			CVisualScene scene = pVisualsScenesLibrary->pScenes[0];
			pSceneData->pRoot = new OGESceneNode;
			pSceneData->pRoot->pParent = NULL;

			for(uint32 i = 0; i < scene.countNodes; i++)
			{
				OGESceneNode* pNewSceneData = new OGESceneNode;
				parseNodes(scene.ppNodes[i], pNewSceneData, models, rLightsData, rCameraData);

				pNewSceneData->pParent = pSceneData->pRoot;
				pSceneData->pRoot->sceneNodes.push_back(pNewSceneData);
			}
		}

		void Max13COLLADAAdapter::parseNodes(CNode* pNode, OGESceneNode* pSceneNode,
											List<OGEModelData*>& models,
											List<OGELightData*>& rLightsData, 
											List<OGECameraData*>& rCameraData)
		{ 
			Matrix rotation;
			Matrix translation;
			Matrix scale;

			Matrix xRotation;
			Matrix yRotation;
			Matrix zRotation;

			Vector defaultDirection(0.0f, 0.0f, -1.0f);

			// used to conversion to lefthanded coordinates
			Matrix reflectionMatrix;
			Matrix::MultiplyMatrix(reflectionMatrix, 
				Matrix::asRotationZ(ToRadian(180)), Matrix::asRotationX(ToRadian(90)));

			uint32 countRotate = pNode->pRTS->countRotate;
			uint32 countTranslate = pNode->pRTS->countTranslate;
			uint32 countScale = pNode->pRTS->countScale;

			CRotate* pRotate = pNode->pRTS->pRotate;
			CTranslate* pTranslate = pNode->pRTS->pTranslate;
			CScale* pScale = pNode->pRTS->pScale;

			for(uint32 i = 0; i < countRotate; i++)
			{
				if(pRotate[i].sid == "rotateX")
				{
					xRotation = Matrix::asRotationX(ToRadian(-pRotate[i].degree));
				}
				if(pRotate[i].sid == "rotateY")
				{
					yRotation = Matrix::asRotationY(ToRadian(-pRotate[i].degree));
				}
				if(pRotate[i].sid == "rotateZ")
				{
					zRotation = Matrix::asRotationZ(ToRadian(-pRotate[i].degree));
				}
			}

			for(uint32 i = 0; i < countTranslate; i++)
			{
				translation *= Matrix::asTranslation(pTranslate[i].x, pTranslate[i].y, pTranslate[i].z);
			}

			for(uint32 i = 0; i < countScale; i++)
			{
				scale *= Matrix::asScale(pScale[i].x, pScale[i].y, pScale[i].z);
			}

			if(pNode->pInstanceGeometry != NULL)
			{
				bool found = false;
				string geometryName = pNode->pInstanceGeometry->geometryId;

				ListIterator<OGEModelData*> it = models.iterator();

				while(it.hasNext() && !found)
				{
					OGEModelData* pModel = it.next();
					if("#"+pModel->geometryName == geometryName)
					{
						found = true;
						pSceneNode->pModelData = pModel;
						pSceneNode->translation *= translation;
						pSceneNode->rotation *= rotation;
						pSceneNode->scale *= scale *  reflectionMatrix;
					}
				}
			}
			else
			{
				pSceneNode->pModelData = NULL;
			}

			if(pNode->pInstanceCamera != NULL)
			{
				string cameraID = pNode->pInstanceCamera->url;

				ListIterator<OGECameraData*> it = rCameraData.iterator();
				bool found = false;

				while(it.hasNext() && !found)
				{
					OGECameraData* pCameraData = it.next();
					if("#"+pCameraData->id == cameraID)
					{
						found = true;
						pCameraData->cameraPosition = Vector(0.0f, 0.0f, 0.0f, 1.0f);
						pCameraData->cameraDirection = defaultDirection * xRotation * yRotation * zRotation * reflectionMatrix;
						pCameraData->cameraPosition *= translation;
						pCameraData->cameraPosition *= reflectionMatrix;
						pCameraData->upDirection = Vector(0.0f, 1.0f, 0.0f);
						pSceneNode->pCameraData = pCameraData;
					}
				}
			}
			else
			{
				pSceneNode->pCameraData = NULL;
			}

			if(pNode->pInstanceLight != NULL)
			{
				string lightID = pNode->pInstanceLight->url;

				ListIterator<OGELightData*> it = rLightsData.iterator();
				bool found = false;

				while(it.hasNext() && !found)
				{
					OGELightData* pLightData = it.next();
					if("#"+pLightData->id == lightID)
					{
						if(pLightData->pDirectionalLight)
						{
							pLightData->pDirectionalLight->direction = defaultDirection * xRotation * yRotation * zRotation * reflectionMatrix;
						}
						if(pLightData->pPointLight)
						{
							pLightData->pPointLight->position *= translation;
							pLightData->pPointLight->position *= reflectionMatrix;
						}
						if(pLightData->pSpotlight)
						{
							pLightData->pSpotlight->direction = defaultDirection * xRotation * yRotation * zRotation * reflectionMatrix;
							pLightData->pSpotlight->position *= translation;
							pLightData->pSpotlight->position *= reflectionMatrix;
						}

						found = true;
						pSceneNode->pLightData = pLightData;
					}
				}
			}
			else
			{
				pSceneNode->pLightData = NULL;
			}

			ListIterator<CNode*> nodeIt = pNode->nodes.iterator();

			while(nodeIt.hasNext())
			{
				OGESceneNode* pNewSceneNode = new OGESceneNode();
				parseNodes(nodeIt.next(), pNewSceneNode, models, rLightsData, rCameraData);

				pNewSceneNode->pParent = pSceneNode;
				pSceneNode->sceneNodes.push_back(pNewSceneNode);
			}
		}

		void Max13COLLADAAdapter::createGeometry(CLibraryGeometries* pGeometriesLibrary, List<OGEModelData*>& models)
		{
			// set mesh data
			for(uint32 countGeometries = 0; 
				countGeometries < pGeometriesLibrary->countGeometries; countGeometries++)
			{
				CGeometry geometry = pGeometriesLibrary->pGeometry[countGeometries];
				CMesh mesh = geometry.mesh;
				CSource* pSources = mesh.pSources;
				CPolygons* pPolygonsBuffers = mesh.pPrimitivesBuffers;

				uint32 countSources = mesh.sourcesCount;
				uint32 countPrimitivesBuffers = mesh.countPrimitivesBuffers;

				vector<V3TNT2Vertex> vertices;
				vector<uint32> indexes;

				CInput* pInputs = pPolygonsBuffers->pInputs;

				// set sources ids
				string verticesId;
				string normalId; 
				string texcoordId;

				// COLLADA sources
				CSource verticesSource;
				CSource normalsSource;
				CSource texcoordSource;

				for(uint32 semanticIndex = 0; semanticIndex < mesh.vertices.countSemantics; 
					semanticIndex++)
				{
					if(mesh.vertices.pInputSemantic[semanticIndex].semantic == "POSITION")
					{
						verticesId = mesh.vertices.pInputSemantic[semanticIndex].sourceId;
					}
					if(mesh.vertices.pInputSemantic[semanticIndex].semantic == "NORMAL")
					{
						normalId = mesh.vertices.pInputSemantic[semanticIndex].sourceId;
					}
					if(mesh.vertices.pInputSemantic[semanticIndex].semantic == "TEXCOORD")
					{
						texcoordId = mesh.vertices.pInputSemantic[semanticIndex].sourceId;

					}
				}

				// find sources 
				for(uint32 sourceIndex = 0; sourceIndex < countSources; sourceIndex++)
				{
					if("#"+pSources[sourceIndex].id == verticesId)
					{
						verticesSource = pSources[sourceIndex];
					}
					if("#"+pSources[sourceIndex].id == normalId)
					{
						normalsSource = pSources[sourceIndex];
					}
					if("#"+pSources[sourceIndex].id == texcoordId)
					{
						texcoordSource = pSources[sourceIndex];
					}
				}

				combineVerticesData(verticesSource, normalsSource, texcoordSource, 
					*pPolygonsBuffers, vertices, indexes);

				V3TNT2Vertex* pVertices = new V3TNT2Vertex[vertices.size()];
				uint32 verticesSize = vertices.size() * sizeof(V3TNT2Vertex);

				uint32* pIndexes = new uint32[indexes.size()];
				uint32 indexesSize = indexes.size() * sizeof(uint32);

				memcpy((void*)pVertices, (void*)vertices.data(), verticesSize);
				memcpy((void*)pIndexes, (void*)indexes.data(), indexesSize);

				VertexBuffer* pVerticesBuffer = 
					BufferFactory::createVertexBuffer(VERTEX_FORMAT_V3TNT2,
					pVertices, verticesSize, Format::getSize(VERTEX_FORMAT_V3TNT2));

				IndexBuffer* pIndexesBuffer = 
					BufferFactory::createIndexBuffer(INDEX_32,
					pIndexes, indexesSize, Format::getSize(INDEX_32));

				OGEModelData* modelData = new OGEModelData;
				modelData->pIndexesBuffer = pIndexesBuffer;
				modelData->pVerticesBuffer = pVerticesBuffer;
				modelData->materialID = pPolygonsBuffers->materialId;
				modelData->geometryName = geometry.id;

				models.push_back(modelData);

				safe_release(pVertices);
				safe_release(pIndexes);
			}
		}

		void Max13COLLADAAdapter::combineVerticesData(const CSource& verticesSource, 
			const CSource& normalsSource, const CSource& texcoordsSource,
			const CPolygons& polygons, 
			vector<V3TNT2Vertex>& vertexData, 
			vector<uint32>& trianglesData)
		
		{
			// description of faces 
			CPolygon* pPolygons = polygons.pPrimitive;
			uint32 countPolygons = polygons.countPrimitives;

			uint32 countVertices = verticesSource.accessor.count;

			Vertex* pVertices = (Vertex*) verticesSource.arrayEllement.pData;
			Normal* pNormals = (Normal*) normalsSource.arrayEllement.pData;
			Texcoord2* pTexcoords = (Texcoord2*) texcoordsSource.arrayEllement.pData;
			
			vector<TangentSum> averageTangets(countVertices);
			V3TNT2Vertex vertex;

			for(uint32 i = 0; i < countVertices; i++)
			{
				vertex.x = pVertices[i].vx;
				vertex.y = pVertices[i].vy;
				vertex.z = pVertices[i].vz;

				vertex.nx = pNormals[i].nx;
				vertex.ny = pNormals[i].ny;
				vertex.nz = pNormals[i].nz;

				vertex.tx = pTexcoords[i].tx;
				vertex.ty = pTexcoords[i].ty;

				vertexData.push_back(vertex);
				averageTangets[i].countPrimitives = 0;
			}

			// parse COLLADA polygons 
			for(uint32 i = 0; i < countPolygons; i++)
			{
				uint32* pPolygonIndexes = pPolygons[i].pData;
				uint32 index = pPolygonIndexes[0];
				uint32 index2 = pPolygonIndexes[1];
				uint32 index3 = pPolygonIndexes[2];

				trianglesData.push_back(index);
				trianglesData.push_back(index2);
				trianglesData.push_back(index3);

				// calculate tangent space vectors 
				Vector v0(vertexData[index].x, vertexData[index].y, vertexData[index].z); 
				Vector v1(vertexData[index2].x, vertexData[index2].y, vertexData[index2].z);
				Vector v2(vertexData[index3].x, vertexData[index3].y, vertexData[index3].z);

				Vector tangent;
				Vector binormal;

				CalculateTangentSpace(v0, v1, v2, 
					vertexData[index2].tx - vertexData[index].tx, 
					vertexData[index2].ty - vertexData[index].ty,  
					vertexData[index3].tx - vertexData[index].tx, 
					vertexData[index3].ty - vertexData[index].ty,
					tangent, binormal);

				averageTangets[index].tangent += tangent;
				averageTangets[index].binormal += binormal;
				averageTangets[index].countPrimitives++;

				CalculateTangentSpace(v1, v0, v2, 
					vertexData[index].tx - vertexData[index2].tx,
					vertexData[index].ty - vertexData[index2].ty,  
					vertexData[index3].tx - vertexData[index2].tx,
					vertexData[index3].ty - vertexData[index2].ty,
					tangent, binormal);

				averageTangets[index2].tangent += tangent;
				averageTangets[index2].binormal += binormal;
				averageTangets[index2].countPrimitives++;

				CalculateTangentSpace(v2, v0, v1, 
					vertexData[index].tx - vertexData[index3].tx,
					vertexData[index].ty - vertexData[index3].ty,  
					vertexData[index2].tx - vertexData[index3].tx,
					vertexData[index2].ty - vertexData[index3].ty,
					tangent, binormal);

				averageTangets[index3].tangent += tangent;
				averageTangets[index3].binormal += binormal;
				averageTangets[index3].countPrimitives++;
			} 

			for(uint32 i = 0; i < countVertices; i++)
			{
				Vector binormal = averageTangets[i].binormal;
				Vector tangent = averageTangets[i].tangent;
				Vector normal(vertexData[i].nx, vertexData[i].ny, vertexData[i].nz);

				// average binormal
				binormal *= 1.0f / averageTangets[i].countPrimitives;

				// average tangent
				tangent *= 1.0f / averageTangets[i].countPrimitives;

				binormal.normalize();
				tangent.normalize();

				// Gram - Schmidt ortogonalization
				tangent = tangent - normal * Vector::DotProduct(normal, tangent);
				binormal = binormal - normal * Vector::DotProduct(normal, binormal) - 
					tangent * Vector::DotProduct(tangent, binormal);

				binormal.normalize();
				tangent.normalize();
				
				vertexData[i].bnx = binormal.x;
				vertexData[i].bny = binormal.y;
				vertexData[i].bnz = binormal.z;

				vertexData[i].tnx = tangent.x;
				vertexData[i].tny = tangent.y;
				vertexData[i].tnz = tangent.z;
			}
		}

		void Max13COLLADAAdapter::createMaterials(CLibraryEffects* pEffectsLibrary, CLibraryImages* pImagesLibrary,
				CLibraryMaterial* pMaterialsLibrary, List<OGEMaterialData>& materials)
		{
			CMaterial* pMaterials = pMaterialsLibrary->pMaterials;
			CEffect* pEffects = pEffectsLibrary->pEffect;
			CImage* pImages = pImagesLibrary->pImages;
			uint32 countMaterials = pMaterialsLibrary->countMaterials;
			uint32 countEffects = pEffectsLibrary->countEffects;
			uint32 countImages = pImagesLibrary->countImages;
			
			for(uint32 materialIndex = 0; materialIndex < countMaterials; materialIndex++)
			{
				OGEMaterialData materialData;
				ZeroMemory(&materialData, sizeof(OGEMaterialData));

				CMaterial cmaterial = pMaterials[materialIndex];
				materialData.materialName = cmaterial.name;
				
				for(uint32 effectIndex = 0; effectIndex < countEffects; effectIndex++)
				{
					if("#"+pEffects[effectIndex].id == cmaterial.effectId)
					{
						CEffect effect = pEffects[effectIndex];
						CEffectMaterial* pEffectMaterial = effect.profileCommon.technique.pEffectMaterial;

						// set materials diffuse data
						if(pEffectMaterial->diffuse.pTexture != nullptr)
						{
							materialData.diffuseTexture.textureName = pEffectMaterial->diffuse.pTexture->textureId;

							for(uint32 i = 0; i < countImages; i++)
							{
								if(pImages[i].id == materialData.diffuseTexture.textureName)
								{
									materialData.diffuseTexture.texturePath = pImages[i].initFromUrl.url;
								}
							}	
						}

						// set materials specular data	
						if(pEffectMaterial->specular.pColor != nullptr)
						{	
							COLLADAColorToEngineColor(*pEffectMaterial->specular.pColor, materialData.specularAlbedo);
							materialData.specularPower = pEffectMaterial->shininess.value.value;
						}

						// set normal map 
						if(pEffectMaterial->emmision.pTexture != nullptr)
						{
							materialData.normalTexture.textureName = pEffectMaterial->emmision.pTexture->textureId;

							for(uint32 i = 0; i < countImages; i++)
							{
								if(pImages[i].id == materialData.normalTexture.textureName)
								{
									materialData.normalTexture.texturePath = pImages[i].initFromUrl.url;
								}
							}
						}
					}
				}

				materials.push_back(materialData);
			}
		}

		void Max13COLLADAAdapter::createCameras(CLibraryCameras* pLibraryCameras, List<OGECameraData*>& rCameraData)
		{
			
			for(uint32 i = 0; i < pLibraryCameras->countCameras; i++)
			{
				OGECameraData* pCameraData = new OGECameraData;
				CPerspective perspective = pLibraryCameras->pCameras[i].optics.perspectiveProjection;

				pCameraData->id = pLibraryCameras->pCameras[i].id;
				pCameraData->aspectRatio = perspective.aspectRatio;
				pCameraData->yfov = perspective.yfov;
				pCameraData->zfar = perspective.zfar;
				pCameraData->znear = perspective.znear;

				rCameraData.push_back(pCameraData);
			}
		}

		void Max13COLLADAAdapter::createLights(CLibraryLights* pLibraryLights, List<OGELightData*>& rLightData)
		{
			CDirectionalLight* pDirectionalLights = pLibraryLights->pDirectionalLights;
			CPointLight* pPointLights = pLibraryLights->pPointLights;
			CSpotlight* pSpotlights = pLibraryLights->pSpotlights;

			for(uint32 i = 0; i < pLibraryLights->countDirectionalLights; i++)
			{
				OGELightData* pLightData = new OGELightData;
				ZeroMemory(pLightData, sizeof(OGELightData));
				pLightData->pDirectionalLight = new OGEDirectionalLight;

				pLightData->id = pDirectionalLights[i].id;
				COLLADAColorToEngineColor(pDirectionalLights[i].lightColor, 
					pLightData->pDirectionalLight->lightColor);

				rLightData.push_back(pLightData);
			}

			for(uint32 i = 0; i < pLibraryLights->countPointLights; i++)
			{
				OGELightData* pLightData = new OGELightData;
				ZeroMemory(pLightData, sizeof(OGELightData));
				pLightData->pPointLight = new OGEPointLight;
				pLightData->pPointLight->position = Vector(0.0f, 0.0f, 0.0f, 1.0f);

				pLightData->id = pLibraryLights->pPointLights[i].id;
				COLLADAColorToEngineColor(pLibraryLights->pPointLights[i].lightColor, 
					pLightData->pPointLight->lightColor);

				pLightData->pPointLight->attenuation.constAttenuation = pPointLights[i].attenuation.constAttenuation;
				pLightData->pPointLight->attenuation.linearAttenuation = pPointLights[i].attenuation.linearAttenuation;
				pLightData->pPointLight->attenuation.quadraticAttenuation = pPointLights[i].attenuation.quadraticAttenuation;

				rLightData.push_back(pLightData);
			}

			for(uint32 i = 0; i < pLibraryLights->countSpotlights; i++)
			{
				OGELightData* pLightData = new OGELightData;
				ZeroMemory(pLightData, sizeof(OGELightData));
				pLightData->pSpotlight = new OGESpotlight;
				pLightData->pSpotlight->position = Vector(0.0f, 0.0f, 0.0f, 1.0f);

				pLightData->id = pSpotlights[i].id;
				COLLADAColorToEngineColor(pSpotlights[i].lightColor, pLightData->pSpotlight->lightColor);

				pLightData->pSpotlight->attenuation.constAttenuation = pSpotlights[i].attenuation.constAttenuation;
				pLightData->pSpotlight->attenuation.linearAttenuation = pSpotlights[i].attenuation.linearAttenuation;
				pLightData->pSpotlight->attenuation.quadraticAttenuation = pSpotlights[i].attenuation.quadraticAttenuation;

				pLightData->pSpotlight->falloffAngle = pSpotlights[i].falloffAngle;
				pLightData->pSpotlight->falloffExp = pSpotlights[i].falloffExp;

				rLightData.push_back(pLightData);
			}
		}

		void Max13COLLADAAdapter::COLLADAColorToEngineColor(const CColor& colorNode, ARGBColor& rColor)
		{
			rColor.setColorf(colorNode.value[3] > 0 ? colorNode.value[3] : 1.0f, colorNode.value[0], 
				colorNode.value[1], colorNode.value[2]);
		}

		void Max13COLLADAAdapter::releaseCOLLADAData(COLLADADocument* pColladaDocument)
		{
			// release effects data
			safe_array_release(pColladaDocument->pLibraryEffects->pEffect);
			safe_release(pColladaDocument->pLibraryEffects);
			
			// release images data
			safe_array_release(pColladaDocument->pLibraryImages)

			// release materials data
			safe_array_release(pColladaDocument->pLibraryMaterials->pMaterials);
			safe_release(pColladaDocument->pLibraryMaterials);

			// release geometry data
			uint32 countGeometries = pColladaDocument->pLibraryGeometries->countGeometries;
			for(uint32 i = 0; i < countGeometries; i++)
			{
				CMesh mesh = pColladaDocument->pLibraryGeometries->pGeometry[i].mesh;
				for(uint32 j = 0; j < mesh.countPrimitivesBuffers; j++)
				{
					safe_array_release(mesh.pPrimitivesBuffers[j].pInputs);
					safe_array_release(mesh.pPrimitivesBuffers[j].pPrimitive);
				}

				safe_array_release(mesh.pPrimitivesBuffers);
				safe_array_release(mesh.vertices.pInputSemantic);

				for(uint32 j = 0; j < mesh.sourcesCount; j++)
				{
					safe_array_release(mesh.pSources[j].arrayEllement.pData);
				}

				safe_array_release(mesh.pSources);
			}

			safe_array_release(pColladaDocument->pLibraryGeometries->pGeometry);
			safe_release(pColladaDocument->pLibraryGeometries);

			// release lights data
			if(pColladaDocument->pLibraryLights->pDirectionalLights)
			{
				safe_array_release(pColladaDocument->pLibraryLights->pDirectionalLights);
			}

			if(pColladaDocument->pLibraryLights->pPointLights)
			{
				safe_array_release(pColladaDocument->pLibraryLights->pPointLights);
			}

			if(pColladaDocument->pLibraryLights->pSpotlights)
			{
				safe_array_release(pColladaDocument->pLibraryLights->pSpotlights);
			}

			// release cameras data
			safe_array_release(pColladaDocument->pLibraryCameras->pCameras);
			
			// release scenes data
			uint32 countScenes = pColladaDocument->pLibraryVisualScenes->countScenes;
			for(uint32 i = 0; i < countScenes; i++)
			{
				uint32 countNodes = pColladaDocument->pLibraryVisualScenes->pScenes[i].countNodes;
				for(uint32 j = 0; j < countNodes; j++)
				{
					releaseCOLLADASceneNodeData(pColladaDocument->pLibraryVisualScenes->pScenes[i].ppNodes[j]);
					safe_release(pColladaDocument->pLibraryVisualScenes->pScenes[i].ppNodes[j]);
				}

				safe_array_release(pColladaDocument->pLibraryVisualScenes->pScenes[i].ppNodes);
			}

			safe_array_release(pColladaDocument->pLibraryVisualScenes->pScenes);
			safe_release(pColladaDocument->pLibraryVisualScenes);
			safe_release(pColladaDocument);
		}

		void Max13COLLADAAdapter::releaseCOLLADASceneNodeData(CNode* pNode)
		{
			safe_release(pNode->pInstanceCamera);
			safe_release(pNode->pInstanceLight);
			if(pNode->pInstanceGeometry  != NULL)
			{
				safe_array_release(pNode->pInstanceGeometry->pBindMaterial->pInstanceMaterials);
				safe_release(pNode->pInstanceGeometry->pBindMaterial);
				safe_release(pNode->pInstanceGeometry);
			}

			if(pNode->pMatrix)
			{
				safe_release(pNode->pMatrix);
			}
			if(pNode->pRTS)
			{
				safe_array_release(pNode->pRTS->pRotate);
				safe_array_release(pNode->pRTS->pScale);
				safe_array_release(pNode->pRTS->pTranslate);
				safe_release(pNode->pRTS);
			}

			ListIterator<CNode*> it = pNode->nodes.iterator();

			while(it.hasNext())
			{
				CNode* pNode = it.next();
				releaseCOLLADASceneNodeData(pNode);
				safe_release(pNode);
			}
		}
	}
}
