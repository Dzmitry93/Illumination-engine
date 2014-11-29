#ifndef __SCENE_CAMERA_NODE_H__
#define __SCENE_CAMERA_NODE_H__

#include "SceneObjectNode.h"
#include "Camera.h"
#include "Matrix.h"

using OGE::math::Matrix;

namespace OGE
{
	namespace scene
	{
		class SceneCameraNode : public SceneObjectNode
		{
		public:
			SceneCameraNode(const Camera& rCamera, const Matrix& rLocalMatrix);
			virtual ~SceneCameraNode(void);

			void updateCamera(const Matrix& rTranslation, const Matrix& rRotationMatrix);

			Camera& getCamera();

		private:
			Camera camera;
		};

		inline SceneCameraNode::SceneCameraNode(const Camera& rCamera, const Matrix& rLocalMatrix) : 
			SceneObjectNode(NODE_TYPE_CAMERA, rLocalMatrix), camera(rCamera) {}

		inline SceneCameraNode::~SceneCameraNode() {}

		inline Camera& SceneCameraNode::getCamera()
		{
			return camera;
		}

		inline void SceneCameraNode::updateCamera(const Matrix& rTranslation, const Matrix& rRotation)
		{
			camera.update(rTranslation, rRotation);
		}
	}
}

#endif