#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "TypeDefinitions.h"
#include "Debug.h"
#include "Matrix.h"

#define Position(x, y, z) Vector(x, y, z, 1.0f)

namespace OGE
{
	namespace math
	{
		class Matrix;

		class Vector
		{
		public:
			Vector();
			Vector(float v, float v2);
			Vector(float v, float v2, float v3);
			Vector(float v, float v2, float v3, float v4);
			~Vector();

			bool operator == (const Vector& rOther) const;

			Vector operator - (const Vector& rhs) const;
			Vector operator + (const Vector& rhs) const;
			Vector operator * (const float value) const;
			Vector operator * (const Matrix& rMatrix) const;
			Vector operator - () const;

			const float& operator [] (uint32 index) const;
			float& operator [] (uint32 index);

			void operator -= (const Vector& rhs);
			void operator += (const Vector& rhs);
			void operator *= (float value);
			void operator *= (const Matrix& rMatrix);

			static Vector Normalize(const Vector& vector);
			static float Length(const Vector& v);
			static float DotProduct(const Vector& vector, const Vector& vector2);
			static Vector CrossProduct(const Vector& vector, const Vector& vector2);
			static Vector ReflectedVector(const Vector& vector, const Vector& normal);
			
			float dotProduct(float x, float y, float z);

			float length() const;
			void normalize();
			
			union 
			{
				struct
				{
					float x, y, z, w;
				};

				float data[4];
			};
			
		};

		inline Vector::Vector(float v, float v2, float v3, float v4) : x(v), y(v2), z(v3), w(v4) {}
		inline Vector::Vector(float v, float v2, float v3) : x(v), y(v2), z(v3), w(0.0f) {}
		inline Vector::Vector(float v, float v2) : x(v), y(v2), z(0.0f), w(0.0f) {}
		inline Vector::Vector() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		inline Vector::~Vector() {}

		inline Vector Vector::operator - (const Vector& rhs) const
		{
			return Vector(this->x - rhs.x, this->y - rhs.y, 
				this->z - rhs.z, this->w - rhs.w);
		}

		inline Vector Vector::operator + (const Vector& rhs) const
		{
			return Vector(this->x  + rhs.x, this->y + rhs.y, 
				this->z + rhs.z, this->w + rhs.w);
		}

		inline Vector Vector::operator * (const float value) const
		{
			return Vector(x * value, y * value, z * value);
		}

		inline void Vector::operator -= (const Vector& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;
		}

		inline void Vector::operator += (const Vector& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;
		}

		inline void Vector::operator *=(float value)
		{
			this->x *= value;
			this->y *= value;
			this->z *= value;
			this->w *= value;
		}

		inline Vector Vector::operator - () const
		{
			return Vector(-x, -y, -z, -w);
		}
		
		inline float Vector::Length(const Vector& v)
		{
			return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
		}

		inline Vector Vector::CrossProduct(const Vector& v, const Vector& v2)
		{
			float i = v.y * v2.z - v.z * v2.y;
			float j = v.z * v2.x - v.x * v2.z;
			float k = v.x * v2.y - v.y * v2.x;

			return Vector(i, j, k);
		}

		inline float Vector::DotProduct(const Vector& v, const Vector& v2)
		{
			return v.x*v2.x + v.y*v2.y + v.z*v2.z;
		}

		inline float Vector::dotProduct(float x, float y, float z)
		{
			return this->x * x + this->y * y + this->z * z;
		}

		inline Vector Vector::Normalize(const Vector& v)
		{
			float len = Length(v);
			return Vector(v.x / len, v.y / len, v.z / len);
		}

		inline Vector Vector::ReflectedVector(const Vector& v, const Vector& v2)
		{
			float temp = Vector::DotProduct(v, v2) / Vector::DotProduct(v2, v2);

			return Vector(2 * v2.x * temp - v.x, 
				2 * v2.y * temp - v.y, 
				2 * v2.z * temp - v.z);
		}

		inline float Vector::length() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		inline void Vector::normalize()
		{
			float l = length();

			if(l > 0)
			{
				this->x = x / l;
				this->y = y / l;
				this->z = z / l;
			}
		}

		inline const float& Vector::operator[] (uint32 index) const
		{
			return data[index];
		}

		inline float& Vector::operator[] (uint32 index)
		{
			return data[index];
		}

		inline bool Vector::operator == (const Vector& rOther) const
		{
			return data == rOther.data;
		}
	}
}

#endif

