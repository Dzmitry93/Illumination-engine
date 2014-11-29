#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include "Matrix.h"
#include "Vector.h"
#include "Plane.h"
#include "OBB.h"
#include "AABB.h"
#include "ViewFrustum.h"
#include <float.h>

namespace OGE
{
	namespace math
	{
		void CalculateTangentSpace(const Vector& rV1, const Vector& rV2, const Vector& rV3, 
			float u1, float v1, float u2, float v2,
			Vector& rTangent, Vector& rBinormal);

		Vector Line(const Plane& rPlane, const Plane& rPlane2);

		namespace MatrixUtils
		{
																										  
			Matrix CreateViewMatrixLH(const Vector& eye,
									const Vector& lookAt,
									const Vector& up);

			Matrix CreateViewMatrixRH(const Vector& eye,
									const Vector& lookAt,
									const Vector& up);

			Matrix CreateProjectionMatrixLH(float fov, 
										  float aspectRatio,	
										  float zNear, 
										  float zFar);

			Matrix CreateProjectionMatrixRH(float fov, 
										  float aspectRatio,	
										  float zNear, 
										  float zFar);
		}

		namespace IntersectionUtils
		{

			enum INTERSECTION_RESULT
			{
				INSIDE,
				OUTSIDE,
				INTERSECT
			};

			inline bool PlaneRayIntersection(const Plane& plane, const Vector& origin, const Vector& rayDir, float& d)
			{
				float temp = Vector::DotProduct(plane.normal, rayDir);
				d = 0.0f;

				if((temp - 0.0f) > 0.00001f)
				{
					d = -(plane.d + Vector::DotProduct(plane.normal, origin) ) / ( temp );
					return true;
				}

				return false;
			}

			bool PlanesIntersection(const Plane& rPlane, const Plane& rPlane2, const Plane& rPlane3, Vector& rPoint);

			/* p1, p2, p3 - vertices of triangle; 
			   out param d - is a distance between ray origin and triangle */
			bool BaricentricTest(const Vector& p1, const Vector& p2, const Vector& p3,
				const Vector& origin, const Vector& rayDir, 
				float& d, float& w, float& v, float& u);

		/*	bool BaricentricTest(const V3N3T2Triangle& rTriangle,
				const Vector& origin, const Vector& rayDir, float& d);*/

			/*
				Slabs method. See Haines, Eric, "Essential Ray Tracing Algorithms," Chapter 2 in Andrew 
					Glassner, ed., An Introduction to Ray Tracing, Academic Press Inc.,  
					London, 1989. Cited on p. 570, 572, 574, 584 
			*/
			bool RayOBBIntersection(const OBB& obb, const Vector& origin, const Vector& rayDir, float& d);
			
			bool RayAABBIntersection(const AABB& aabb, const Vector& origin, const Vector& rayDir, float& d);
			bool RayAABBIntersectionSlab(const AABB& aabb, const Vector& origin, const Vector& rayDir, float& d);

			// from http://ray-tracing.ru/articles244.html
			bool RayAABBIntersectionSlab(const AABB& aabb, const Vector& origin, 
				const Vector& rayDir, float& tnear, float& tfar);

			bool OverlapOBBOBB(const OBB& rOBB, const OBB& rOBB2);
			bool OverlapAABBAABB(const AABB& rAABB, const AABB& rAABB2); 
			
			INTERSECTION_RESULT FrustumAABBIntersection(const ViewFrustum& rFrustum, const AABB& rAABB);
			INTERSECTION_RESULT FrustumOBBIntersection(const ViewFrustum& rFrustum, const OBB& rOBB);

			INTERSECTION_RESULT SphereAABBIntersection(const Vector& rCenter, float radius, const AABB& rAABB);
			INTERSECTION_RESULT SphereOBBIntersection(const Vector& rCenter, float radius, const OBB& rOBB);
			INTERSECTION_RESULT SphereSphereIntersection(const Vector& rCenter, float radius, const Vector& rCenter2, float radius2);
		}
	}
}

#endif