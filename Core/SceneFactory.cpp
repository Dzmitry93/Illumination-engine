#include "SceneFactory.h"

namespace OGE
{
	namespace scene
	{
		void SceneFactory::createSceneFromData(OGESceneNode* pSceneData, TreeIterator<SceneNode*>& whereIt, 
			Scene* pScene,  map<uint64, IntrusivePtr<BaseMesh>>& rMeshes)
		{
			OGEModelData* pModelData = pSceneData->pModelData;
			OGECameraData* pCameraData = pSceneData->pCameraData;
			OGELightData* pLightData = pSceneData->pLightData;
			Matrix translation = pSceneData->translation;
			Matrix rotation = pSceneData->rotation;
			Matrix scale = pSceneData->scale;

			if(pSceneData->pParent != NULL)
			{
				if(pModelData != NULL)
				{
					uint64 meshGUID = HashUtil::getHash(pModelData->geometryName);
					map<uint64, IntrusivePtr<BaseMesh>>::iterator it = rMeshes.find(meshGUID);
					
					debug_assert(it != rMeshes.end(), "not found mesh");

					IntrusivePtr<BaseMesh> pMesh = it->second;
					IterableMesh* pIterableMesh = MeshFactory::createIterableMesh(pMesh);
					OBB obb = BoundingBodyFactory::createOBB(*pIterableMesh);

					pScene->addModelNode(whereIt, pMesh, obb, translation, rotation, scale);
				}
				else
				if(pLightData != NULL)
				{
					BaseLight* pLight = NULL;

					if(pLightData->pDirectionalLight)
					{
						pLight = LightFactory::createDirectionalLight(pLightData);
					}
					if(pLightData->pPointLight)
					{
						pLight = LightFactory::createPointLight(pLightData);
					}
					if(pLightData->pSpotlight)
					{
						pLight = LightFactory::createSpotLight(pLightData);
					}

					pScene->addLightNode(whereIt, pLight, translation, rotation, scale);
				}
				else
				if(pCameraData != NULL)
				{
					pScene->addCameraNode(whereIt, 
						CameraFactory::createCamera(pCameraData),
						translation, rotation, scale);
				}else
				{
					pScene->addTransformNode(whereIt, translation, rotation, scale);
				}
			}

			ListIterator<OGESceneNode*> sceneBeginIt = pSceneData->sceneNodes.iterator();

			while(sceneBeginIt.hasNext())
			{
				if(pSceneData->pParent != NULL)
				{
					whereIt.down();
				}
				createSceneFromData(sceneBeginIt.next(), whereIt, pScene, rMeshes);
			}

			whereIt.up();
		}
	}
}