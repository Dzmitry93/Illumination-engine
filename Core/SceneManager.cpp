#include "SceneManager.h"

namespace OGE
{
	namespace scene
	{
		uint32 SceneManager::DEFAULT_OCTREE_NODE_SIZE = 5;

		void SceneManager::setActiveScene(Scene* pScene)
		{
			this->pScene = pScene;

			OctreeFactory octreeFactory;
			List<SceneObject<IntrusivePtr<BaseMesh>>> meshes = getMeshes();
			List<SceneObject<BaseLight*>> lights = getLights(LIGHT_TYPE_POINT | LIGHT_TYPE_SPOTLIGHT);

			// calculate scene aabb for octree
			List<SceneObject<OBB>> obbs = getBoundingVolumes();
			AABB rootAABB = BoundingBodyFactory::createAABB(obbs);
			
			//this->pOctree = octreeFactory.createOctree(meshes, rootAABB, DEFAULT_OCTREE_NODE_SIZE);
			this->pBVH = BVHFactory::createBVH(meshes, lights, rootAABB);
		}
	}
} 