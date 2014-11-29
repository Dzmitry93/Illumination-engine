#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Camera.h"
#include "LightFactory.h"
#include "Scene.h"	
#include "ISystemManager.h"
#include "OctreeFactory.h"
#include "TransformUtils.h"
#include "MeshUtil.h"
#include "BVHFactory.h"		   

using namespace OGE::Collections;

using OGE::util::BoundingBodyUtils;

using std::pair;

using namespace OGE::render;

namespace OGE
{
	namespace scene
	{
		class SceneManager : public ISystemManager
		{
		public:
			SceneManager();
			~SceneManager();

			void start();
			void shutdown();

			SceneObject<Camera> getActiveCamera() const;
			List<SceneObject<BaseLight*>> getLights(uint32 type) const;
			List<SceneObject<IntrusivePtr<BaseMesh>>> getMeshes();

			List<SceneObject<IntrusivePtr<BaseMesh>>> getViewFrustumMeshes();
			void getViewFrustumLights(List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData);

			List<SceneObject<OBB>> getBoundingVolumes();
			List<OBB> getViewFrustumBoundingVolumes();

			List<SceneObject<IntrusivePtr<BaseMesh>>> getBoundingVolumeMeshes();

			void updateNode(uint64 guid, const Matrix& rTransformation);
			void updateActiveCamera(uint64 guid, const Matrix& rTranslation, const Matrix& rRotation);

			void setActiveScene(Scene* pScene);

		private:
			Scene* pScene;
			Octree* pOctree;
			BVH* pBVH;

			Camera defaultCamera;

			static uint32 DEFAULT_OCTREE_NODE_SIZE;
		};

		inline SceneManager::SceneManager()
			: defaultCamera(HANDEDNESS_LEFT)
		{
		}

		inline SceneManager::~SceneManager()
		{
			shutdown();
		}

		inline void SceneManager::start()
		{
			defaultCamera.lookAt(
							Vector(0.0f, 0.0f, -1.0f, 1.0f), 
							Vector(0.0f, 0.0f, 1.0f, 1.0f), 
							Vector(0.0f, 1.0f, 0.0f, 1.0f));
			defaultCamera.setPerspective(45, 1366 / 768.0f, 0.1f, 10000.0f);
		}

		inline void SceneManager::shutdown()
		{
		}

		inline SceneObject<Camera> SceneManager::getActiveCamera() const
		{
			List<SceneObject<Camera>> cameras = pScene->getCameras();

			if(cameras.size() > 0)
			{
				return cameras.iterator().next();
			}
			else
			{
				SceneObject<Camera> camera;
				camera.setSceneObject(defaultCamera);
				return camera;
			}
		}

		inline List<SceneObject<OBB>> SceneManager::getBoundingVolumes()
		{
			return pScene->getBoundingBodys();
		}

		inline List<OBB> SceneManager::getViewFrustumBoundingVolumes()
		{
			return pBVH->getViewFrustumVolumes(getActiveCamera().getSceneObject().getViewFrustum());
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> SceneManager::getViewFrustumMeshes()
		{
			return pBVH->getViewFrustumObjects(getActiveCamera().getSceneObject().getViewFrustum());
		}

		inline void SceneManager::getViewFrustumLights(List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData)
		{
			List<OBB> volumes = pBVH->getViewFrustumVolumes(getActiveCamera().getSceneObject().getViewFrustum());

			AABB aabb = BoundingBodyFactory::createAABB(volumes.iterator());

			Vector center = aabb.centerPoint;
			float radius = sqrt(aabb.halfDistance.x * aabb.halfDistance.x +
				aabb.halfDistance.y * aabb.halfDistance.y +
				aabb.halfDistance.z * aabb.halfDistance.z) + 10000;

			pBVH->getLightsData(center, radius, rLightsData);
		}

		inline List<SceneObject<BaseLight*>> SceneManager::getLights(uint32 type) const
		{
			return pScene->getLights(type);
		}

		inline void SceneManager::updateNode(uint64 guid, const Matrix& rTransformation)
		{
			pScene->updateObject(guid, rTransformation);
		}

		inline void SceneManager::updateActiveCamera(uint64 guid, const Matrix& rTranslation, const Matrix& rRotation)
		{
			pScene->updateCamera(guid, rTranslation, rRotation);
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> SceneManager::getMeshes()
		{
			return pScene->getAllModels();
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> SceneManager::getBoundingVolumeMeshes()
		{
			SceneObject<Camera>& rCameraObject = getActiveCamera();
			Matrix update;

			Matrix::MultiplyMatrix(update, rCameraObject.getWorldMatrix(), rCameraObject.getSceneObject().getViewMatrix());
			Matrix::MultiplyMatrix(update, update, rCameraObject.getSceneObject().getProjectionMatrix());

		/*	ViewFrustum viewfrustum(update);

			SceneObject<IntrusivePtr<BaseMesh>> renderableObject;
			IntrusivePtr<BaseMesh> pViewfrustumMesh = MeshUtil::createViewFrustumMesh(viewfrustum);
			renderableObject.setSceneObject(pViewfrustumMesh);	   */

			List<SceneObject<IntrusivePtr<BaseMesh>>> objects = pScene->getBoundingBodysMeshes();
		//	objects.push_back(renderableObject);

		/*	List<AABB> aabbList = pOctree->getAABBNodes();
			ListIterator<AABB> it = aabbList.iterator();

			while(it.hasNext())
			{
				AABB aabb = it.next();
				BaseMesh* pMesh = MeshUtil::createAABBMesh(aabb);

				SceneObject<IntrusivePtr<BaseMesh>> object;
				object.setSceneObject(pMesh);

				objects.push_back(object);
			}	 				*/
			
			List<SceneObject<IntrusivePtr<BaseMesh>>> resultList;
			List<AABB> aabbList = pBVH->getAABBNodes();
			ListIterator<AABB> it = aabbList.iterator();

			while(it.hasNext())
			{
				BaseMesh* pMesh = MeshUtil::createAABBMesh(it.next());

				SceneObject<IntrusivePtr<BaseMesh>> object;
				object.setSceneObject(pMesh);

				objects.push_back(object);
			}	   

			return resultList;
		}
	}
}

#endif

