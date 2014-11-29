#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "MeshFactory.h"
#include "SceneFactory.h"
#include "ModelLoader.h"
#include "MaterialFactory.h"
#include "HashUtil.h"
#include "ISystemManager.h"
#include <vector>
#include <map>
#include <string>

using std::map;
using std::string;
using std::vector;
using namespace OGE::util;
using namespace OGE::scene;
using namespace OGE::fileSystem;

namespace OGE
{
	namespace resource
	{
		class ResourceManager : public ISystemManager
		{
		public:
			~ResourceManager();

			Scene* loadScene(string& scenePath);
			void releaseResource(uint64 GUID);

			void start();
			void shutdown();

		private:
			map<uint64, IntrusivePtr<BaseMesh>> meshes;
			map<uint64, IntrusivePtr<Material>> materials;

			void createMeshes(List<OGEModelData*>& rModels);
			void createMaterials(List<OGEMaterialData>& rMaterials);
			void releaseLoaderData(List<OGEMaterialData>& materialsData, 
				List<OGEModelData*>& models, OGESceneData& sceneData);

			void releaseLoaderSceneNode(OGESceneNode* pSceneNode);
		};

		inline ResourceManager::~ResourceManager()
		{
			shutdown();
		}

		inline void ResourceManager::start()
		{
		}

		inline void ResourceManager::shutdown()
		{
		}
	}
}

#endif