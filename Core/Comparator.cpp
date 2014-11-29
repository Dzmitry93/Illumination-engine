#include "Comparator.h"

namespace OGE
{
	namespace Collections
	{
		IObjectObjectComparator<float, float, COMPARE_RESULT>* Comparators::pFloatFloatComparator = new FLoatComparator();

		IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* Comparators::pOBBOBBComparatorX = new OBBComparatorX();
		IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* Comparators::pOBBOBBComparatorY = new OBBComparatorY();
		IObjectObjectComparator<OBB, OBB, COMPARE_RESULT>* Comparators::pOBBOBBComparatorZ = new OBBComparatorZ();
	}
}