#ifndef __PLANE_H__
#define __PLANE_H__

#include "Vector.h"
#include "MathDefinitions.h"

namespace OGE
{
	namespace math
	{
		class Plane
		{
		public:
			Plane();
			Plane(float a, float b, float c, float d);
			Plane(const Vector& point, const Vector& normal);
			Plane(const Vector& normal, float d);
			Plane(const Vector& point, const Vector& point2, const Vector& point3);
			~Plane();

			bool isParallel(const Plane& other) const;
			bool isOrtogonal(const Plane& other) const;

			float angle(const Plane& plane) const;
			float distanceToPoint(const Vector& point) const;

			void normalize();

			Vector normal;
			float d;
		};

		inline Plane::Plane() : normal(), d(0.0f) {}
		inline Plane::Plane(float a, float b, float c, float d) : normal(a, b, c), d(d) {}
		inline Plane::Plane(const Vector& normal, float d) : normal(normal), d(d) {}
		inline Plane::Plane(const Vector& point, const Vector& normal)
		{
			this->normal = normal;
			this->d = -Vector::DotProduct(normal, point);
		}
		inline Plane::Plane(const Vector& point, const Vector& point2, const Vector& point3)
		{
			this->normal = Vector::CrossProduct(point2 - point, point3 - point);
			this->d = -Vector::DotProduct(point, normal);
		}
		inline Plane::~Plane() {}

		inline bool Plane::isOrtogonal(const Plane& other) const
		{
			float dotResult = Vector::DotProduct(normal, other.normal);
			return FloatEquals( dotResult, 0.0f );
		}

		inline bool Plane::isParallel(const Plane& other) const
		{
			float a = normal.x / other.normal.x;
			float b = normal.y / other.normal.y;
			float c = normal.z / other.normal.z;

			return FloatEquals(a, b) && FloatEquals(b, c);
		}

		inline float Plane::angle(const Plane& other) const
		{
			float dot = Vector::DotProduct(normal, other.normal);
			float value = dot / (normal.length() * other.normal.length());

			return acosf(value);
		}

		inline float Plane::distanceToPoint(const Vector& point) const
		{
			return fabsf( Vector::DotProduct(normal, point) + d ) / normal.length();
		}

		inline void Plane::normalize()
		{
			float l = normal.length();

			if(l - 0.0f > 0.0001f)
			{
				normal.x /= l;
				normal.y /= l;
				normal.z /= l;
				d /= l;
			}
			else
			{
				normal.x = 0.0f;
				normal.y = 0.0f;
				normal.z = 0.0f;
				d = 0;
			}
		}
	}
}

#endif

