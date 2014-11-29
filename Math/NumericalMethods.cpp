#include "NumericalMethods.h"

namespace OGE
{
	namespace math
	{
		void NumericalMethods::CalculateEigensystem(const Matrix& rMatrix, Vector& rLambda, Matrix& rEigenVectors)
		{
			const uint32 maxSweeps = 32;
			const float eps = 1.0e-10f;

			float m11 = rMatrix.data[0][0];
			float m12 = rMatrix.data[0][1];
			float m13 = rMatrix.data[0][2];
			float m22 = rMatrix.data[1][1];
			float m23 = rMatrix.data[1][2];
			float m33 = rMatrix.data[2][2];

			for(uint32 i = 0; i < maxSweeps; i++)
			{
				if((fabsf(m12) < eps ) && (fabsf(m13) < eps) && (fabsf(m23) < eps))
				{
					break;
				}

				// Annihilate (1, 2) entry
				if(m12 != 0.0f)
				{
					float u = (m22 - m11) * 0.5f / m12;
					float u2 = u*u;
					float u2pl = u2 + 1.0f;
					float t = (u2pl != u2) ? 
						(( u < 0.0f ) ? -1.0f : 1.0f ) * (sqrtf(u2pl) - fabsf(u)) 
						: 0.5f / u;

					float c = 1.0f / sqrtf(t * t + 1.0f);
					float s = c * t;

					m11 -= t * m12;
					m22 += t * m12;
					m12 = 0.0f;

					float temp = c * m13 - s * m23;
					m23 = s * m13 + c * m23;
					m13 = temp;

					for(uint32 i = 0; i < 3; i++)
					{
						float temp = c * rEigenVectors.data[i][0] - s * rEigenVectors.data[i][1];
						rEigenVectors.data[i][1] = s * rEigenVectors.data[i][0] + c * rEigenVectors.data[i][1];
						rEigenVectors.data[i][0] = temp;
					}
				}

				// Annihilate (1, 3) entry
				if(m13 != 0.0f)
				{
					float u = (m33 - m11) * 0.5f / m13;
					float u2 = u*u;
					float u2pl = u2 + 1.0f;
					float t = (u2pl != u2) ? 
						(( u < 0.0f ) ? -1.0f : 1.0f ) * (sqrtf(u2pl) - fabsf(u)) 
						: 0.5f / u;

					float c = 1.0f / sqrtf(t * t + 1.0f);
					float s = c * t;

					m11 -= t * m13;
					m33 += t * m13;
					m13 = 0.0f;

					float temp = c * m12 - s * m23;
					m23 = s * m12 + c * m23;
					m12 = temp;

					for(uint32 i = 0; i < 3; i++)
					{
						float temp = c * rEigenVectors.data[i][0] - s * rEigenVectors.data[i][2];
						rEigenVectors.data[i][2] = s * rEigenVectors.data[i][0] + c * rEigenVectors.data[i][2];
						rEigenVectors.data[i][0] = temp;
					}
				}

				// Annihilate (2, 3) entry
				if(m23 != 0.0f)
				{
					float u = (m33 - m22) * 0.5f / m23;
					float u2 = u*u;
					float u2pl = u2 + 1.0f;
					float t = (u2pl != u2) ? 
						(( u < 0.0f ) ? -1.0f : 1.0f ) * (sqrtf(u2pl) - fabsf(u)) 
						: 0.5f / u;

					float c = 1.0f / sqrtf(t * t + 1.0f);
					float s = c * t;

					m22 -= t * m23;
					m33 += t * m23;
					m23 = 0.0f;

					float temp = c * m12 - s * m13;
					m13 = s * m12 + c * m13;
					m12 = temp;

					for(uint32 i = 0; i < 3; i++)
					{
						float temp = c * rEigenVectors.data[i][1] - s * rEigenVectors.data[i][2];
						rEigenVectors.data[i][2] = s * rEigenVectors.data[i][1] + c * rEigenVectors.data[i][2];
						rEigenVectors.data[i][1] = temp;
					}
				}
			}

			rLambda[0] = m11;
			rLambda[1] = m22;
			rLambda[2] = m33;
		}
	}
}