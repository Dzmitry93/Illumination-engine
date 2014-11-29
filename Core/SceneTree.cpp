#include "SceneTree.h"

namespace OGE
{
	namespace scene
	{
		void SceneTree::updateNode(TreeIterator<SceneNode*>& rWhereIt, const Matrix& rLocalMatrix)
		{
			// update node
			const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
			SceneNode* pCurrentNode = pTreeNode->data;

			if(pCurrentNode->getNodeType() != NODE_TYPE::NODE_TYPE_NOTHING)
			{
				TreeIterator<SceneNode*> parentIt = rWhereIt;
				parentIt.up();

				Matrix parentMatrix;
				if(!parentIt.isRoot())
				{
					SceneNode* pParentNode = rWhereIt.getCurrentNode()->data;
					SceneObjectNode* pObjectNode = (SceneObjectNode*)pParentNode;
					parentMatrix = pObjectNode->getWorldMatrix();
				}
				
				SceneObjectNode* pObjNode = ((SceneObjectNode*)pCurrentNode);
				pObjNode->updateNode(parentMatrix, rLocalMatrix);
			}

			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				updateNode(rWhereIt, rLocalMatrix);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::updateCamera(TreeIterator<SceneNode*>& rWhereIt,
				const Matrix& rTranslation, const Matrix& rRotation)
		{
			const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
			SceneNode* pCurrentNode = pTreeNode->data;

			if(pCurrentNode->getNodeType() != NODE_TYPE::NODE_TYPE_NOTHING)
			{
				TreeIterator<SceneNode*> parentIt = rWhereIt;
				parentIt.up();

				Matrix parentMatrix;
				if(!parentIt.isRoot())
				{
					SceneNode* pParentNode = rWhereIt.getCurrentNode()->data;
					SceneObjectNode* pObjectNode = (SceneObjectNode*)pParentNode;
					parentMatrix = pObjectNode->getWorldMatrix();
				}
				
				SceneCameraNode* pObjNode = ((SceneCameraNode*)pCurrentNode);
				pObjNode->updateNode(parentMatrix, rTranslation);
				pObjNode->updateCamera(pObjNode->getWorldMatrix(), rRotation);
			}
		}

		void SceneTree::traverseModels(TreeIterator<SceneNode*>& rWhereIt, 
			List<SceneObject<IntrusivePtr<BaseMesh>>>& rMeshes) const
		{
			if(!rWhereIt.isRoot())
			{
				const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
				SceneNode* pSceneNode = pTreeNode->data;

				if(pSceneNode->getNodeType() == NODE_TYPE_SYSTEM_MODEL)
				{
					SceneModelNode* pModelNode = (SceneModelNode*)pSceneNode;
					SceneObject<IntrusivePtr<BaseMesh>> object;
					object.setSceneObject(pModelNode->getModel());
					object.setOBB(pModelNode->getBoundingBody());
					object.setWorldMatrix(pModelNode->getWorldMatrix());
					object.setLocalMatrix(pModelNode->getLocalMatrix());

					rMeshes.push_back(object);
				}
			}
			
			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				traverseModels(rWhereIt, rMeshes);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::traverseBoundingBodys(TreeIterator<SceneNode*>& rWhereIt, List<SceneObject<OBB>>& rOBBs) const
		{
			if(!rWhereIt.isRoot())
			{
				const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
				SceneNode* pSceneNode = pTreeNode->data;

				if(pSceneNode->getNodeType() == NODE_TYPE_SYSTEM_MODEL)
				{
					SceneModelNode* pModelNode = (SceneModelNode*)pSceneNode;
					SceneObject<OBB> sceneObject;

					sceneObject.setSceneObject(pModelNode->getBoundingBody());
					sceneObject.setWorldMatrix(pModelNode->getWorldMatrix());
					sceneObject.setLocalMatrix(pModelNode->getLocalMatrix());

					rOBBs.push_back(sceneObject);
				}
			}
			
			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				traverseBoundingBodys(rWhereIt, rOBBs);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::traverseLights(
			TreeIterator<SceneNode*>& rWhereIt,
			uint32 lightType, 
			List<SceneObject<BaseLight*>>& rLights) const
		{
			if(!rWhereIt.isRoot())
			{
				const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
				SceneNode* pSceneNode = pTreeNode->data;

				if(pSceneNode->getNodeType() == NODE_TYPE_LIGHT)
				{
					SceneLightNode* pLightNode = (SceneLightNode*)pSceneNode;
					if(pLightNode->getLight()->getLightType() & lightType 
						|| lightType == LIGHT_TYPE::LIGHT_TYPE_ALL)
					{
						SceneObject<BaseLight*> sceneObject;

						sceneObject.setSceneObject(pLightNode->getLight());
						sceneObject.setOBB(pLightNode->getBoundingBody());
						sceneObject.setWorldMatrix(pLightNode->getWorldMatrix());
						sceneObject.setLocalMatrix(pLightNode->getLocalMatrix());

						rLights.push_back(sceneObject);
					}
				}
			}
			
			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				traverseLights(rWhereIt, lightType, rLights);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::traverseCameras(TreeIterator<SceneNode*>& rWhereIt,
			List<SceneObject<Camera>>& rCameras) const
		{
			if(!rWhereIt.isRoot())
			{
				const TreeNode<SceneNode*>* pTreeNode = rWhereIt.getCurrentNode();
				SceneNode* pSceneNode = pTreeNode->data;

				if(pSceneNode->getNodeType() == NODE_TYPE_CAMERA)
				{
					SceneCameraNode* pCameraNode = (SceneCameraNode*) pSceneNode;
					SceneObject<Camera> sceneObject;
					sceneObject.setSceneObject(pCameraNode->getCamera());
					sceneObject.setOBB(pCameraNode->getBoundingBody());
					sceneObject.setWorldMatrix(pCameraNode->getWorldMatrix());
					sceneObject.setLocalMatrix(pCameraNode->getLocalMatrix());

					rCameras.push_back(sceneObject);
				}
			}

			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				traverseCameras(rWhereIt, rCameras);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::findLight(TreeIterator<SceneNode*>& rWhereIt, 
				uint64 guid, bool& found, SceneObject<BaseLight*>& rLight) const
		{
			if(!rWhereIt.isRoot())
			{
				SceneNode* pCurrentSceneNode = rWhereIt.getCurrentNode()->data;
				if(NODE_TYPE_LIGHT == pCurrentSceneNode->getNodeType())
				{
					SceneLightNode* pNode = (SceneLightNode*)pCurrentSceneNode;
					if(pNode->getLight()->getGUID() == guid)
					{
						rLight.setSceneObject(pNode->getLight());
						rLight.setOBB(pNode->getBoundingBody());
						rLight.setWorldMatrix(pNode->getWorldMatrix());
						rLight.setLocalMatrix(pNode->getLocalMatrix());
						found = true;
					}
				}
			}
			
			if(!found)
			{
				while(rWhereIt.hasNext() && !found)
				{
					rWhereIt.down();
					findLight(rWhereIt, guid, found, rLight);
					rWhereIt.up();
					rWhereIt.next();
				}
			}
		}

		void SceneTree::findCamera(TreeIterator<SceneNode*>& rWhereIt, 
				uint64 guid, bool& found, SceneObject<Camera>& rCamera) const
		{
			if(!rWhereIt.isRoot())
			{
				SceneNode* pCurrentSceneNode = rWhereIt.getCurrentNode()->data;
				if(NODE_TYPE_CAMERA == pCurrentSceneNode->getNodeType())
				{
					SceneCameraNode* pNode = (SceneCameraNode*)pCurrentSceneNode;
					if(pNode->getCamera().getGUID() == guid)
					{
						rCamera.setSceneObject(pNode->getCamera());
						rCamera.setOBB(pNode->getBoundingBody());
						rCamera.setWorldMatrix(pNode->getWorldMatrix());
						rCamera.setLocalMatrix(pNode->getLocalMatrix());

						found = true;
					}
				}
			}
			
			if(!found)
			{		
				while(rWhereIt.hasNext() && !found)
				{
					rWhereIt.down();
					findCamera(rWhereIt, guid, found, rCamera);
					rWhereIt.up();
					rWhereIt.next();
				}
			}
		}

		void SceneTree::findMesh(TreeIterator<SceneNode*>& rWhereIt,
			uint64 guid, bool& found, SceneObject<IntrusivePtr<BaseMesh>>& rObject) const
		{

			if(!rWhereIt.isRoot())
			{
				SceneNode* pCurrentSceneNode = rWhereIt.getCurrentNode()->data;
				if(NODE_TYPE_SYSTEM_MODEL == pCurrentSceneNode->getNodeType())
				{
					SceneModelNode* pNode = (SceneModelNode*)pCurrentSceneNode;
					if(pNode->getModel()->getGUID() == guid)
					{
						rObject.setSceneObject(pNode->getModel());
						rObject.setOBB(pNode->getBoundingBody());
						rObject.setWorldMatrix(pNode->getWorldMatrix());
						rObject.setLocalMatrix(pNode->getLocalMatrix());

						found = true;
					}
				}
			}
			
			if(!found)
			{			
				while(rWhereIt.hasNext() && !found)
				{
					rWhereIt.down();
					findMesh(rWhereIt, guid, found, rObject);
					rWhereIt.up();
					rWhereIt.next();
				}
			}
		}

		void SceneTree::findObject(TreeIterator<SceneNode*>& rWhereIt, uint64 guid, 
			bool& found, TreeIterator<SceneNode*>& rIt)
		{
			if(!rWhereIt.isRoot())
			{
				SceneNode* pSceneNode = rWhereIt.getCurrentNode()->data;
				NODE_TYPE nodeType = pSceneNode->getNodeType();
				if(nodeType == NODE_TYPE_CAMERA)
				{
					SceneCameraNode* pCameraNode = (SceneCameraNode*)pSceneNode;
					if(pCameraNode->getCamera().getGUID()== guid)
					{
						rIt = rWhereIt;
						found = true;
					}
				}
				if(nodeType == NODE_TYPE_LIGHT)
				{
					SceneLightNode* pLightNode = (SceneLightNode*)pSceneNode;
					if(pLightNode->getLight()->getGUID() == guid)
					{
						rIt = rWhereIt;
						found = true;
					}
				}
				if(nodeType == NODE_TYPE_SYSTEM_MODEL)
				{
					SceneModelNode* pModelNode = (SceneModelNode*)pSceneNode;
					if(pModelNode->getModel()->getGUID() == guid)
					{
						rIt = rWhereIt;
						found = true;
					}
				}
			}

			while(rWhereIt.hasNext() && ! found)
			{
				rWhereIt.down();
				findObject(rWhereIt, guid, found, rIt);
				rWhereIt.up();
				rWhereIt.next();
			}
		}

		void SceneTree::deleteNodes(TreeIterator<SceneNode*>& rWhereIt)
		{
			if(!rWhereIt.isRoot())
			{
				delete rWhereIt.getCurrentNode()->data;
			}
			
			while(rWhereIt.hasNext())
			{
				rWhereIt.down();
				deleteNodes(rWhereIt);
				rWhereIt.up();
				rWhereIt.next();
			}
		}
	}
}