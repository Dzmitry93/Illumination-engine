#ifndef __CAMERA_CONTROLLER_H__
#define __CAMERA_CONTROLLER_H__

#include "InputManager.h"
#include "ICameraController.h"
#include "SceneManager.h"
#include "Camera.h"

using namespace OGE::input;

namespace OGE
{
	namespace scene
	{
		class CameraController
		{
		public:
			CameraController(const InputManager* pInputManager, SceneManager* pSceneManager);
			~CameraController();

			virtual void updateCamera(SceneObject<Camera>& rCamera);

			void setStep(float step);
			void setStepAngle(float gradus);

			float getStep() const;
			float getStepAngle() const;

		private:
			const InputManager* pInputManager;
			SceneManager* pSceneManager;

			float mStep;
			float mStepAngle;
			bool stateGhanged;

			int32 prevX;
			int32 prevY;
		};

		inline CameraController::CameraController(const InputManager* pInputManager, SceneManager* pSceneManager)
			: pInputManager(pInputManager), pSceneManager(pSceneManager), stateGhanged(false), prevX(0), prevY(0)
		{
		}

		inline CameraController::~CameraController()
		{
		}

		inline void CameraController::setStep(float step)
		{
			this->mStep = step;
		}

		inline void CameraController::setStepAngle(float gradus)
		{
			this->mStepAngle = gradus;
		}

		inline float CameraController::getStep() const
		{
			return mStep;
		}

		inline float CameraController::getStepAngle() const
		{
			return mStepAngle;
		}
	}
}

#endif

