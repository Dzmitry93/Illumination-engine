#ifndef _CAMERA_H__
#define _CAMERA_H__

#include "Matrix.h"
#include "Vector.h"
#include "MathUtils.h"
#include "ResourceItem.h"
#include "ViewFrustum.h"

using namespace OGE::math;
using OGE::resource::ResourceItem;

namespace OGE
{
	namespace scene
	{
		enum HANDEDNESS
		{
			HANDEDNESS_LEFT,
			HANDEDNESS_RIGHT
		};

		class Camera : public ResourceItem
		{
		public:

			Camera();
			Camera(HANDEDNESS handedness);
			Camera(const Camera& rOther);
			~Camera();

			HANDEDNESS getHandedness() const;

			void setPerspective(float fovy, float aspectRatio, float zNear, float zFar);
			void lookAt(const Vector& pos, const Vector& at, const Vector& dir);

			void update(const Matrix& rTranslation, const Matrix& rRotation);

			Matrix getWorldMatrix() const;
			const Matrix& getViewMatrix() const;
			const Matrix& getProjectionMatrix() const;

			const Vector& getViewDirection() const;
			const Vector& getSideDirection()  const;

			const Vector& getWorldSpacePosition() const;
			Vector getCameraSpacePosition() const;

			const ViewFrustum& getViewFrustum() const;

			float getFocalLength() const;
			float getAspectRatio() const;
			
			void operator = (const Camera& rOther);
			bool operator == (const Camera& rOther) const;

		private:
			Vector position;
			Matrix viewMatrix;
			Matrix projectionMatrix;

			Vector viewDir;
			Vector sideDir;

			ViewFrustum viewFrustum;

			HANDEDNESS handedness;
			
			void destroyObject();
			void shallowCopy(const ResourceItem& rOther);
		};

		inline Camera::Camera() : 
			handedness(HANDEDNESS_LEFT) {}

		inline Camera::Camera(HANDEDNESS handedness) : 
			handedness(handedness) {}

		inline Camera::Camera(const Camera& rOther)	:
			viewFrustum(rOther.viewFrustum)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline Camera::~Camera() {}

		inline void Camera::destroyObject() {}

		inline void Camera::operator = (const Camera& rOther)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline void Camera::setPerspective(float fovy, float aspectRatio, float zNear, float zFar)

		{
			if(handedness == HANDEDNESS_LEFT)
			{
				projectionMatrix = MatrixUtils::CreateProjectionMatrixLH(fovy, aspectRatio, zNear, zFar);
			}
			else
			{
				projectionMatrix = MatrixUtils::CreateProjectionMatrixRH(fovy, aspectRatio, zNear, zFar);
			}

			viewFrustum = ViewFrustum(viewMatrix * projectionMatrix);
		}

		inline void Camera::lookAt(const Vector& eye, const Vector& at, const Vector& up)
		{
			if(handedness == HANDEDNESS_LEFT)
			{
				viewMatrix = MatrixUtils::CreateViewMatrixLH(eye, at, up);
			}
			else
			{
				viewMatrix = MatrixUtils::CreateViewMatrixRH(eye, at, up);
			}

			position = eye;

			viewDir.x = viewMatrix.data[0][2];
			viewDir.y = viewMatrix.data[1][2];
			viewDir.z = viewMatrix.data[2][2];

			sideDir.x = viewMatrix.data[0][0];
			sideDir.y = viewMatrix.data[1][0];
			sideDir.z = viewMatrix.data[2][0];
		}

		inline Matrix Camera::getWorldMatrix() const
		{
			return Matrix::asTranslation(position);
		}

		inline const Vector& Camera::getWorldSpacePosition() const
		{
			return position;
		}

		inline Vector Camera::getCameraSpacePosition() const
		{
			return Vector(viewMatrix.data[3][0], viewMatrix.data[3][1],
				viewMatrix.data[3][2], viewMatrix.data[3][3]);
		}

		inline const Matrix& Camera::getViewMatrix() const
		{
			return viewMatrix;
		}

		inline const Matrix& Camera::getProjectionMatrix() const
		{
			return projectionMatrix;
		}

		inline float Camera::getFocalLength() const
		{
			return projectionMatrix.data[1][1];
		}

		inline float Camera::getAspectRatio() const
		{
			return projectionMatrix.data[1][1] / projectionMatrix.data[0][0];
		}

		inline HANDEDNESS Camera::getHandedness() const
		{
			return handedness;
		}

		inline const Vector& Camera::getViewDirection() const
		{
			return viewDir;
		}

		inline const Vector& Camera::getSideDirection() const
		{
			return sideDir;
		}

		inline const ViewFrustum& Camera::getViewFrustum() const
		{
			return viewFrustum;
		}

		inline void Camera::update(const Matrix& rTranslation, const Matrix& rRotationMatrix)
		{
			Matrix updateMatrix;
			
			
			Matrix::MultiplyMatrix(viewMatrix, viewMatrix, rRotationMatrix);
			Matrix::MultiplyMatrix(updateMatrix, updateMatrix, rTranslation);
			Matrix::MultiplyMatrix(updateMatrix, updateMatrix, viewMatrix);
			Matrix::MultiplyMatrix(updateMatrix, updateMatrix, projectionMatrix);

			viewFrustum.update(updateMatrix);

			viewDir.x = viewMatrix.data[0][2];
			viewDir.y = viewMatrix.data[1][2];
			viewDir.z = viewMatrix.data[2][2];

			sideDir.x = viewMatrix.data[0][0];
			sideDir.y = viewMatrix.data[1][0];
			sideDir.z = viewMatrix.data[2][0];
		}

		inline bool Camera::operator == (const Camera& rOther) const
		{
			return compare(rOther) == 0 ? false : true;
		}

		inline void Camera::shallowCopy(const ResourceItem& rOther)
		{
			ResourceItem::shallowCopy(rOther);

			Camera& rTemp = (Camera&) rOther;
			this->viewMatrix = rTemp.viewMatrix;
			this->projectionMatrix = rTemp.projectionMatrix;
			this->viewDir = rTemp.viewDir;
			this->sideDir = rTemp.sideDir;
			this->position = rTemp.position;
			this->handedness = rTemp.handedness;
			this->viewFrustum = rTemp.viewFrustum;
		}
	}
}

#endif
