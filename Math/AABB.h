#ifndef __AABB_H__
#define __AABB_H__

#include "Vector.h"

namespace OGE
{
	namespace math
	{
		class AABB
		{
		public:
			AABB();
			AABB(const Vector& centerPoint, const Vector& halfDistance);
			AABB(float cx, float cy, float cz, const Vector& halfDistance);

			Vector centerPoint;
			Vector halfDistance;

			bool contains(const Vector& rPoint) const;
			bool isBorder(const Vector& rPoint, float eps = 0.01f) const;

			float getVolume() const;
		};

		inline AABB::AABB() {}
		inline AABB::AABB(const Vector& centerPoint, const Vector& halfDistance)  
			: centerPoint(centerPoint), halfDistance(halfDistance) {}

		inline AABB::AABB(float cx, float cy, float cz, const Vector& halfDistance)  
			: centerPoint(cx, cy, cz), halfDistance(halfDistance) {}

		inline bool AABB::contains(const Vector& rPoint) const
		{
			float minX = centerPoint.x - halfDistance.x;
			float maxX = centerPoint.x + halfDistance.x;
			float minY = centerPoint.y - halfDistance.y;
			float maxY = centerPoint.y + halfDistance.y;
			float minZ = centerPoint.z - halfDistance.z;
			float maxZ = centerPoint.z + halfDistance.z;

			if(rPoint.x >= minX && rPoint.x <= maxX &&
				rPoint.y >= minY && rPoint.y <= maxY &&
					rPoint.z >= minZ && rPoint.z <= maxZ)
			{
				return true;
			}
			return false;
		}

		inline bool AABB::isBorder(const Vector& rPoint, float eps) const
		{
			float minX = centerPoint.x - halfDistance.x;
			float maxX = centerPoint.x + halfDistance.x;
			float minY = centerPoint.y - halfDistance.y;
			float maxY = centerPoint.y + halfDistance.y;
			float minZ = centerPoint.z - halfDistance.z;
			float maxZ = centerPoint.z + halfDistance.z;

			if((abs(rPoint.x - minX) < eps) || (abs(rPoint.x - maxX) < eps))
			{
				return true;
			}
			if((abs(rPoint.y - minY) < eps) || (abs(rPoint.y - maxY) < eps))
			{
				return true;
			}
			if((abs(rPoint.z - minZ) < eps) || (abs(rPoint.z - maxZ) < eps))
			{
				return true;
			}

			return false;
		}

		inline float AABB::getVolume() const
		{
			return halfDistance.x * halfDistance.y * halfDistance.z;
		}
	}
}

#endif