#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Debug.h"
#include "TypeDefinitions.h"
#include "Vector.h"
#include "MathDefinitions.h"

namespace OGE
{
	namespace math
	{
		class Vector;
		class Matrix
		{
		public:
			Matrix();

			Matrix(
				float x1, float y1, float z1, float w1,
				float x2, float y2, float z2, float w2,
				float x3, float y3, float z3, float w3,
				float x4, float y4, float z4, float w4);

			Matrix(float* ppData);
			Matrix(const Vector& rRow, 
				const Vector& rRow2,
				const Vector& rRow3, 
				const Vector& rRow4);	 

			~Matrix();

			bool operator == (const Matrix& rhs) const;

			void operator += (const Matrix& rhs);
			void operator -= (const Matrix& rhs);
			void operator *= (const Matrix& rhs);
			Matrix operator * (const Matrix& rhs) const;
			Matrix operator + (const Matrix& rhs) const;
			Matrix operator - (const Matrix& rhs) const;

			Vector operator * (const Vector& rV) const;

			static void MultiplyMatrix(Matrix& rResult, const Matrix& rMatrix, const Matrix& rMatrix2);
			static void Sum(Matrix& rResult, const Matrix& m1, const Matrix& m2);
			static void Difference(Matrix& rResult, const Matrix& m1, const Matrix& m2);

			static Matrix asScale(float scaleX, float scaleY, float scaleZ);
			static Matrix asScale(const Vector& v);
			static Matrix asTranslation(float x, float y, float z);
			static Matrix asTranslation(const Vector& v);

			static Matrix asRotationX(float radians);
			static Matrix asRotationY(float radians);
			static Matrix asRotationZ(float radians);
			static Matrix asRotationArbitraryAxis(const Vector& axis, float radians);

			static Matrix asTranspose(const Matrix& rMatrix);

			static Matrix asRotationInverse(const Matrix& rRotation);
			static Matrix asTranslationInverse(const Matrix& rTranslation);
			static Matrix asScaleInverse(const Matrix& rScale);

			void setRows(const Vector& v, const Vector& v2, const Vector& v3, const Vector& v4);
			void setRow(const Vector& v, uint32 row);
			void setColumn(const Vector& v, uint32 column);
			void setColumns(const Vector& v, const Vector& v2, const Vector& v3, const Vector& v4);

			Vector getRow(uint32 index) const;
			Vector getColumn(uint32 index) const;

			union
			{
				struct {
					float x1, y1, z1, w1;
					float x2, y2, z2, w2;
					float x3, y3, z3, w3;
					float x4, y4, z4, w4;
				};

				float data[4][4];
			};  
		};
		 
		inline Matrix::Matrix()
		{
			memset((void*)&data, 0, 64);

			data[0][0] = 1;
			data[1][1] = 1;
			data[2][2] = 1;
			data[3][3] = 1;
		}

		inline Matrix::Matrix(float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3,
			float x4, float y4, float z4, float w4) :

			x1(x1), y1(y1), z1(z1), w1(w1),
			x2(x2), y2(y2), z2(z2), w2(w2),
			x3(x3), y3(y3), z3(z3), w3(w3),
			x4(x4), y4(y4), z4(z4), w4(w4)
		{
		}

		inline Matrix::Matrix(float* ppData)
		{
			memcpy((void*)&data, (void*)ppData, 64);
		}

		inline Matrix::~Matrix() {}

		inline void Matrix::operator += (const Matrix& rhs)
		{
			data[0][0] += rhs.data[0][0];
			data[0][1] += rhs.data[0][1];
			data[0][2] += rhs.data[0][2];
			data[0][3] += rhs.data[0][3];

			data[1][0] += rhs.data[1][0];
			data[1][1] += rhs.data[1][1];
			data[1][2] += rhs.data[1][2];
			data[1][3] += rhs.data[1][3];

			data[2][0] += rhs.data[2][0];
			data[2][1] += rhs.data[2][1];
			data[2][2] += rhs.data[2][2];
			data[2][3] += rhs.data[2][3];

			data[3][0] += rhs.data[3][0];
			data[3][1] += rhs.data[3][1];
			data[3][2] += rhs.data[3][2];
			data[3][3] += rhs.data[3][3];
		}

		inline void Matrix::operator -= (const Matrix& rhs)
		{
			data[0][0] -= rhs.data[0][0];
			data[0][1] -= rhs.data[0][1];
			data[0][2] -= rhs.data[0][2];
			data[0][3] -= rhs.data[0][3];

			data[1][0] -= rhs.data[1][0];
			data[1][1] -= rhs.data[1][1];
			data[1][2] -= rhs.data[1][2];
			data[1][3] -= rhs.data[1][3];

			data[2][0] -= rhs.data[2][0];
			data[2][1] -= rhs.data[2][1];
			data[2][2] -= rhs.data[2][2];
			data[2][3] -= rhs.data[2][3];

			data[3][0] -= rhs.data[3][0];
			data[3][1] -= rhs.data[3][1];
			data[3][2] -= rhs.data[3][2];
			data[3][3] -= rhs.data[3][3];
		}

		inline void Matrix::operator *= (const Matrix& m2)
		{
			// cache 
			float x = data[0][0];
			float y = data[0][1];
			float z = data[0][2];
			float w = data[0][3];

			data[0][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			data[0][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			data[0][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			data[0][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = data[1][0];
			y = data[1][1];
			z = data[1][2];
			w = data[1][3];

			data[1][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			data[1][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			data[1][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			data[1][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = data[2][0];
			y = data[2][1];
			z = data[2][2];
			w = data[2][3];

			data[2][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			data[2][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			data[2][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			data[2][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = data[3][0];
			y = data[3][1];
			z = data[3][2];
			w = data[3][3];

			data[3][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			data[3][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			data[3][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			data[3][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

		}

		inline Matrix Matrix::operator * (const Matrix& rhs) const
		{
			Matrix result;

			// cache 
			float x = data[0][0];
			float y = data[0][1];
			float z = data[0][2];
			float w = data[0][3];

			result.data[0][0] = x * rhs.data[0][0] + y * rhs.data[1][0] + z * rhs.data[2][0] + w * rhs.data[3][0];
			result.data[0][1] = x * rhs.data[0][1] + y * rhs.data[1][1] + z * rhs.data[2][1] + w * rhs.data[3][1];
			result.data[0][2] = x * rhs.data[0][2] + y * rhs.data[1][2] + z * rhs.data[2][2] + w * rhs.data[3][2];
			result.data[0][3] = x * rhs.data[0][3] + y * rhs.data[1][3] + z * rhs.data[2][3] + w * rhs.data[3][3];

			x = data[1][0];
			y = data[1][1];
			z = data[1][2];
			w = data[1][3];

			result.data[1][0] = x * rhs.data[0][0] + y * rhs.data[1][0] + z * rhs.data[2][0] + w * rhs.data[3][0];
			result.data[1][1] = x * rhs.data[0][1] + y * rhs.data[1][1] + z * rhs.data[2][1] + w * rhs.data[3][1];
			result.data[1][2] = x * rhs.data[0][2] + y * rhs.data[1][2] + z * rhs.data[2][2] + w * rhs.data[3][2];
			result.data[1][3] = x * rhs.data[0][3] + y * rhs.data[1][3] + z * rhs.data[2][3] + w * rhs.data[3][3];

			x = data[2][0];
			y = data[2][1];
			z = data[2][2];
			w = data[2][3];

			result.data[2][0] = x * rhs.data[0][0] + y * rhs.data[1][0] + z * rhs.data[2][0] + w * rhs.data[3][0];
			result.data[2][1] = x * rhs.data[0][1] + y * rhs.data[1][1] + z * rhs.data[2][1] + w * rhs.data[3][1];
			result.data[2][2] = x * rhs.data[0][2] + y * rhs.data[1][2] + z * rhs.data[2][2] + w * rhs.data[3][2];
			result.data[2][3] = x * rhs.data[0][3] + y * rhs.data[1][3] + z * rhs.data[2][3] + w * rhs.data[3][3];

			x = data[3][0];
			y = data[3][1];
			z = data[3][2];
			w = data[3][3];

			result.data[3][0] = x * rhs.data[0][0] + y * rhs.data[1][0] + z * rhs.data[2][0] + w * rhs.data[3][0];
			result.data[3][1] = x * rhs.data[0][1] + y * rhs.data[1][1] + z * rhs.data[2][1] + w * rhs.data[3][1];
			result.data[3][2] = x * rhs.data[0][2] + y * rhs.data[1][2] + z * rhs.data[2][2] + w * rhs.data[3][2];
			result.data[3][3] = x * rhs.data[0][3] + y * rhs.data[1][3] + z * rhs.data[2][3] + w * rhs.data[3][3];

			return result;
		}

		inline Matrix Matrix::operator + (const Matrix& rhs) const
		{
			Matrix temp;

			temp.data[0][0] = data[0][0] + rhs.data[0][0];
			temp.data[0][1] = data[0][1] + rhs.data[0][1];
			temp.data[0][2] = data[0][2] + rhs.data[0][2];
			temp.data[0][3] = data[0][3] + rhs.data[0][3];

			temp.data[1][0] = data[1][0] + rhs.data[1][0];
			temp.data[1][1] = data[1][1] + rhs.data[1][1];
			temp.data[1][2] = data[1][2] + rhs.data[1][2];
			temp.data[1][3] = data[1][3] + rhs.data[1][3];

			temp.data[2][0] = data[2][0] + rhs.data[2][0];
			temp.data[2][1] = data[2][1] + rhs.data[2][1];
			temp.data[2][2] = data[2][2] + rhs.data[2][2];
			temp.data[2][3] = data[2][3] + rhs.data[2][3];

			temp.data[3][0] = data[3][0] + rhs.data[3][0];
			temp.data[3][1] = data[3][1] + rhs.data[3][1];
			temp.data[3][2] = data[3][2] + rhs.data[3][2];
			temp.data[3][3] = data[3][3] + rhs.data[3][3];

			return temp;
		}

		inline Matrix Matrix::operator - (const Matrix& rhs) const
		{
			Matrix temp;

			temp.data[0][0] = data[0][0] - rhs.data[0][0];
			temp.data[0][1] = data[0][1] - rhs.data[0][1];
			temp.data[0][2] = data[0][2] - rhs.data[0][2];
			temp.data[0][3] = data[0][3] - rhs.data[0][3];

			temp.data[1][0] = data[1][0] - rhs.data[1][0];
			temp.data[1][1] = data[1][1] - rhs.data[1][1];
			temp.data[1][2] = data[1][2] - rhs.data[1][2];
			temp.data[1][3] = data[1][3] - rhs.data[1][3];

			temp.data[2][0] = data[2][0] - rhs.data[2][0];
			temp.data[2][1] = data[2][1] - rhs.data[2][1];
			temp.data[2][2] = data[2][2] - rhs.data[2][2];
			temp.data[2][3] = data[2][3] - rhs.data[2][3];

			temp.data[3][0] = data[3][0] - rhs.data[3][0];
			temp.data[3][1] = data[3][1] - rhs.data[3][1];
			temp.data[3][2] = data[3][2] - rhs.data[3][2];
			temp.data[3][3] = data[3][3] - rhs.data[3][3];

			return temp;
		}

		inline void Matrix::MultiplyMatrix(Matrix& rResult, const Matrix& m, const Matrix& m2)
		{
			// cache 
			float x = m.data[0][0];
			float y = m.data[0][1];
			float z = m.data[0][2];
			float w = m.data[0][3];

			rResult.data[0][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			rResult.data[0][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			rResult.data[0][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			rResult.data[0][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = m.data[1][0];
			y = m.data[1][1];
			z = m.data[1][2];
			w = m.data[1][3];

			rResult.data[1][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			rResult.data[1][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			rResult.data[1][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			rResult.data[1][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = m.data[2][0];
			y = m.data[2][1];
			z = m.data[2][2];
			w = m.data[2][3];

			rResult.data[2][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			rResult.data[2][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			rResult.data[2][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			rResult.data[2][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];

			x = m.data[3][0];
			y = m.data[3][1];
			z = m.data[3][2];
			w = m.data[3][3];

			rResult.data[3][0] = x * m2.data[0][0] + y * m2.data[1][0] + z * m2.data[2][0] + w * m2.data[3][0];
			rResult.data[3][1] = x * m2.data[0][1] + y * m2.data[1][1] + z * m2.data[2][1] + w * m2.data[3][1];
			rResult.data[3][2] = x * m2.data[0][2] + y * m2.data[1][2] + z * m2.data[2][2] + w * m2.data[3][2];
			rResult.data[3][3] = x * m2.data[0][3] + y * m2.data[1][3] + z * m2.data[2][3] + w * m2.data[3][3];
		}

		inline void Matrix::Sum(Matrix& rResult, const Matrix& m1, const Matrix& m2)
		{
			rResult.data[0][0] = m1.data[0][0] + m2.data[0][0];
			rResult.data[0][1] = m1.data[0][1] + m2.data[0][1];
			rResult.data[0][2] = m1.data[0][2] + m2.data[0][2];
			rResult.data[0][3] = m1.data[0][3] + m2.data[0][3];

			rResult.data[1][0] = m1.data[1][0] + m2.data[1][0];
			rResult.data[1][1] = m1.data[1][1] + m2.data[1][1];
			rResult.data[1][2] = m1.data[1][2] + m2.data[1][2];
			rResult.data[1][3] = m1.data[1][3] + m2.data[1][3];

			rResult.data[2][0] = m1.data[2][0] + m2.data[2][0];
			rResult.data[2][1] = m1.data[2][1] + m2.data[2][1];
			rResult.data[2][2] = m1.data[2][2] + m2.data[2][2];
			rResult.data[2][3] = m1.data[2][3] + m2.data[2][3];

			rResult.data[3][0] = m1.data[3][0] + m2.data[3][0];
			rResult.data[3][1] = m1.data[3][1] + m2.data[3][1];
			rResult.data[3][2] = m1.data[3][2] + m2.data[3][2];
			rResult.data[3][3] = m1.data[3][3] + m2.data[3][3];
		}

		inline void Matrix::Difference(Matrix& rResult, const Matrix& m1, const Matrix& m2)
		{
			rResult.data[0][0] = m1.data[0][0] - m2.data[0][0];
			rResult.data[0][1] = m1.data[0][1] - m2.data[0][1];
			rResult.data[0][2] = m1.data[0][2] - m2.data[0][2];
			rResult.data[0][3] = m1.data[0][3] - m2.data[0][3];

			rResult.data[1][0] = m1.data[1][0] - m2.data[1][0];
			rResult.data[1][1] = m1.data[1][1] - m2.data[1][1];
			rResult.data[1][2] = m1.data[1][2] - m2.data[1][2];
			rResult.data[1][3] = m1.data[1][3] - m2.data[1][3];

			rResult.data[2][0] = m1.data[2][0] - m2.data[2][0];
			rResult.data[2][1] = m1.data[2][1] - m2.data[2][1];
			rResult.data[2][2] = m1.data[2][2] - m2.data[2][2];
			rResult.data[2][3] = m1.data[2][3] - m2.data[2][3];

			rResult.data[3][0] = m1.data[3][0] - m2.data[3][0];
			rResult.data[3][1] = m1.data[3][1] - m2.data[3][1];
			rResult.data[3][2] = m1.data[3][2] - m2.data[3][2];
			rResult.data[3][3] = m1.data[3][3] - m2.data[3][3];
		}

		inline Matrix Matrix::asScale(float scaleX, float scaleY, float scaleZ)
		{
			Matrix temp;
			temp.data[0][0] = scaleX;
			temp.data[1][1] = scaleY;
			temp.data[2][2] = scaleZ;

			return temp;
		}

		inline Matrix Matrix::asTranslation(float x, float y, float z)
		{
			Matrix temp;
			temp.data[3][0] = x;
			temp.data[3][1] = y;
			temp.data[3][2] = z;

			return temp;
		}

		inline Matrix Matrix::asRotationX(float radians)
		{
			Matrix temp;
			
			temp.data[1][1] = cosf(radians);
			temp.data[1][2] = -sinf(radians);
			temp.data[2][1] = sinf(radians);
			temp.data[2][2] = cosf(radians);

			return temp;
		}

		inline Matrix Matrix::asRotationY(float radians)
		{
			Matrix temp;

			temp.data[0][0] = cosf(radians);
			temp.data[0][2] = sinf(radians);
			temp.data[2][0] = -sinf(radians);
			temp.data[2][2] = cosf(radians);

			return temp;
		}

		inline Matrix Matrix::asRotationZ(float radians)
		{
			Matrix temp;

			temp.data[0][0] = cosf(radians);
			temp.data[0][1] = -sinf(radians);
			temp.data[1][0] = sinf(radians);
			temp.data[1][1] = cosf(radians);

			return temp;
		}

		inline Matrix Matrix::asTranspose(const Matrix& rMatrix)
		{
			Matrix temp;

			temp.data[0][0] = rMatrix.data[0][0];
			temp.data[1][0] = rMatrix.data[0][1];
			temp.data[2][0] = rMatrix.data[0][2];
			temp.data[3][0] = rMatrix.data[0][3];

			temp.data[0][1] = rMatrix.data[1][0];
			temp.data[1][1] = rMatrix.data[1][1];
			temp.data[2][1] = rMatrix.data[1][2];
			temp.data[3][1] = rMatrix.data[1][3];

			temp.data[0][2] = rMatrix.data[2][0];
			temp.data[1][2] = rMatrix.data[2][1];
			temp.data[2][2] = rMatrix.data[2][2];
			temp.data[3][2] = rMatrix.data[2][3];

			temp.data[0][3] = rMatrix.data[3][0];
			temp.data[1][3] = rMatrix.data[3][1];
			temp.data[2][3] = rMatrix.data[3][2];
			temp.data[3][3] = rMatrix.data[3][3];

			return temp;
		}

		inline Matrix Matrix::asRotationInverse(const Matrix& rRotation)
		{
			return asTranspose(rRotation);
		}

		inline Matrix Matrix::asTranslationInverse(const Matrix& rTranslation)
		{
			Matrix translation;
			translation.data[0][0] = -rTranslation.data[0][0];
			translation.data[1][1] = -rTranslation.data[1][1];
			translation.data[2][2] = -rTranslation.data[2][2];
			translation.data[3][3] = -rTranslation.data[3][3];

			return translation;
		}

		inline Matrix Matrix::asScaleInverse(const Matrix& rScale)
		{
			Matrix scale;
			scale.data[0][0] = 1.0f / rScale.data[0][0];
			scale.data[1][1] = 1.0f / rScale.data[1][1];
			scale.data[2][2] = 1.0f / rScale.data[2][2];
			scale.data[3][3] = 1.0f / rScale.data[3][3];
		}

		inline bool Matrix::operator == (const Matrix& rOther) const
		{
			return this->data == rOther.data;
		}
	}
}

#endif