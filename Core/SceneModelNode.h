#ifndef __SCENE_MODEL_NODE_H__
#define __SCENE_MODEL_NODE_H__

#include "SceneObjectNode.h"
#include "IntrusivePtr.h"
#include "Mesh.h"

using OGE::memory::IntrusivePtr;
using OGE::math::Matrix;
using OGE::resource::BaseMesh;

namespace OGE
{
	namespace scene
	{
		class SceneModelNode : public SceneObjectNode
		{
		public:
			SceneModelNode(const IntrusivePtr<BaseMesh>& pModel, const Matrix& rLocalMatrix);
			virtual ~SceneModelNode(void);

			IntrusivePtr<BaseMesh> getModel() const;

		private:

			IntrusivePtr<BaseMesh> pModel;
		};

		inline SceneModelNode::SceneModelNode(const IntrusivePtr<BaseMesh>& pModel, const Matrix& rLocalMatrix) 
				: SceneObjectNode(NODE_TYPE::NODE_TYPE_SYSTEM_MODEL, rLocalMatrix), pModel(pModel) {}
		inline SceneModelNode::~SceneModelNode(void) {} 
			
		inline IntrusivePtr<BaseMesh>  SceneModelNode::getModel() const
		{
			return pModel;
		}
	}
}

#endif
