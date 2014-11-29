#include "Matrix.h"

namespace OGE
{
	namespace math
	{
		Matrix::Matrix(const Vector& rRow, 
				const Vector& rRow2,
				const Vector& rRow3, 
				const Vector& rRow4)
		{
			data[0][0] = rRow.x;
			data[0][1] = rRow.y;
			data[0][2] = rRow.z;
			data[0][3] = rRow.w;

			data[1][0] = rRow2.x;
			data[1][1] = rRow2.y;
			data[1][2] = rRow2.z;
			data[1][3] = rRow2.w;

			data[2][0] = rRow3.x;
			data[2][1] = rRow3.y;
			data[2][2] = rRow3.z;
			data[2][3] = rRow3.w;

			data[3][0] = rRow4.x;
			data[3][1] = rRow4.y;
			data[3][2] = rRow4.z;
			data[3][3] = rRow4.w;
		}

		Vector Matrix::operator * (const Vector& rVector) const
		{
			float x = data[0][0] * rVector.x +  data[0][1] * rVector.y +  
				data[0][2] * rVector.z +  data[0][3] * rVector.w;
			float y = data[1][0] * rVector.x +  data[1][1] * rVector.y +  
				data[1][2] * rVector.z +  data[1][3] * rVector.w;
			float z = data[2][0] * rVector.x +  data[2][1] * rVector.y +  
				data[2][2] * rVector.z +  data[2][3] * rVector.w;
			float w = data[3][0] * rVector.x +  data[3][1] * rVector.y +  
				data[3][2] * rVector.z +  data[3][3] * rVector.w;
			
			return Vector(x, y, z, w);
		}

		Matrix Matrix::asTranslation(const Vector& v)
		{
			Matrix temp;
			temp.data[3][0] = v.x;
			temp.data[3][1] = v.y;
			temp.data[3][2] = v.z;

			return temp;
		}

		Matrix Matrix::asScale(const Vector& v)
		{
			Matrix temp;
			temp.data[0][0] = v.x;
			temp.data[1][1] = v.y;
			temp.data[2][2] = v.z;

			return temp;
		}

		void Matrix::setRows(const Vector& v, const Vector& v2, const Vector& v3, const Vector& v4)
		{
			data[0][0] = v.x;
			data[0][1] = v.y;
			data[0][2] = v.z;
			data[0][3] = v.w;

			data[1][0] = v2.x;
			data[1][1] = v2.y;
			data[1][2] = v2.z;
			data[1][3] = v2.w;

			data[2][0] = v3.x;
			data[2][1] = v3.y;
			data[2][2] = v3.z;
			data[2][3] = v3.w;

			data[3][0] = v4.x;
			data[3][1] = v4.y;
			data[3][2] = v4.z;
			data[3][3] = v4.w;
		}	

		void Matrix::setColumns(const Vector& v, const Vector& v2, const Vector& v3, const Vector& v4)
		{
			data[0][0] = v[0];
			data[1][0] = v[1];
			data[2][0] = v[2];
			data[3][0] = v[3];

			data[0][1] = v2[0];
			data[1][1] = v2[1];
			data[2][1] = v2[2];
			data[3][1] = v2[3];

			data[0][2] = v3[0];
			data[1][2] = v3[1];
			data[2][2] = v3[2];
			data[3][2] = v3[3];

			data[0][3] = v4[0];
			data[1][3] = v4[1];
			data[2][3] = v4[2];
			data[3][3] = v4[3];
		}

		Vector Matrix::getRow(uint32 index) const
		{
			debug_assert(index >= 0 && index < 4, "incorrect row index");
			return Vector(data[index][0], data[index][1], data[index][2], data[index][3]);
		}

		Vector Matrix::getColumn(uint32 index) const
		{
			debug_assert(index >= 0 && index < 4, "incorrect row index");
			return Vector(data[0][index], data[1][index], data[2][index], data[3][index]);
		}

		Matrix Matrix::asRotationArbitraryAxis(const Vector& axis, float radians)
		{
			Matrix rotationMatrix;
			
			float c = cos(radians);
			float s = sin(radians);

			rotationMatrix.data[0][0] = c + (1 - c) * axis.x * axis.x;
			rotationMatrix.data[0][1] = (1 - c) * axis.x * axis.y - s * axis.z;
			rotationMatrix.data[0][2] = (1 - c) * axis.x * axis.z + s * axis.y;

			rotationMatrix.data[1][0] = (1 - c) * axis.x * axis.y + s * axis.z;
			rotationMatrix.data[1][1] = c + (1 - c) * axis.y * axis.y;
			rotationMatrix.data[1][2] = (1 - c) * axis.y * axis.z - s * axis.x;

			rotationMatrix.data[2][0] = (1 - c) * axis.x * axis.z - s * axis.y;
			rotationMatrix.data[2][1] = (1 - c) * axis.y * axis.z + s * axis.x;
			rotationMatrix.data[2][2] = c + (1 - c) * axis.z * axis.z;

			return rotationMatrix;
		}

		void Matrix::setRow(const Vector& v, uint32 row)
		{
			debug_assert(row >= 0 && row < 4, "incorrect row index");
			data[row][0] = v.x;
			data[row][1] = v.y;
			data[row][2] = v.z;
			data[row][3] = v.w;
		}

		void Matrix::setColumn(const Vector& v, uint32 column)
		{
			debug_assert(column >= 0 && column < 4, "incorrect column index");
			data[0][column] = v.x;
			data[1][column] = v.y;
			data[2][column] = v.z;
			data[3][column] = v.w;
		}
	}
}