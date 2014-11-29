#ifndef __SCENE_TRANSFORM_NODE_H__
#define __SCENE_TRANSFORM_NODE_H__

#include "SceneObjectNode.h"

namespace OGE
{
	namespace scene
	{
		class SceneTransformNode : public SceneObjectNode
		{ 
		public:
			SceneTransformNode(const Matrix& rLocalMatrix);
			virtual ~SceneTransformNode();

		};

		inline SceneTransformNode::SceneTransformNode(
				const Matrix& rLocalMatrix)
				: SceneObjectNode(NODE_TYPE::NODE_TYPE_TRANSFORM, rLocalMatrix)  {} 

		inline SceneTransformNode::~SceneTransformNode(void) {} 

	}
}

#endif