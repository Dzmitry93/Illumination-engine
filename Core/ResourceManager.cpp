#include "ResourceManager.h"

namespace OGE
{
	namespace resource
	{
		Scene* ResourceManager::loadScene(string& scenePath)
		{
			ModelLoader loader;
			List<OGEMaterialData> materialsData;
			List<OGEModelData*> modelsData;
			OGESceneData sceneData;
			
			loader.loadScene(scenePath, FILE_FORMAT::FILE_FORMAT_COLLADA, materialsData, 
				modelsData, sceneData);
			createMaterials(materialsData);
			createMeshes(modelsData);

			Scene* pScene = SceneFactory::createScene(&sceneData, meshes);

			releaseLoaderData(materialsData, modelsData, sceneData);

			return pScene;
		}

		void ResourceManager::createMaterials(List<OGEMaterialData>& rMaterials)
		{
			ListIterator<OGEMaterialData> it = rMaterials.iterator();

			while(it.hasNext())
			{
				OGEMaterialData& data = it.next();
				Material* pMaterial = MaterialFactory::createMaterial(data);
				materials.insert(std::pair<uint64, IntrusivePtr<Material>>(pMaterial->getGUID(), pMaterial));
			}
		}

		void ResourceManager::createMeshes(List<OGEModelData*>& rModelsData)
		{
			ListIterator<OGEModelData*> it = rModelsData.iterator();

			while(it.hasNext())
			{
				OGEModelData* pModelData = it.next();
				map<uint64, IntrusivePtr<Material>>::iterator materialsIt = materials.find( HashUtil::getHash(pModelData->materialID) );

				if(materialsIt != materials.end())
				{
					IntrusivePtr<Material> pMaterial = materialsIt->second;
					BaseMesh* pMesh = MeshFactory::createMesh(pModelData, pMaterial);
					uint64 guid = pMesh->getGUID();

					meshes.insert(std::pair<uint64, IntrusivePtr<BaseMesh>>(guid, pMesh));
				}
				else
				{
					error_msg("not found specified material");
				}
			};
		}

		void ResourceManager::releaseLoaderData(List<OGEMaterialData>& materialsData,
										List<OGEModelData*>& models, OGESceneData& sceneData)
		{
			ListIterator<OGEModelData*> modelIt = models.iterator();

			while(modelIt.hasNext())
			{
				OGEModelData* pModelData = modelIt.next();

				pModelData->pVerticesBuffer->release();
				pModelData->pIndexesBuffer->release();
				delete pModelData;
			}

			releaseLoaderSceneNode(sceneData.pRoot);
		}

		void ResourceManager::releaseLoaderSceneNode(OGESceneNode* pNode)
		{
			ListIterator<OGESceneNode*> sceneNodeIt = pNode->sceneNodes.iterator();

			while(sceneNodeIt.hasNext())
			{
				OGESceneNode* pChildNode = sceneNodeIt.next();
				releaseLoaderSceneNode(pChildNode);

				safe_release(pChildNode->pCameraData);
				safe_release(pChildNode->pLightData);
				safe_release(pChildNode);
			};
		}
	}
}