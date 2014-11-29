#pragma once
#ifndef __NUMERICAL_H__
#define __NUMERICAL_H__

#include "Matrix.h"
#include "Vector.h"

namespace OGE
{
	namespace math
	{
		class NumericalMethods
		{
		public:

			/* Jacobi rotation method
			 Matrix must be symmetric
			 See Mathematics for 3d game programmer and Computer Graphics - Eric Lengyel, Second Edition
			*/
			static void CalculateEigensystem(const Matrix& rMatrix, Vector& rLambda, Matrix& rEigenVectors);
		};
	}
}

#endif