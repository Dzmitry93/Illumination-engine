#ifndef __SCENE_H__
#define __SCENE_H__

/*
===============================================

	This file involves scene representation 
	functionality

================================================
*/

#include "SceneTree.h"
#include "SceneNodeFactory.h"
#include "List.h"
#include "BitFlags.h"
#include "MeshUtil.h"

using namespace OGE::Collections;
using OGE::util::MeshUtil;
using OGE::util::StringUtil;

namespace OGE
{
	namespace scene
	{
		enum SceneState
		{
			drawBB = 1
		};

		class Scene
		{
		public:
			Scene();
			~Scene();

			void addModelNode(TreeIterator<SceneNode*>& rWhereIt, 
				const IntrusivePtr<BaseMesh>& pModel,
				const OBB& rOBB,
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale);

			void addLightNode(TreeIterator<SceneNode*>&  rWhereIt, BaseLight* pLight,
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale);

			void addCameraNode(TreeIterator<SceneNode*>&  rWhereIt,
				const Camera& rCamera,
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale);

			void addTransformNode(TreeIterator<SceneNode*>& rWhereIt,
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale);

			void updateObject(uint64 guid, const Matrix& rTransformation);
			void updateCamera(uint64 guid, const Matrix& rTranslation, const Matrix& rRotation);

			SceneObject<BaseLight*> findLight(uint64 guid) const;
			SceneObject<Camera> findCamera(uint64 guid) const;
			SceneObject<IntrusivePtr<BaseMesh>> findMesh(uint64 guid) const;

			List<SceneObject<IntrusivePtr<BaseMesh>>> getAllModels() const;
			List<SceneObject<OBB>> getBoundingBodys() const;
			List<SceneObject<BaseLight*>> getLights(uint32 type) const;

			List<SceneObject<Camera>> getCameras() const;

			List<SceneObject<IntrusivePtr<BaseMesh>>> getBoundingBodysMeshes();

			TreeIterator<SceneNode*> root() const;

		private:
			SceneTree mSceneTree;
			map<uint64, SceneObject<IntrusivePtr<BaseMesh>>> boundingBoxMeshCache;
			map<uint64, BaseLight*> lightsCache;
		};

		inline Scene::Scene()  {}
		inline Scene::~Scene() {}

		inline TreeIterator<SceneNode*> Scene::root() const
		{
			return mSceneTree.root();
		}

		inline void Scene::addModelNode(
				TreeIterator<SceneNode*>& rWhereIt,
				const IntrusivePtr<BaseMesh>& pModel,
				const OBB& rOBB,
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale)
		{
			SceneNodeFactory nodeFactory;
			Matrix localMatrix;
			Matrix::MultiplyMatrix(localMatrix, rTranslation, rRotation);
			Matrix::MultiplyMatrix(localMatrix, localMatrix, rScale);
			SceneNode* pSceneNode = nodeFactory.createModelNode(pModel, localMatrix);
			((SceneObjectNode*) pSceneNode)->setBoundingBody(rOBB);

			mSceneTree.addNode(rWhereIt, pSceneNode);

			uint64 guid = pModel->getGUID();
			SceneObject<IntrusivePtr<BaseMesh>> sceneObject = findMesh(guid);
			IntrusivePtr<BaseMesh> pOBBMesh = MeshUtil::createOBBMesh(rOBB);
			sceneObject.setSceneObject(pOBBMesh);

			boundingBoxMeshCache.insert(std::pair<uint64, SceneObject<IntrusivePtr<BaseMesh>>>(guid, sceneObject));
		}

		inline void Scene::addTransformNode(TreeIterator<SceneNode*>& rWhereIt, 
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale)
		{
			SceneNodeFactory nodeFactory;
			Matrix localMatrix;
			Matrix::MultiplyMatrix(localMatrix, rTranslation, rRotation);
			Matrix::MultiplyMatrix(localMatrix, localMatrix, rScale);

			SceneNode* pSceneNode = nodeFactory.createTransformNode(localMatrix);
			mSceneTree.addNode(rWhereIt, pSceneNode);
		}

		inline void Scene::addLightNode(TreeIterator<SceneNode*>&  rWhereIt, BaseLight* pLight, 
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale)
		{
			SceneNodeFactory nodeFactory;
			Matrix localMatrix;
			Matrix::MultiplyMatrix(localMatrix, rTranslation, rRotation);
			Matrix::MultiplyMatrix(localMatrix, localMatrix, rScale);

			SceneNode* pSceneNode = nodeFactory.createLightNode(pLight, localMatrix);
			mSceneTree.addNode(rWhereIt, pSceneNode);
		}

		inline void Scene::addCameraNode(TreeIterator<SceneNode*>&  rWhereIt, const Camera& rCamera, 
				const Matrix& rTranslation,
				const Matrix& rRotation,
				const Matrix& rScale)
		{
			SceneNodeFactory nodeFactory;
			Matrix localMatrix;
			Matrix::MultiplyMatrix(localMatrix, rTranslation, rRotation);
			Matrix::MultiplyMatrix(localMatrix, localMatrix, rScale);

			SceneNode* pSceneNode = nodeFactory.createCameraNode(rCamera, localMatrix);
			mSceneTree.addNode(rWhereIt, pSceneNode);
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> Scene::getAllModels() const
		{
			List<SceneObject<IntrusivePtr<BaseMesh>>> meshList;
			mSceneTree.traverseModels(mSceneTree.root(), meshList);

			return meshList;
		}

		inline List<SceneObject<OBB>> Scene::getBoundingBodys() const
		{
			List<SceneObject<OBB>> bbs;
			mSceneTree.traverseBoundingBodys(mSceneTree.root(), bbs);

			return bbs;
		}

		inline List<SceneObject<BaseLight*>> Scene::getLights(uint32 type) const
		{
			List<SceneObject<BaseLight*>> lights;
			mSceneTree.traverseLights(mSceneTree.root(), type, lights);

			return lights;
		}

		inline List<SceneObject<Camera>> Scene::getCameras() const
		{
			List<SceneObject<Camera>> cameras;
			mSceneTree.traverseCameras(mSceneTree.root(), cameras);

			return cameras;
		}

		inline SceneObject<IntrusivePtr<BaseMesh>> Scene::findMesh(uint64 guid) const
		{
			SceneObject<IntrusivePtr<BaseMesh>> renderable; 
			bool found = false;
			mSceneTree.findMesh(root(), guid, found, renderable);

			return renderable;
		}

		inline SceneObject<BaseLight*> Scene::findLight(uint64 guid) const
		{
			SceneObject<BaseLight*> light;
			bool found = false;
			mSceneTree.findLight(root(), guid, found, light);

			return light;
		}

		inline SceneObject<Camera> Scene::findCamera(uint64 guid) const
		{
			SceneObject<Camera> cameraSceneObject;
			bool found = false;
			mSceneTree.findCamera(root(), guid, found, cameraSceneObject);

			return cameraSceneObject;
		}

		inline void Scene::updateObject(uint64 guid, const Matrix& rTransformation)
		{
			TreeIterator<SceneNode*> it = mSceneTree.root();
			bool found = false;
			mSceneTree.findObject(root(), guid, found, it);

			debug_error(found, "not found scene object with guid : "  + StringUtil::NumberToString(guid));

			mSceneTree.updateNode(it, rTransformation);	
		}

		inline void Scene::updateCamera(uint64 guid, const Matrix& rTranslation, const Matrix& rRotation)
		{
			TreeIterator<SceneNode*> it = mSceneTree.root();
			bool found = false;
			mSceneTree.findObject(root(), guid, found, it);
			
			debug_error(found, "not found scene object with guid : "  + StringUtil::NumberToString(guid));
	
			mSceneTree.updateCamera(it, rTranslation, rRotation);
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> Scene::getBoundingBodysMeshes()
		{
			List<SceneObject<IntrusivePtr<BaseMesh>>> renderableList;

			map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>::iterator it = boundingBoxMeshCache.begin();
			map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>::iterator end = boundingBoxMeshCache.end();

			while(it != end)
			{
				renderableList.push_back(it->second);
				it++;
			}

			return renderableList;
		}
	}
}

#endif

