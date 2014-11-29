#ifndef __SCENE_FACTORY_H__
#define __SCENE_FACTORY_H__

#include "ImportStructures.h"
#include "Scene.h"
#include "List.h"
#include "Material.h"
#include "IntrusivePtr.h"
#include "MeshFactory.h"
#include "LightFactory.h"
#include "CameraFactory.h"
#include <map>

using namespace OGE::fileSystem;
using namespace OGE::Collections;
using namespace OGE::resource;
using OGE::memory::IntrusivePtr;

using std::map;

namespace OGE
{
	namespace scene
	{
		class SceneFactory
		{
		public:
			
			static Scene* createScene(OGESceneData* pSceneData, map<uint64, IntrusivePtr<BaseMesh>>& rMeshes);

		private:

			static void createSceneFromData(OGESceneNode* pSceneNode, TreeIterator<SceneNode*>& root, 
				Scene* pScene, map<uint64, IntrusivePtr<BaseMesh>>& rMeshes);
		};

		inline Scene* SceneFactory::createScene(OGESceneData* pSceneData, map<uint64, IntrusivePtr<BaseMesh>>& rMeshes)
		{
			Scene* pScene = new Scene();
			createSceneFromData(pSceneData->pRoot, pScene->root(), pScene, rMeshes);

			return pScene;
		}
	}
}

#endif

