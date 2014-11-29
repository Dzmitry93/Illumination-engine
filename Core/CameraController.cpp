#include "CameraController.h"

namespace OGE
{
	namespace scene
	{
		void CameraController::updateCamera(SceneObject<Camera>& rCameraSceneObject)
		{
			KeyboardController* pKeyboard = pInputManager->getKeyboardController();
			MouseController* pMouse = pInputManager->getMouserController();
			const Camera& rCamera = rCameraSceneObject.getSceneObject();
			
			const Vector& viewDir = rCamera.getViewDirection();
			const Vector& sideDir = rCamera.getSideDirection();

			Matrix rotationMatrix;
			Matrix translationMatrix;

			int32 x = pMouse->getPositionX();
			int32 y = pMouse->getPositionY();

			int32 xDiff = x - 0;
			int32 yDiff = y - 0;
			prevX = x;
			prevY = y;

			float angleX = x > 0 ? -mStepAngle : mStepAngle;
			float angleY = y > 0 ? -mStepAngle : mStepAngle;

			float step = mStep;

			if(rCamera.getHandedness() == HANDEDNESS_LEFT)
			{
				angleX = -angleX;
				angleY = -angleY;
				step = -mStep;
			}

			if(xDiff != 0)
			{
				stateGhanged = true;
				rotationMatrix *= Matrix::asRotationY(ToRadian(angleX));
			}
			if(yDiff != 0)
			{
				stateGhanged = true;
				rotationMatrix *= Matrix::asRotationX(ToRadian(angleY));
			}

			if(pKeyboard->isPressed(DIK_W))
			{
				stateGhanged = true;
				translationMatrix *= Matrix::asTranslation(viewDir.x * step, viewDir.y * step, viewDir.z * step);
			}
			if(pKeyboard->isPressed(DIK_S))
			{
				stateGhanged = true;
				translationMatrix *= Matrix::asTranslation(-viewDir.x * step, -viewDir.y * step, -viewDir.z * step);
			}
			if(pKeyboard->isPressed(DIK_A))
			{
				stateGhanged = true;
				translationMatrix *= Matrix::asTranslation(sideDir.x * mStep, sideDir.y * mStep, sideDir.z * mStep);
			}
			if(pKeyboard->isPressed(DIK_D))
			{
				stateGhanged = true;
				translationMatrix *= Matrix::asTranslation(-sideDir.x * mStep, -sideDir.y * mStep, -sideDir.z * mStep);
			}

			if(stateGhanged)
			{
				pSceneManager->updateActiveCamera(rCamera.getGUID(), translationMatrix, rotationMatrix);
				stateGhanged = false;
			}
		}
	}
}
