#include "MathUtils.h"

namespace OGE
{
	namespace math
	{
		void CalculateTangentSpace(const Vector& rV1, const Vector& rV2, const Vector& rV3, 
			float u1, float v1, float u2, float v2,
			Vector& rTangent, Vector& rBinormal)
		{
			float l = u1 * v2 - u2 * v1;
			float k = 0.0f;

			if(fabs(l - 0.0f) >= 0.001f)
			{
				k = 1.0f / l;
			}
			
			Vector q1 = rV2 - rV1;
			Vector q2 = rV3 - rV1;

			rTangent.x = k * (v2 * q1.x - v1 * q2.x);
			rTangent.y = k * (v2 * q1.y - v1 * q2.y);
			rTangent.z = k * (v2 * q1.z - v1 * q2.z);

			rBinormal.x = k * (-u2 * q1.x + u1 * q2.x);
			rBinormal.y = k * (-u2 * q1.y + u1 * q2.y);
			rBinormal.z = k * (-u2 * q1.z + u1 * q2.z);
		}

		Vector line(const Plane& rPlane, const Plane& rPlane2)
		{
			Vector dir = Vector::CrossProduct(rPlane.normal, rPlane2.normal);
			
			float y;
			float z;

			float y1 = rPlane.normal.y;
			float z1 = rPlane.normal.z;
			float c1 = rPlane.d;

			float y2 = rPlane2.normal.y;
			float z2 = rPlane2.normal.z;
			float c2 = rPlane2.d;

			z1 /= y1;
			c1 /= y1;

			z2 -= z1 * y2;
			c2 -= c1 * y2;

			z = -c2 / z2;
			y = - (z1 * z + c1);

			float a = dir.y * dir.z;
			float b = - dir.x * dir.z;
			float c = - dir.x * dir.y;
			float d = dir.x * ( dir.z * y + dir.y * z);

			return Vector(a, b, c, d);
		}

		namespace MatrixUtils
		{
			Matrix CreateViewMatrixLH(const Vector& eye,
									const Vector& lookAt,
									const Vector& up)
			{
				Vector viewDir = lookAt - eye;
				viewDir.normalize();

				Vector upDir = up - viewDir * Vector::DotProduct(up, viewDir);
				upDir.normalize();

				Vector sideDir = Vector::CrossProduct(upDir, viewDir);
				sideDir.normalize();

				Matrix viewMatrix;
				viewMatrix.setColumns(sideDir, upDir, viewDir, Vector(0.0f, 0.0f, 0.0f, 1.0f));
				viewMatrix.setRow(Vector(
					-Vector::DotProduct(sideDir, eye),
					-Vector::DotProduct(upDir, eye),
					-Vector::DotProduct(viewDir, eye),
					1.0f), 3
					);

				return viewMatrix;
			}

			Matrix CreateViewMatrixRH(const Vector& eye,
									const Vector& lookAt,
									const Vector& up)
			{
				Vector viewDir = lookAt - eye;
				viewDir.normalize();

				Vector upDir = up - viewDir * Vector::DotProduct(up, viewDir);
				upDir.normalize();

				Vector sideDir = Vector::CrossProduct(viewDir, upDir);
				sideDir.normalize();

				Matrix viewMatrix;
				viewMatrix.setColumns(sideDir, upDir, -viewDir, Vector(0.0f, 0.0f, 0.0f, 1.0f));
				viewMatrix.setRow(Vector(
					-Vector::DotProduct(sideDir, eye),
					-Vector::DotProduct(upDir, eye),
					-Vector::DotProduct(-viewDir, eye),
					1.0f), 3
					);
					

				return viewMatrix;
			}

			Matrix CreateProjectionMatrixLH(float fov, 
										float aspectRatio,	
										float zNear, 
										float zFar)
			{
				Matrix matrix;

				float d = 1.0f / tanf(ToRadian(fov / 2.0f));
				float scaleZ = 1.0f / (zFar - zNear);

				matrix.data[0][0] = d / aspectRatio;
				matrix.data[1][1] = d;
				matrix.data[2][2] = zFar * scaleZ;
				matrix.data[3][2] = -zFar * zNear * scaleZ;
				matrix.data[2][3] = 1;
				matrix.data[3][3] = 0;

				return matrix;
			}

			Matrix CreateProjectionMatrixRH(float fov, 
										  float aspectRatio,	
										  float zNear, 
										  float zFar)
			{
				Matrix matrix;

				float d = 1.0f / tanf(ToRadian(fov / 2.0f));
				float scaleZ = 1.0f / (zNear - zFar);

				matrix.data[0][0] = d  / aspectRatio;
				matrix.data[1][1] = d;
				matrix.data[2][2] = zFar * scaleZ;
				matrix.data[3][2] = zFar * zNear * scaleZ;
				matrix.data[2][3] = -1;
				matrix.data[3][3] = 0;

				return matrix;
			}
		}

		namespace IntersectionUtils
		{
			bool PlanesIntersection(const Plane& rPlane, const Plane& rPlane2, const Plane& rPlane3, Vector& rPoint)
			{	
				float det = 
					rPlane.normal.x *
						(rPlane2.normal.y * rPlane3.normal.z - rPlane2.normal.z * rPlane3.normal.y)	-
					rPlane.normal.y *
						(rPlane2.normal.x * rPlane3.normal.z - rPlane2.normal.z * rPlane3.normal.x)	+
					rPlane.normal.z *
						(rPlane2.normal.x * rPlane3.normal.y - rPlane2.normal.y * rPlane3.normal.x);

				if(det == 0.0f || (det - 0.0001f < 0.0f) ) return false;

				float detX = 
					rPlane.d *
						(rPlane2.normal.y * rPlane3.normal.z - rPlane2.normal.z * rPlane3.normal.y)	-
					rPlane.normal.y *
						(rPlane2.d * rPlane3.normal.z - rPlane2.normal.z * rPlane3.d)	+
					rPlane.normal.z *
						(rPlane2.d * rPlane3.normal.y - rPlane2.normal.y * rPlane3.d);

				float detY = 
					rPlane.normal.x *
						(rPlane2.d * rPlane3.normal.z - rPlane2.normal.z * rPlane3.d)	-
					rPlane.d *
						(rPlane2.normal.x * rPlane3.normal.z - rPlane2.normal.z * rPlane3.normal.x)	+
					rPlane.normal.z *
						(rPlane2.normal.x * rPlane3.d - rPlane2.d * rPlane3.normal.x);

				float detZ = 
					rPlane.normal.x *
						(rPlane2.normal.y * rPlane3.d - rPlane2.d * rPlane3.normal.y)	-
					rPlane.normal.y *
						(rPlane2.normal.x * rPlane3.d - rPlane2.d * rPlane3.normal.x)	+
					rPlane.d *
						(rPlane2.normal.x * rPlane3.normal.y - rPlane2.normal.y * rPlane3.normal.x);

				rPoint.x = detX / det;
				rPoint.y = detY / det;
				rPoint.z = detZ / det;

				return true;
			}

			bool BaricentricTest(const Vector& p1, const Vector& p2, const Vector& p3,
					const Vector& origin, const Vector& rayDir, 
					float& d, float& w, float& v, float& u)
			{
				
				Vector e1 = p2 - p1;
				Vector e2 = p3 - p1;
			
				Vector p = Vector::CrossProduct(rayDir, e2);
				float det  = Vector::DotProduct(p, e1);

				if(det < -0.001f || det > 0.001f)
				{
					Vector t = origin - p1;
					float invDet = 1.0f / (det);
				
					u = Vector::DotProduct(p, t) * invDet;
					if(u < 0.0f || u > 1.0f)
					{
						return false;
					}

					Vector q = Vector::CrossProduct(t, e1);
					v = Vector::DotProduct(q, rayDir) * invDet;

					if(v < 0.0f || ((v + u) > 1.0f))
					{
						return false;
					}

					d = Vector::DotProduct(q, e2) * invDet;
					w = 1 - u - v;
					return true;
				}

				return false;
			}

			bool RayOBBIntersection(const OBB& obb, const Vector& origin, const Vector& rayDir, float& d)
			{
				float tmin = -FLT_MAX;
				float tmax = FLT_MAX;

				float eps = 0.0e-10f;
				d = 0.0f;

				Vector p = obb.middlePoint - origin;

				float e = Vector::DotProduct( obb.planes[0].dir, p );
				float f = Vector::DotProduct( obb.planes[0].dir, rayDir );

				if(f > eps || f < -eps)
				{
					float t1 = ( e + obb.planes[0].halfDistance ) / f;
					float t2 = ( e - obb.planes[0].halfDistance ) / f;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0) { return false; }
				}
				else
				{
					if( (-e - obb.planes[0].halfDistance > 0) || 
						(-e + obb.planes[0].halfDistance < 0) )
					{
						return false;
					}
				}

				e = Vector::DotProduct( obb.planes[1].dir, p );
				f = Vector::DotProduct( obb.planes[1].dir, rayDir );

				if(f > eps || f < -eps)
				{
					float t1 = ( e + obb.planes[1].halfDistance ) / f;
					float t2 = ( e - obb.planes[1].halfDistance ) / f;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0) { return false; }
				}
				else
				{
					if( (-e - obb.planes[1].halfDistance > 0) || 
						(-e + obb.planes[1].halfDistance < 0) )
					{
						return false;
					}
				}

				e = Vector::DotProduct( obb.planes[2].dir, p );
				f = Vector::DotProduct( obb.planes[2].dir, rayDir );

				if(f > eps || f < -eps)
				{
					float t1 = ( e + obb.planes[2].halfDistance ) / f;
					float t2 = ( e - obb.planes[2].halfDistance ) / f;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0) { return false; }
				}
				else
				{
					if( (-e - obb.planes[2].halfDistance > 0) || 
						(-e + obb.planes[2].halfDistance < 0) )
					{
						return false;
					}
				}

				if(tmin > 0.0f) 
				{
					d = tmin;
				} 
				else
				{
					d = tmax;
				}

				return true;
			}

			bool RayAABBIntersectionSlab(const AABB& rAabb, const Vector& rOrigin, const Vector& rDir, float& d)
			{
				float tmin = -FLT_MAX;
				float tmax = FLT_MAX;

				float eps = 0.0001f;
				d = 0.0f;

				float e = rAabb.centerPoint.x - rOrigin.x;
				float f = rDir.x;
				float invF = 1.0f / f;

				if(f > eps || f < -eps)
				{
					float t1 = ( e + rAabb.halfDistance.x ) * invF;
					float t2 = ( e - rAabb.halfDistance.x ) * invF;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0.0f) { return false; }
				}
				else
				{
					if( ((-e - rAabb.halfDistance.x) > 0.0f) || 
						((-e + rAabb.halfDistance.x) < 0.0f) )
					{
						return false;
					}
				}

				e = rAabb.centerPoint.y - rOrigin.y;
				f = rDir.y;
				invF = 1.0f / f;

				if(f > eps || f < -eps)
				{
					float t1 = ( e + rAabb.halfDistance.y ) * invF;
					float t2 = ( e - rAabb.halfDistance.y ) * invF;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0.0f) { return false; }
				}
				else
				{
					if( ((-e - rAabb.halfDistance.y) > 0.0f) || 
						((-e + rAabb.halfDistance.y) < 0.0f) )
					{
						return false;
					}
				}

				e = rAabb.centerPoint.z - rOrigin.z;
				f = rDir.z;
				invF = 1.0f / f;

				if(f > eps || f < -eps)
				{
					float t1 = ( e + rAabb.halfDistance.z ) * invF;
					float t2 = ( e - rAabb.halfDistance.z ) * invF;

					if(t1 > t2)
					{
						t1 = t1 + t2;
						t2 = t1 - t2;
						t1 = t1 - t2;
					}

					if(t1 > tmin) { tmin = t1; }
					if(t2 < tmax) { tmax = t2; }

					if(tmin > tmax) { return false; }
					if(tmax < 0.0f) { return false; }
				}
				else
				{
					if( ((-e - rAabb.halfDistance.z) > 0.0f) || 
						((-e + rAabb.halfDistance.z) < 0.0f) )
					{
						return false;
					}
				}

				if(tmin > 0.0f) 
				{
					d = tmin;
				} 
				else
				{
					d = tmax;
				}

				return true;
			}

			bool RayAABBIntersection(const AABB& rAabb, const Vector& rOrigin, const Vector& rDir, float& d)
			{
				Vector xPoint = rAabb.centerPoint;
				Vector yPoint = rAabb.centerPoint;
				Vector zPoint = rAabb.centerPoint;

				Vector xDir(1.0f, 0.0f, 0.0f);
				Vector yDir(0.0f, 1.0f, 0.0f);
				Vector zDir(0.0f, 0.0f, 1.0f);
				
				float minX = rAabb.centerPoint.x - rAabb.halfDistance.x;
				float maxX = rAabb.centerPoint.x + rAabb.halfDistance.x;
				float minY = rAabb.centerPoint.y - rAabb.halfDistance.y;
				float maxY = rAabb.centerPoint.y + rAabb.halfDistance.y;
				float minZ = rAabb.centerPoint.z - rAabb.halfDistance.z;
				float maxZ = rAabb.centerPoint.z + rAabb.halfDistance.z;

				if(rDir.x < 0.0f){
					xPoint.x = maxX; xDir.x = -1;
				}
				else{
					xPoint.x = minX;
				}

				if(rDir.y < 0.0f){
					yPoint.y = maxY; yDir.y = -1;
				}
				else{
					yPoint.y = minY; 
				}

				if(rDir.z < 0.0f){
					zPoint.z = maxZ;
					zDir.z = -1;
				}
				else {
					zPoint.z = minZ;
				}

				float xD = xPoint.x - rOrigin.x;
				float yD = yPoint.y - rOrigin.y; 
				float zD = zPoint.z - rOrigin.z;

				xD = rDir.x == 0 ? 0.0f : xD / rDir.x;
				yD = rDir.y == 0 ? 0.0f : yD / rDir.y;
				zD = rDir.z == 0 ? 0.0f : zD / rDir.z;
				
				float max = 0;
				float x;
				float y = rOrigin.y + rDir.y * xD;
				float z = rOrigin.z + rDir.z * xD;
				
				if((y >= (minY) && y <= (maxY)) && 
					(z >= (minZ) && z <= (maxZ)) && xD > max) { max = xD; }

				x = rOrigin.x +  rDir.x * yD;
				z = rOrigin.z +  rDir.z * yD;
				
				if((x >= (minX) && x <= (maxX)) &&
					(z >= (minZ) && z <= (maxZ)) && yD > max) { max = yD; }

				y = rOrigin.y +  rDir.y * zD;
				x = rOrigin.x +  rDir.x * zD;
				
				if((y >= (minY) && y <= (maxY)) && 
					(x >= (minX) && x <= (maxX)) && zD > max) { max = zD; }
				
				d = max;
				return  d > 0.0f;
			}

			bool RayAABBIntersectionSlab(const AABB& aabb, const Vector& pos, 
				const Vector& dir, float& tmin, float& tmax)
			{
				float minX = aabb.centerPoint.x - aabb.halfDistance.x;
				float maxX = aabb.centerPoint.x + aabb.halfDistance.x;
				float minY = aabb.centerPoint.y - aabb.halfDistance.y;
				float maxY = aabb.centerPoint.y + aabb.halfDistance.y;
				float minZ = aabb.centerPoint.z - aabb.halfDistance.z;
				float maxZ = aabb.centerPoint.z + aabb.halfDistance.z;

				float lo = (1.0f / dir.x)*(minX - pos.x);
				float hi = (1.0f / dir.x)*(maxX - pos.x);
				tmin  = min(lo, hi);
				tmax = max(lo, hi);
 
				float lo1 = (1.0f / dir.y)*(minY - pos.y);
				float hi1 = (1.0f / dir.y)*(maxY - pos.y);
				tmin  = max(tmin, min(lo1, hi1));
				tmax = min(tmax, max(lo1, hi1));
 
				float lo2 = (1.0f / dir.z)*(minZ - pos.z);
				float hi2 = (1.0f / dir.z)*(maxZ - pos.z);
				tmin  = max(tmin, min(lo2, hi2));
				tmax = min(tmax, max(lo2, hi2));
 
				return (tmin <= tmax) && (tmax > 0.f);
			}

			bool OverlapOBBOBB(const OBB& rOBB, const OBB& rOBB2)
			{
				Vector center = rOBB.middlePoint;
				Vector center2 = rOBB2.middlePoint;

				Vector normals[15] = 
				{
					rOBB.planes[0].dir,
					rOBB.planes[1].dir,
					rOBB.planes[2].dir,
					rOBB2.planes[0].dir,
					rOBB2.planes[1].dir,
					rOBB2.planes[2].dir,
					Vector::CrossProduct(rOBB.planes[0].dir, rOBB2.planes[0].dir),
					Vector::CrossProduct(rOBB.planes[0].dir, rOBB2.planes[1].dir),
					Vector::CrossProduct(rOBB.planes[0].dir, rOBB2.planes[2].dir),
					Vector::CrossProduct(rOBB.planes[1].dir, rOBB2.planes[0].dir),
					Vector::CrossProduct(rOBB.planes[1].dir, rOBB2.planes[1].dir),
					Vector::CrossProduct(rOBB.planes[1].dir, rOBB2.planes[2].dir),
					Vector::CrossProduct(rOBB.planes[2].dir, rOBB2.planes[0].dir),
					Vector::CrossProduct(rOBB.planes[2].dir, rOBB2.planes[1].dir),
					Vector::CrossProduct(rOBB.planes[2].dir, rOBB2.planes[2].dir)
				};

				float hx = rOBB.planes[0].halfDistance;
				float hy = rOBB.planes[1].halfDistance;
				float hz = rOBB.planes[2].halfDistance;

				// first OBB points 
				Vector points[8] = 
				{
					Vector(center.x + hx, center.y + hy, center.z + hz, 1.0f),
					Vector(center.x - hx, center.y + hy, center.z + hz, 1.0f),
					Vector(center.x - hx, center.y - hy, center.z + hz, 1.0f),
					Vector(center.x + hx, center.y - hy, center.z + hz, 1.0f),
					Vector(center.x + hx, center.y + hy, center.z - hz, 1.0f),
					Vector(center.x - hx, center.y + hy, center.z - hz, 1.0f),
					Vector(center.x - hx, center.y - hy, center.z - hz, 1.0f),
					Vector(center.x + hx, center.y - hy, center.z - hz, 1.0f)
				};

				hx = rOBB2.planes[0].halfDistance;
				hy = rOBB2.planes[1].halfDistance;
				hz = rOBB2.planes[2].halfDistance;

				// second OBB points 
				Vector points2[8] = 
				{
					Vector(center2.x + hx, center2.y + hy, center2.z + hz, 1.0f),
					Vector(center2.x - hx, center2.y + hy, center2.z + hz, 1.0f),
					Vector(center2.x - hx, center2.y - hy, center2.z + hz, 1.0f),
					Vector(center2.x + hx, center2.y - hy, center2.z + hz, 1.0f),
					Vector(center2.x + hx, center2.y + hy, center2.z - hz, 1.0f),
					Vector(center2.x - hx, center2.y + hy, center2.z - hz, 1.0f),
					Vector(center2.x - hx, center2.y - hy, center2.z - hz, 1.0f),
					Vector(center2.x + hx, center2.y - hy, center2.z - hz, 1.0f)
				};

				for(uint32 i = 0; i < 15; i++)
				{
					float min = FLT_MAX, max = -FLT_MAX;
					float min2 = FLT_MAX, max2 = -FLT_MAX;

					for(uint32 j = 0; j < 8; j++)
					{
						float projection = Vector::DotProduct(points[j], normals[i]);
						float projection2 = Vector::DotProduct(points2[j], normals[i]);

						if(projection < min) min = projection;
						if(projection > max) max = projection;
						if(projection2 < min2) min2 = projection2;
						if(projection2 > max2) max2 = projection2;
					}

					if((((abs(min - min2) + abs(min - max2)) - (max2 - min2) > 0.001f) && 
						((abs(max - min2) + abs(max - max2)) - (max2 - min2) > 0.001f)) && 
						(((abs(min2 - min) + abs(min2 - max)) - (max - min) > 0.001f) && 
						((abs(max2 - min) + abs(max2 - max)) - (max - min) > 0.001f)))
					{
						return false;
					}
				}

				return true;
			}

			bool OverlapAABBAABB(const AABB& rAABB, const AABB& rAABB2)
			{
				if( abs(rAABB.centerPoint.x - rAABB2.centerPoint.x) 
					> (rAABB.halfDistance.x + rAABB2.halfDistance.x) ) return false;
				if( abs(rAABB.centerPoint.y - rAABB2.centerPoint.y) 
					> (rAABB.halfDistance.y + rAABB2.halfDistance.y) ) return false;
				if( abs(rAABB.centerPoint.z - rAABB2.centerPoint.z) 
					> (rAABB.halfDistance.z + rAABB2.halfDistance.z) ) return false;

				return true;
			}

			INTERSECTION_RESULT FrustumAABBIntersection(const ViewFrustum& rFrustum, const AABB& rAABB) 
			{
				Vector point(
					rAABB.centerPoint.x + rAABB.halfDistance.x,
					rAABB.centerPoint.y + rAABB.halfDistance.y,
					rAABB.centerPoint.z + rAABB.halfDistance.z);

				Vector point2(
					rAABB.centerPoint.x + rAABB.halfDistance.x,
					rAABB.centerPoint.y + rAABB.halfDistance.y,
					rAABB.centerPoint.z - rAABB.halfDistance.z);

				Vector point3(
					rAABB.centerPoint.x + rAABB.halfDistance.x,
					rAABB.centerPoint.y - rAABB.halfDistance.y,
					rAABB.centerPoint.z - rAABB.halfDistance.z);

				Vector point4(
					rAABB.centerPoint.x + rAABB.halfDistance.x,
					rAABB.centerPoint.y - rAABB.halfDistance.y,
					rAABB.centerPoint.z + rAABB.halfDistance.z);

				Vector point5(
					rAABB.centerPoint.x - rAABB.halfDistance.x,
					rAABB.centerPoint.y - rAABB.halfDistance.y,
					rAABB.centerPoint.z - rAABB.halfDistance.z);

				Vector point6(
					rAABB.centerPoint.x - rAABB.halfDistance.x,
					rAABB.centerPoint.y + rAABB.halfDistance.y,
					rAABB.centerPoint.z - rAABB.halfDistance.z);

				Vector point7(
					rAABB.centerPoint.x - rAABB.halfDistance.x,
					rAABB.centerPoint.y - rAABB.halfDistance.y,
					rAABB.centerPoint.z + rAABB.halfDistance.z);

				Vector point8(
					rAABB.centerPoint.x - rAABB.halfDistance.x,
					rAABB.centerPoint.y + rAABB.halfDistance.y,
					rAABB.centerPoint.z + rAABB.halfDistance.z);

				for(uint32 i = 0; i < 6; i++)
				{
					const Plane& rPlane = rFrustum.planes[i];

					if(Vector::DotProduct(rPlane.normal, point) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point2) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point3) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point4) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point5) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point6) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point7) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point8) + rPlane.d >= 0) continue;

					return OUTSIDE;
				}

				return INTERSECT;
			}

			INTERSECTION_RESULT FrustumOBBIntersection(const ViewFrustum& rFrustum, const OBB& rOBB)
			{
				float fd = rOBB.planes[0].halfDistance;
				float sd = rOBB.planes[1].halfDistance;
				float td = rOBB.planes[2].halfDistance;

				Vector point = rOBB.middlePoint + 
					rOBB.planes[0].dir * fd +
					rOBB.planes[1].dir * sd + 
					rOBB.planes[2].dir * td;

				Vector point2 = rOBB.middlePoint + 
					rOBB.planes[0].dir * fd +
					rOBB.planes[1].dir * sd - 
					rOBB.planes[2].dir * td;

				Vector point3 = rOBB.middlePoint + 
					rOBB.planes[0].dir * fd -
					rOBB.planes[1].dir * sd + 
					rOBB.planes[2].dir * td;

				Vector point4 = rOBB.middlePoint +
					rOBB.planes[0].dir * fd -
					rOBB.planes[1].dir * sd - 
					rOBB.planes[2].dir * td;

				Vector point5 = rOBB.middlePoint - 
					rOBB.planes[0].dir * fd -
					rOBB.planes[1].dir * sd - 
					rOBB.planes[2].dir * td;

				Vector point6 = rOBB.middlePoint - 
					rOBB.planes[0].dir * fd +
					rOBB.planes[1].dir * sd - 
					rOBB.planes[2].dir * td;

				Vector point7 = rOBB.middlePoint - 
					rOBB.planes[0].dir * fd -
					rOBB.planes[1].dir * sd + 
					rOBB.planes[2].dir * td;

				Vector point8 = rOBB.middlePoint - 
					rOBB.planes[0].dir * fd +
					rOBB.planes[1].dir * sd + 
					rOBB.planes[2].dir * td;

				for(uint32 i = 0; i < 6; i++)
				{
					const Plane& rPlane = rFrustum.planes[i];

					if(Vector::DotProduct(rPlane.normal, point) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point2) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point3) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point4) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point5) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point6) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point7) + rPlane.d >= 0) continue;
					if(Vector::DotProduct(rPlane.normal, point8) + rPlane.d >= 0) continue;

					return OUTSIDE;
				}

				return INTERSECT;
			}

			INTERSECTION_RESULT SphereAABBIntersection(const Vector& rCenter, float radius, const AABB& rAABB)
			{
				float d = 0.0f;

				float xMin = rAABB.centerPoint.x - rAABB.halfDistance.x;
				float xMax = rAABB.centerPoint.x + rAABB.halfDistance.x;

				float yMin = rAABB.centerPoint.y - rAABB.halfDistance.y;
				float yMax = rAABB.centerPoint.y + rAABB.halfDistance.y;

				float zMin = rAABB.centerPoint.z - rAABB.halfDistance.z;
				float zMax = rAABB.centerPoint.z + rAABB.halfDistance.z;

				if(rCenter.x < xMin) {
					d += (rCenter.x - xMin) * (rCenter.x - xMin);
				}  else if(rCenter.x > xMax) {
					d += (rCenter.x - xMax) * (rCenter.x - xMax);
				}

				if(rCenter.y < yMin) {
					d += (rCenter.y - yMin) * (rCenter.y - yMin);
				}  else if(rCenter.y > yMax) {
					d += (rCenter.y - yMax) * (rCenter.y - yMax);
				}

				if(rCenter.z < zMin) {
					d += (rCenter.z - zMin) * (rCenter.z - zMin);
				}  else if(rCenter.z > zMax) {
					d += (rCenter.z - zMax) * (rCenter.z - zMax);
				}

				if(d > (radius * radius))
				{
					return OUTSIDE;
				}

				return INTERSECT;
			}

			INTERSECTION_RESULT SphereOBBIntersection(const Vector& rCenter, float radius, const OBB& rOBB)
			{
				float d = 0.0f;

				Vector sphereCenter = rCenter * Matrix(
													rOBB.planes[0].dir, 
													rOBB.planes[1].dir,
													rOBB.planes[2].dir,
													Vector());

				float uMin = Vector::DotProduct(rOBB.planes[0].dir, 
					rOBB.middlePoint + rOBB.planes[0].dir * rOBB.planes[0].halfDistance);
				float uMax = Vector::DotProduct(rOBB.planes[0].dir, 
					rOBB.middlePoint - rOBB.planes[0].dir * rOBB.planes[0].halfDistance);

				float vMin = Vector::DotProduct(rOBB.planes[1].dir, 
					rOBB.middlePoint + rOBB.planes[1].dir * rOBB.planes[1].halfDistance);
				float vMax = Vector::DotProduct(rOBB.planes[1].dir, 
					rOBB.middlePoint - rOBB.planes[1].dir * rOBB.planes[1].halfDistance);

				float kMin = Vector::DotProduct(rOBB.planes[1].dir, 
					rOBB.middlePoint + rOBB.planes[1].dir * rOBB.planes[1].halfDistance);
				float kMax = Vector::DotProduct(rOBB.planes[1].dir, 
					rOBB.middlePoint - rOBB.planes[1].dir * rOBB.planes[1].halfDistance);

				if(rCenter.x < uMin) {
					d += (rCenter.x - uMin) * (rCenter.x - uMin);
				}  else if(rCenter.x > uMax) {
					d += (rCenter.x - uMax) * (rCenter.x - uMax);
				}

				if(rCenter.y < vMin) {
					d += (rCenter.y - vMin) * (rCenter.y - vMin);
				}  else if(rCenter.y > vMax) {
					d += (rCenter.y - vMax) * (rCenter.y - vMax);
				}

				if(rCenter.z < kMin) {
					d += (rCenter.z - kMin) * (rCenter.z - kMin);
				}  else if(rCenter.z > kMax) {
					d += (rCenter.z - kMax) * (rCenter.z - kMax);
				}

				if(d > (radius * radius))
				{
					return OUTSIDE;
				}

				return INTERSECT;
			}	 

			INTERSECTION_RESULT SphereSphereIntersection(
				const Vector& rCenter, 
				float radius, 
				const Vector& rCenter2,
				float radius2)
			{
				Vector dist = rCenter - rCenter2;
				float length = dist.length();

				if(length <= (radius + radius2)) 
				{
					return INTERSECT;
				}
				else
				{
					return OUTSIDE;
				}
			}
		}
	}
}