#include "TransformUtils.h"

namespace OGE
{
	namespace util
	{
		V3TNT2Triangle TransformUtils::apllyTransform(const V3TNT2Triangle& rTriangle, 
			const Matrix& rMatrix, const Matrix& rViewMatrix)
		{
			Vector v1(rTriangle.v1.x, rTriangle.v1.y, rTriangle.v1.z, 1.0f);
			Vector v2(rTriangle.v2.x, rTriangle.v2.y, rTriangle.v2.z, 1.0f);
			Vector v3(rTriangle.v3.x, rTriangle.v3.y, rTriangle.v3.z, 1.0f);

			Vector n1(rTriangle.v1.nx, rTriangle.v1.ny, rTriangle.v1.nz, 0.0f);
			Vector n2(rTriangle.v2.nx, rTriangle.v2.ny, rTriangle.v2.nz, 0.0f);
			Vector n3(rTriangle.v3.nx, rTriangle.v3.ny, rTriangle.v3.nz, 0.0f);

			v1 = v1 * rMatrix * rViewMatrix;
			v2 = v2 * rMatrix * rViewMatrix;
			v3 = v3 * rMatrix * rViewMatrix;
				
			n1 = n1 * rMatrix;
			n2 = n2 * rMatrix;
			n3 = n3 * rMatrix;

			n1.normalize();
			n2.normalize();
			n3.normalize();

			V3TNT2Triangle triangle;

			triangle.v1.x = v1.x;
			triangle.v1.y = v1.y;
			triangle.v1.z = v1.z;

			triangle.v2.x = v2.x;
			triangle.v2.y = v2.y;
			triangle.v2.z = v2.z;

			triangle.v3.x = v3.x;
			triangle.v3.y = v3.y;
			triangle.v3.z = v3.z;

			triangle.v1.nx = n1.x;
			triangle.v1.ny = n1.y;
			triangle.v1.nz = n1.z;

			triangle.v2.nx = n2.x;
			triangle.v2.ny = n2.y;
			triangle.v2.nz = n2.z;

			triangle.v3.nx = n3.x;
			triangle.v3.ny = n3.y;
			triangle.v3.nz = n3.z;

			return triangle;
		}

		OBB TransformUtils::transform(const OBB& rOBB, const Matrix& rMatrix) 
		{
			OBB obb = rOBB;
			obb.middlePoint *= rMatrix;

			obb.planes[0].dir *= rMatrix;
			obb.planes[0].dir.normalize();

			obb.planes[1].dir *= rMatrix;
			obb.planes[1].dir.normalize();

			obb.planes[2].dir *= rMatrix;
			obb.planes[2].dir.normalize();

			obb.planes[3].dir *= rMatrix;
			obb.planes[3].dir.normalize();

			obb.planes[4].dir *= rMatrix;
			obb.planes[4].dir.normalize();

			obb.planes[5].dir *= rMatrix;
			obb.planes[5].dir.normalize();

			return obb;
		}
	}
}