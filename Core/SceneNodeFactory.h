#ifndef __SCENE_NODE_FACTORY_H__
#define __SCENE_NODE_FACTORY_H__

#include "SceneModelNode.h"
#include "SceneTransformNode.h"
#include "SceneIterableMeshNode.h"
#include "SceneLightNode.h"
#include "SceneCameraNode.h"

namespace OGE
{
	namespace scene
	{
		class SceneNodeFactory
		{
		public:

			SceneNode* createModelNode(const IntrusivePtr<BaseMesh>& pModel, const Matrix& rLocalMatrix);

			SceneNode* createIterableMeshNode(const IntrusivePtr<IterableMesh>& pModel, const Matrix& rLocalMatrix);

			SceneNode* createTransformNode(const Matrix& rLocalMatrix);

			SceneNode* createLightNode(BaseLight* pLight, const Matrix& rLocalMatrix);

			SceneNode* createCameraNode(const Camera& rCamera, const Matrix& rLocalMatrix);
		};

		inline SceneNode* SceneNodeFactory::createIterableMeshNode(const IntrusivePtr<IterableMesh>& pModel, const Matrix& rLocalMatrix)
		{
			return new SceneIterableMeshNode(pModel, rLocalMatrix);
		}

		inline SceneNode* SceneNodeFactory::createModelNode(const IntrusivePtr<BaseMesh>& pModel, const Matrix& rLocalMatrix)
		{
			return new SceneModelNode(pModel, rLocalMatrix);
		}

		inline SceneNode* SceneNodeFactory::createTransformNode(const Matrix& rLocalMatrix)
		{
			return new SceneTransformNode(rLocalMatrix);
		}

		inline SceneNode* SceneNodeFactory::createLightNode(BaseLight* pLight, const Matrix& rLocalMatrix)
		{
			return new SceneLightNode(pLight, rLocalMatrix);
		}

		inline SceneNode* SceneNodeFactory::createCameraNode(const Camera& rCamera, const Matrix& rLocalMatrix)
		{
			return new SceneCameraNode(rCamera, rLocalMatrix);
		}
	}
}

#endif