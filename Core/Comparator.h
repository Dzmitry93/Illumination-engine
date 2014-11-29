#ifndef __COMPARATOR_H__
#define __COMPARATOR_H__

#include "Format.h"
#include "Plane.h"
#include "OBB.h"

using namespace OGE::math;
using namespace OGE::resource;
using std::binary_function;

namespace OGE
{
	namespace Collections
	{
		enum COMPARE_AGREEMENT
		{
			COMPARE_X,
			COMPARE_Y,
			COMPARE_Z
		};

		enum COMPARE_RESULT
		{
			LESSER,
			GREATER,
			EQUALS
		};

		template<class T, class S, class K>
		class IObjectObjectComparator
		{
		public:
			virtual K compare(T first, S second) const = 0;
		};

		class FLoatComparator : public IObjectObjectComparator<float, float, COMPARE_RESULT>
		{
		public:
			COMPARE_RESULT compare(float first, float second)  const
			{
				if(first == second) { return EQUALS; }

				if(first > second) { return GREATER; }
				else { return LESSER; }
			}
		};
		
		class OBBComparatorX	: public IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>
		{
		public:
			virtual COMPARE_RESULT compare(OBB obb, OBB obb2) const
			{
				float minX = FLT_MAX;
				float minX2 = FLT_MAX;

				for(uint32 i = 0; i < 6; i++)
				{
					float x = (obb.middlePoint + obb.planes[i].dir * obb.planes[i].halfDistance).x;
					float x2 = (obb2.middlePoint + obb2.planes[i].dir * obb2.planes[i].halfDistance).x;

					if(x < minX) { minX = x; }
					if(x2 < minX2) { minX2 = x2; }
				}

				if(minX < minX2) { return COMPARE_RESULT::LESSER; }
				if(minX > minX2) { return COMPARE_RESULT::GREATER; }
				else { return COMPARE_RESULT::EQUALS; }
			}
		};

		class OBBComparatorY	: public IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>
		{
		public:
			virtual COMPARE_RESULT compare(OBB obb, OBB obb2) const
			{
				float minY = FLT_MAX;
				float minY2 = FLT_MAX;

				for(uint32 i = 0; i < 6; i++)
				{
					float y = (obb.middlePoint + obb.planes[i].dir * obb.planes[i].halfDistance).y;
					float y2 = (obb2.middlePoint + obb2.planes[i].dir * obb2.planes[i].halfDistance).y;

					if(y < minY) { minY = y; }
					if(y2 < minY2) { minY2 = y2; }
				}

				if(minY < minY2) { return COMPARE_RESULT::LESSER; }
				if(minY > minY2) { return COMPARE_RESULT::GREATER; }
				else { return COMPARE_RESULT::EQUALS; }
			}
		};

		class OBBComparatorZ	: public IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>
		{
		public:
			virtual COMPARE_RESULT compare(OBB obb, OBB obb2) const
			{
				float minZ = FLT_MAX;
				float minZ2 = FLT_MAX;

				for(uint32 i = 0; i < 6; i++)
				{
					float z = (obb.middlePoint + obb.planes[i].dir * obb.planes[i].halfDistance).z;
					float z2 = (obb2.middlePoint + obb2.planes[i].dir * obb2.planes[i].halfDistance).z;

					if(z < minZ) { minZ = z; }
					if(z2 < minZ2) { minZ2 = z2; }
				}

				if(minZ < minZ2) { return COMPARE_RESULT::LESSER; }
				if(minZ > minZ2) { return COMPARE_RESULT::GREATER; }
				else { return COMPARE_RESULT::EQUALS; }
			}
		};

		class Comparators
		{
		public:
			static IObjectObjectComparator<float, float, COMPARE_RESULT>* pFloatFloatComparator;

			static IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* pOBBOBBComparatorX;
			static IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* pOBBOBBComparatorY;
			static IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* pOBBOBBComparatorZ;
		};
	}
}

#endif