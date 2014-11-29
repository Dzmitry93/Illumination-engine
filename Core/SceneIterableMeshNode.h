#pragma once
#ifndef __SCENE_ITERABLE_MESH_NODE_H__
#define __SCENE_ITERABLE_MESH_NODE_H__

#include "SceneObjectNode.h"
#include "IntrusivePtr.h"
#include "IterableMesh.h"

using OGE::memory::IntrusivePtr;
using OGE::math::Matrix;
using OGE::resource::IterableMesh;

namespace OGE
{
	namespace scene
	{
		class SceneIterableMeshNode : public SceneObjectNode
		{
		public:
			SceneIterableMeshNode(const IntrusivePtr<IterableMesh>& pModel, const Matrix& rLocalMatrix);
			virtual ~SceneIterableMeshNode(void);

			virtual void updateNode(const Matrix& rParentWorldMatrix, const Matrix& rLocalMatrix);
			IntrusivePtr<IterableMesh> getModel() const;

		private:

			IntrusivePtr<IterableMesh> pModel;
		};
		
		inline SceneIterableMeshNode::SceneIterableMeshNode(const IntrusivePtr<IterableMesh>& pModel, const Matrix& rLocalMatrix)
				: SceneObjectNode(NODE_TYPE::NODE_TYPE_ITERABLE_MODEL, rLocalMatrix), pModel(pModel){}

		inline SceneIterableMeshNode::~SceneIterableMeshNode(void) {} 

		inline void SceneIterableMeshNode::updateNode(const Matrix& rParentWorldMatrix, const Matrix& rLocalMatrix)
		{
			SceneObjectNode::updateNode(rParentWorldMatrix, rLocalMatrix);
			this->pModel->createIterableData(getWorldMatrix());
		}
			
		inline IntrusivePtr<IterableMesh>  SceneIterableMeshNode::getModel() const
		{
			return pModel;
		}
		
	}
}

#endif
