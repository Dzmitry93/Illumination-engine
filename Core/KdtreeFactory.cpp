#include "KdtreeFactory.h"

namespace OGE
{
	namespace Collections
	{
		Kdtree<MeshTriangle>* KdtreeFactory::createTrianglesKdtree(
			const List<SceneObject<IntrusivePtr<BaseMesh>>*>& rDataList, const SceneObject<Camera*>& rCamera,
				const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxDepth, uint32 maxObjectsPerNode)
		{
			ConstListIterator<SceneObject<IntrusivePtr<BaseMesh>>*> it = rDataList.citerator();

			List<MeshTriangle> triangles;

			while(it.hasNext())
			{
				SceneObject<IntrusivePtr<BaseMesh>>* pObject = it.next();
				const Matrix& rWorldMatrix = pObject->getWorldMatrix();
				

				IterableMesh* pIterable = MeshFactory::createIterableMesh(pObject->getSceneObject());
		
				if(pIterable->isIterable())
				{
					ArrayIterator<V3TNT2Triangle> triangleIt = pIterable->iterator();
					while(triangleIt.hasNext())
					{
						MeshTriangle meshTriangle;
						V3TNT2Triangle triangle = TransformUtils::apllyTransform(triangleIt.next(), rWorldMatrix, Matrix());
						meshTriangle.v1 = Vector(triangle.v1.x, triangle.v1.y, triangle.v1.z);
						meshTriangle.v2 = Vector(triangle.v2.x, triangle.v2.y, triangle.v2.z);
						meshTriangle.v3 = Vector(triangle.v3.x, triangle.v3.y, triangle.v3.z);
						meshTriangle.n1 = Vector(triangle.v1.nx, triangle.v1.ny, triangle.v1.nz);
						meshTriangle.n2 = Vector(triangle.v2.nx, triangle.v2.ny, triangle.v2.nz);
						meshTriangle.n3 = Vector(triangle.v3.nx, triangle.v3.ny, triangle.v3.nz);
						meshTriangle.pMaterial = pIterable->getMaterial();

						triangles.push_back(meshTriangle);
					}
				}
				pIterable->release();
			}
	
			TriangleKdtree* pKdtree = new TriangleKdtree(rVolume, rPosition, step, maxObjectsPerNode, maxDepth);
			pKdtree->createKdtree(triangles);

			return pKdtree;
		}
	}
}