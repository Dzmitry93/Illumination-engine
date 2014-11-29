#ifndef __SCENE_TREE_H__
#define __SCENE_TREE_H__

#include "Tree.h"
#include "SceneTransformNode.h"
#include "SceneModelNode.h"
#include "SceneLightNode.h"
#include "SceneCameraNode.h"
#include "SceneIterableMeshNode.h"
#include "SceneObject.h"
#include "MeshFactory.h"
#include "BoundingBodyFactory.h"
#include "SceneObject.h"

using namespace OGE::Collections;
using namespace OGE::resource;
using OGE::util::BoundingBodyFactory;
using OGE::scene::SceneObject;

namespace OGE
{
	namespace scene
	{
		class SceneTree
		{
		public:
			SceneTree();
			~SceneTree();
			
			void addNode(TreeIterator<SceneNode*>& rWhereIt, SceneNode* pSceneNode);
			void updateNode(TreeIterator<SceneNode*>& rWhereIt, const Matrix& rLocalMatrix);
			void updateNode(TreeIterator<SceneNode*>& rWhereIt);
			void updateCamera(TreeIterator<SceneNode*>& rWhereIt,
				const Matrix& rTranslation, const Matrix& rRotation);

			void findLight(TreeIterator<SceneNode*>& rWhereIt, 
				uint64 guid, bool& found, SceneObject<BaseLight*>&) const;
			void findCamera(TreeIterator<SceneNode*>& rWhereIt, 
				uint64 guid, bool& found, SceneObject<Camera>&) const;
			void findMesh(TreeIterator<SceneNode*>& rWhereIt, 
				uint64 guid, bool& found, SceneObject<IntrusivePtr<BaseMesh>>& rRenderable) const;

			void findObject(TreeIterator<SceneNode*>& rWhereIt, uint64 guid, bool& found, TreeIterator<SceneNode*>& rIt);

			void traverseModels(TreeIterator<SceneNode*>& rWhereIt, List<SceneObject<IntrusivePtr<BaseMesh>>>& rMeshList) const;
			void traverseLights(TreeIterator<SceneNode*>& rWhereIt, uint32 lightType, List<SceneObject<BaseLight*>>& rLights) const;
			void traverseCameras(TreeIterator<SceneNode*>& rWhereIt, List<SceneObject<Camera>>& rCameras) const;
			void traverseBoundingBodys(TreeIterator<SceneNode*>& rWhereIt, List<SceneObject<OBB>>& rOBBs) const;

			TreeIterator<SceneNode*> root() const;

		private:
			Tree<SceneNode*> tree;
			void deleteNodes(TreeIterator<SceneNode*>& whereIt);
		};

		inline SceneTree::SceneTree() :
			tree(new SceneObjectNode(NODE_TYPE::NODE_TYPE_TRANSFORM, Matrix()))
		{
		}

		inline SceneTree::~SceneTree() 
		{ 
			deleteNodes(tree.root());
		}

		inline TreeIterator<SceneNode*> SceneTree::root() const
		{
			return tree.root();
		}

		inline void SceneTree::addNode(TreeIterator<SceneNode*>& rWhereIt, SceneNode* pSceneNode)
		{
			tree.insert(rWhereIt, pSceneNode);
			if(!rWhereIt.isRoot())
			{
				updateNode(rWhereIt);
			}
		}

		inline void SceneTree::updateNode(TreeIterator<SceneNode*>& rWhereIt)
		{
			SceneObjectNode* pNode = (SceneObjectNode*) rWhereIt.data();
			updateNode(rWhereIt, pNode->getWorldMatrix());
		}
	}
}

#endif

