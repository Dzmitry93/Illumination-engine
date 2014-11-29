#include "TriangleKdtree.h"

namespace OGE
{
	namespace Collections
	{
		void TriangleKdtree::traverseRay(const Vector& rOrigin, const Vector& rDir,
			IntersectionResult& rResult)
		{
			TreeIterator<KdtreeData<MeshTriangle>*> it = mTree.root();
			AABB& rAABB = it.data()->aabb;

			float tnear;
			float tfar;
			bool hitBox = IntersectionUtils::RayAABBIntersectionSlab(rAABB, rOrigin, rDir, tnear, tfar);
			
			if(tnear < 0.0f) 
			{
				tnear = 0.0f;
			}

			if(!hitBox || tnear >= tfar) 
			{
				return;
			}

			traverseRay(it, rOrigin, rDir, tnear, tfar, rResult);
		}

		void TriangleKdtree::traverseRay(TreeIterator<KdtreeData<MeshTriangle>*>& rWhereIt, 
				const Vector& rOrigin, const Vector& rDir,
				float tnear, float tfar,
				IntersectionResult& rResult)
		{
			if(rWhereIt.getCurrentNode()->childs.size() > 0)
			{
				COMPARE_AGREEMENT  agr = rWhereIt.data()->agr;

				float k;
				float pos;
				if(agr == COMPARE_X) { k = 1.0f / rDir.x; pos = rOrigin.x; } else
				if(agr == COMPARE_Y) { k = 1.0f / rDir.y; pos = rOrigin.y; } else
				{ k = 1.0f / rDir.z; pos = rOrigin.z; }

				float split = rWhereIt.data()->split - pos * k;

				if(split > tfar || split <= 0.0f)  
				{
						rWhereIt.down();
						traverseRay(rWhereIt, rOrigin, rDir, tnear, tfar, rResult);
						rWhereIt.up();
				}
				else
				if(split < tnear)
				{
					rWhereIt.next();
					rWhereIt.down();
					traverseRay(rWhereIt, rOrigin, rDir, tnear, tfar, rResult);
					rWhereIt.up();
					rWhereIt.previous();
				}
				else
				{
					rWhereIt.down();
					traverseRay(rWhereIt, rOrigin, rDir, tnear, split, rResult);
					rWhereIt.up();

					if(rResult.d <= 0.0f)
					{
						rWhereIt.next();
						rWhereIt.down();
						traverseRay(rWhereIt, rOrigin, rDir, split, tfar, rResult);
						rWhereIt.up();
						rWhereIt.previous();
					}
				}
			}
			else
			{ 
				const List<MeshTriangle>& rNodeData = rWhereIt.data()->data;
				if(rNodeData.size() > 0)
				{
					ConstListIterator<MeshTriangle> it = rNodeData.citerator();

					float d = 0.0f;
					float u, v, w;
					bool found = false;
					float maxD = FLT_MAX;

					while(it.hasNext())
					{
						const MeshTriangle& rPrimitive = it.next();
						found = IntersectionUtils::BaricentricTest(rPrimitive.v1, 
							rPrimitive.v2, rPrimitive.v3, rOrigin, rDir, d, w, v, u);

						if(found && d < maxD && d > 0.0f)
						{
							rResult.triangle = rPrimitive;
							rResult.u = u;
							rResult.v = v;
							rResult.w = w;
							rResult.d = d;
							maxD = d;
						}
					}
				}
			}
		}
	}
}