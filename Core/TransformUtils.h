#ifndef __PRIMITIVE_UTILS_H__
#define __PRIMITIVE_UTILS_H__

#include "Format.h"
#include "Matrix.h"
#include "AABB.h"
#include "OBB.h"
#include "ViewFrustum.h"

using namespace OGE::math;
using OGE::resource::V3TNT2Triangle;


namespace OGE
{
	namespace util
	{
		class TransformUtils
		{
		public:
			static V3TNT2Triangle apllyTransform(const V3TNT2Triangle& rTriangle,
				const Matrix& rWorldMatrix, const Matrix& rViewMatrix);

			static OBB transform(const OBB& rOBB, const Matrix& rMatrix);
			static AABB transform(const AABB& rAABB, const Matrix& rMatrix);
		};

		inline AABB TransformUtils::transform(const AABB& rAABB, const Matrix& rMatrix)
		{
			AABB aabb = rAABB;
			aabb.centerPoint *= rMatrix;

			return aabb;
		}
	}
}

#endif