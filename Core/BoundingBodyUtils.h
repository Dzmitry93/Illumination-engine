#ifndef __BOUNDING_BODY_UTILS_H__
#define __BOUNDING_BODY_UTILS_H__

#include "Vector.h"
#include "OBB.h"
#include "List.h"
#include "AABB.h"
#include "OBB.h"
#include "Matrix.h"
#include "TransformUtils.h"

using OGE::Collections::List;
using OGE::math::OBB;
using OGE::math::AABB;
using OGE::math::Matrix;
using OGE::math::Vector;

namespace OGE
{
	namespace util
	{
		class BoundingBodyUtils
		{
		public:		
			static List<Vector> getBoundingBodyPoints(const OBB& rOBB);
		};
	}
}

#endif