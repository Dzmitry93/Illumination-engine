#ifndef __SCENE_OBJECT_NODE_H__
#define __SCENE_OBJECT_NODE_H__

#include "SceneNode.h"
#include "List.h"
#include "Matrix.h"
#include "OBB.h"

using namespace OGE::math;
using namespace OGE::Collections;

namespace OGE
{
	namespace scene
	{
		class SceneObjectNode : public SceneNode
		{
		public:
			SceneObjectNode(NODE_TYPE nodeType,
				const Matrix& rLocalMatrix);
			virtual ~SceneObjectNode();

			virtual void updateNode(const Matrix& rParentWorldMatrix, 
				const Matrix& rLocalMatrix);

			virtual void updateBoundingBody(
				const Matrix& rLocalMatrix);
			
			void setBoundingBody(const OBB& rBB);
			const OBB& getBoundingBody() const;

			const Matrix& getWorldMatrix() const;
			const Matrix& getLocalMatrix() const;

		protected: 
			
			Matrix worldMatrix;
			Matrix localMatrix;

			OBB bb;
		};

		inline SceneObjectNode::SceneObjectNode(NODE_TYPE nodeType,
				const Matrix& rLocalMatrix) : 
					SceneNode(nodeType),
							worldMatrix(rLocalMatrix),
							localMatrix(rLocalMatrix) {}

		inline SceneObjectNode::~SceneObjectNode() {}

		inline const Matrix& SceneObjectNode::getWorldMatrix() const
		{
			return worldMatrix;
		}

		inline const Matrix& SceneObjectNode::getLocalMatrix() const 
		{
			return localMatrix;
		}

		inline void SceneObjectNode::updateNode(const Matrix& rParentWorldMatrix, 
				const Matrix& rLocalMatrix)
		{
			this->localMatrix *= rLocalMatrix;

			Matrix::MultiplyMatrix(worldMatrix, rParentWorldMatrix, localMatrix);
			updateBoundingBody(localMatrix);
		}

		// TODO scale
		inline void SceneObjectNode::updateBoundingBody(
				const Matrix& rLocalMatrix)
		{
			bb.middlePoint *= rLocalMatrix;

			/*bb.planes[0].dir *= rLocalMatrix;
			bb.planes[1].dir *= rLocalMatrix;
			bb.planes[2].dir *= rLocalMatrix;
			bb.planes[3].dir *= rLocalMatrix;
			bb.planes[4].dir *= rLocalMatrix;
			bb.planes[5].dir *= rLocalMatrix; */
		}

		inline void SceneObjectNode::setBoundingBody(const OBB& rBB)
		{
			this->bb = rBB;
		}

		inline const OBB& SceneObjectNode::getBoundingBody() const
		{
			return bb;
		}
	}
}

#endif