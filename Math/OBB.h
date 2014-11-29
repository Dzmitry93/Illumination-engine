#ifndef __OBB_H__
#define __OBB_H__

#include "Plane.h"
#include "Vector.h"
#include "AABB.h"

namespace OGE
{
	namespace math
	{
		class OBB
		{
		public:
			OBB();
			OBB(const Vector&  rMiddlePoint, 
				const Plane& pl1, const Plane& pl2, const Plane& pl3,
				const Plane& pl4, const Plane& pl5, const Plane& pl6);
			OBB(const AABB& rAABB);
			OBB(const Vector&  rMiddlePoint, 
				const Vector& rDirU, const Vector& rDirV, const Vector& rDirw,
				float halfDistanceU, float halfDistanceV, float halfDistanceW);

			bool contains(const Vector& rPoint) const;
			bool operator == (const OBB& rOther) const;

			Vector middlePoint;
			struct
			{
				Vector dir;
				float halfDistance;
			} planes[6];
		};

		inline OBB::OBB(const AABB& rAABB)
		{
			planes[0].dir.x = 1.0f;
			planes[0].dir.y = 0.0f;
			planes[0].dir.z = 0.0f;
			planes[0].halfDistance = rAABB.halfDistance.x;

			planes[1].dir.x = 0.0f;
			planes[1].dir.y = 1.0f;
			planes[1].dir.z = 0.0f;
			planes[1].halfDistance = rAABB.halfDistance.y;

			planes[2].dir.x = 0.0f;
			planes[2].dir.y = 0.0f;
			planes[2].dir.z = 1.0f;
			planes[2].halfDistance = rAABB.halfDistance.z;

			planes[3].dir.x = -1.0f;
			planes[3].dir.y = 0.0f;
			planes[3].dir.z = 0.0f;
			planes[3].halfDistance = rAABB.halfDistance.x;

			planes[4].dir.x = 0.0f;
			planes[4].dir.y = -1.0f;
			planes[4].dir.z = 0.0f;
			planes[4].halfDistance = rAABB.halfDistance.y;

			planes[5].dir.x = 0.0f;
			planes[5].dir.y = 0.0f;
			planes[5].dir.z = -1.0f;
			planes[5].halfDistance = rAABB.halfDistance.z;
			middlePoint = rAABB.centerPoint;
		}

		inline OBB::OBB()
		{
			this->middlePoint = Vector(0.0f, 0.0f, 0.0f, 1.0f);

			for(uint32 i = 0; i < 6; i++)
			{
				this->planes[i].halfDistance = 0.0f;
			}
		}

		inline OBB::OBB(const Vector& middlePoint, 
				const Plane& pl1, const Plane& pl2, const Plane& pl3,
				const Plane& pl4, const Plane& pl5, const Plane& pl6) : middlePoint(middlePoint)
		{
			this->planes[0].halfDistance = abs(Vector::DotProduct(pl1.normal, middlePoint) + pl1.d)/ 
							sqrt( Vector::DotProduct(pl1.normal, pl1.normal) );

			this->planes[1].halfDistance = abs(Vector::DotProduct(pl2.normal, middlePoint) + pl2.d)/ 
							sqrt( Vector::DotProduct(pl2.normal, pl2.normal) );

			this->planes[2].halfDistance = abs(Vector::DotProduct(pl3.normal, middlePoint) + pl3.d)/ 
							sqrt( Vector::DotProduct(pl3.normal, pl3.normal) );

			this->planes[3].halfDistance = this->planes[0].halfDistance;

			this->planes[4].halfDistance = this->planes[1].halfDistance;

			this->planes[5].halfDistance = this->planes[2].halfDistance;

			this->planes[0].dir = Vector::Normalize(pl1.normal);
			this->planes[1].dir = Vector::Normalize(pl2.normal);
			this->planes[2].dir = Vector::Normalize(pl3.normal);
			this->planes[3].dir = Vector::Normalize(pl4.normal);
			this->planes[4].dir = Vector::Normalize(pl5.normal);
			this->planes[5].dir = Vector::Normalize(pl6.normal);
		}

		inline OBB::OBB(const Vector&  rMiddlePoint, 
				const Vector& rDirU, const Vector& rDirV, const Vector& rDirW,
				float halfDistanceU, float halfDistanceV, float halfDistanceW)
		{
			this->middlePoint = rMiddlePoint;

			this->planes[0].halfDistance = halfDistanceU;
			this->planes[3].halfDistance = halfDistanceU;
			this->planes[1].halfDistance = halfDistanceV;
			this->planes[4].halfDistance = halfDistanceV;
			this->planes[2].halfDistance = halfDistanceW;
			this->planes[5].halfDistance = halfDistanceW;

			this->planes[0].dir = Vector::Normalize(rDirU);
			this->planes[3].dir = Vector::Normalize(-rDirU);
			this->planes[1].dir = Vector::Normalize(rDirV);
			this->planes[4].dir = Vector::Normalize(-rDirV);
			this->planes[2].dir = Vector::Normalize(rDirW);
			this->planes[5].dir = Vector::Normalize(-rDirW);
		}

		inline bool OBB::contains(const Vector& rPoint) const
		{
			float u = abs(Vector::DotProduct(rPoint - middlePoint, planes[0].dir));
			float v = abs(Vector::DotProduct(rPoint - middlePoint, planes[1].dir));
			float w = abs(Vector::DotProduct(rPoint - middlePoint, planes[2].dir));

			return (u <= planes[0].halfDistance 
				&& v < planes[1].halfDistance 
				&& w < planes[2].halfDistance);
		}

		inline bool OBB::operator == (const OBB& rOther) const
		{
			if(middlePoint == rOther.middlePoint)
			{
				bool equals = false;
				for(uint32 i = 0; i < 6; i++)
				{
					if(!(planes[i].dir == rOther.planes[i].dir &&
						planes[i].halfDistance == planes[i].halfDistance))
					{
						return false;
					}
				}

				return true;
			}

			return false;
		}
	}
}

#endif