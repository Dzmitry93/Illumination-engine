#include "Vector.h"

namespace OGE
{
	namespace math
	{
		Vector Vector::operator * (const Matrix& rMatrix) const
		{
			float x = this->x * rMatrix.data[0][0] + this->y * rMatrix.data[1][0] + 
				this->z * rMatrix.data[2][0] + this->w * rMatrix.data[3][0];

			float y = this->x * rMatrix.data[0][1] + this->y * rMatrix.data[1][1] + 
				this->z * rMatrix.data[2][1] + this->w * rMatrix.data[3][1];

			float z = this->x * rMatrix.data[0][2] + this->y * rMatrix.data[1][2] + 
				this->z * rMatrix.data[2][2] + this->w * rMatrix.data[3][2];

			float w = this->x * rMatrix.data[0][3] + this->y * rMatrix.data[1][3] + 
				this->z * rMatrix.data[2][3] + this->w * rMatrix.data[3][3];

			return Vector(x, y, z, w);
		}

		void Vector::operator *= (const Matrix& rMatrix)
		{
			float x = this->x * rMatrix.data[0][0] + this->y * rMatrix.data[1][0] + 
				this->z * rMatrix.data[2][0] + this->w * rMatrix.data[3][0];

			float y = this->x * rMatrix.data[0][1] + this->y * rMatrix.data[1][1] + 
				this->z * rMatrix.data[2][1] + this->w * rMatrix.data[3][1];

			float z = this->x * rMatrix.data[0][2] + this->y * rMatrix.data[1][2] + 
				this->z * rMatrix.data[2][2] + this->w * rMatrix.data[3][2];

			float w = this->x * rMatrix.data[0][3] + this->y * rMatrix.data[1][3] + 
				this->z * rMatrix.data[2][3] + this->w * rMatrix.data[3][3];

			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
	}
}