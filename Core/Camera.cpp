#include "Camera.h"

namespace OGE
{
	namespace scene
	{
	
		void Camera::setPerspective(float fovy, float aspectRatio, float zNear, float zFar)

		{
			if(handedness == HANDEDNESS_LEFT)
			{
				projectionMatrix = MatrixUtils::CreateProjectionMatrixLH(fovy, aspectRatio, zNear, zFar);
			}
			else
			{
				projectionMatrix = MatrixUtils::CreateProjectionMatrixRH(fovy, aspectRatio, zNear, zFar);
			}
		}

		void Camera::lookAt(const Vector& eye, const Vector& at, const Vector& up)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				viewMatrix = MatrixUtils::CreateViewMatrixLH(eye, at, up);
			}
			else
			{
				viewMatrix = MatrixUtils::CreateViewMatrixRH(eye, at, up);
			}

			worldMatrix = Matrix::asTranslation(eye.x, eye.y, eye.z);
		}

		Vector Camera::getViewDirection() const
		{
			return Vector(viewMatrix.data[0][2],
				viewMatrix.data[1][2],
				viewMatrix.data[2][2]);
		}

		Vector Camera::getPosition() const
		{
			return Vector(worldMatrix.data[0][3],
				worldMatrix.data[1][3],
				worldMatrix.data[2][3],
				worldMatrix.data[3][3]);
		}

		void Camera::rotateX(float degree)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				degree = -degree;
			}
			viewMatrix *= Matrix::asRotationX(ToRadian(degree));
		}

		void Camera::rotateY(float degree)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				degree = -degree;
			}
			viewMatrix *= Matrix::asRotationY(ToRadian(degree));
		} 

		void Camera::moveForward(float steps)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				steps = -steps;
			}

			worldMatrix *= Matrix::asTranslation(steps * viewMatrix.data[0][2],
												 steps * viewMatrix.data[1][2],
												 steps * viewMatrix.data[2][2]);
		}

		void Camera::moveBack(float steps)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				steps = -steps;
			}

			worldMatrix *= Matrix::asTranslation(steps * -viewMatrix.data[0][2],
												 steps * -viewMatrix.data[1][2],
												 steps * -viewMatrix.data[2][2]);
		}

		void Camera::moveLeft(float steps)
		{
			worldMatrix *= Matrix::asTranslation(steps * viewMatrix.data[0][0],
												 steps * viewMatrix.data[1][0],
												 steps * viewMatrix.data[2][0]);
		}

		void Camera::moveRight(float steps)
		{
			worldMatrix *= Matrix::asTranslation(steps * -viewMatrix.data[0][0],
												 steps * -viewMatrix.data[1][0],
												 steps * -viewMatrix.data[2][0]);
		}
	}
}