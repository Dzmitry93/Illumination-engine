#ifndef __MATH_DEFINITIONS_H__
#define __MATH_DEFINITIONS_H__

#include "TypeDefinitions.h"

#define PI 3.14159f
#define GRADUS_IN_RADIAN 57.2958f
#define RADIAN_IN_GRADUS 0.0174539f
#define ToRadian(angle) OGE::math::ToRadins(angle)
#define ToDegree(radians) OGE::math::ToDegrees(radians)

#define FLOAT_EQUALS(a, b) OGE::math:::FloatEquals(a, b);

namespace OGE
{
	namespace math
	{
		float ToRadians(float angle);
		float ToDegrees(float radians);
		bool FloatEquals(float a, float b, float eps = 0.0e-6f);
		bool FloatGreaterOrEquals(float a, float b, float eps  = 0.0e-6f);

		inline float ToRadins(float angle)
		{
			return angle * RADIAN_IN_GRADUS;
		}

		inline float ToDegrees(float radians)
		{
			return radians * GRADUS_IN_RADIAN;
		}

		inline bool FloatEquals(float a, float b, float eps)
		{
			float c = a - b;
			if(abs( c ) <= eps )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool FloatGreaterOrEquals(float a, float b, float eps)
		{
			float c = a - b;

			if(c > 0 || abs(c) <= eps)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

#endif