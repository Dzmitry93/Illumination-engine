#include "IterableMesh.h"

namespace OGE
{
	namespace resource
	{
		void IterableMesh::createIterableData(const Matrix& rWorldMatrix)
		{
			if(pIndexesBuffer->isNULL())
			{
				error_msg("indexes buffer null. cannot create iterable data");
				return ;
			}

			V3TNT2Vertex* pTempVB = (V3TNT2Vertex*)((Buffer*) pVerticesBuffer)->getPointer();
			uint32* pTempIB = (uint32*) ((Buffer*) pIndexesBuffer)->getPointer();

			uint32 countVertices = pVerticesBuffer->getSize() / pVerticesBuffer->getStride();
			uint32 countIndexes= pIndexesBuffer->getSize() / pIndexesBuffer->getStride();

			safe_array_release(pIterableData);

			pIterableData = new V3TNT2Triangle[countIndexes/3];
			uint32 primitiviIndex = 0;

			for(uint32 i = 0; i < countIndexes; i+= 3, primitiviIndex++)
			{
				V3TNT2Triangle triangle;
			
				uint32 index = pTempIB[i];
				uint32 index2 = pTempIB[i+1];
				uint32 index3 = pTempIB[i+2];

				triangle.v1 = pTempVB[index];
				triangle.v2 = pTempVB[index2];
				triangle.v3 = pTempVB[index3];

				Vector  v1(triangle.v1.x, triangle.v1.y, triangle.v1.z);
				Vector  v2(triangle.v2.x, triangle.v2.y, triangle.v2.z);
				Vector  v3(triangle.v3.x, triangle.v3.y, triangle.v3.z);

				v1 *= rWorldMatrix;
				v2 *= rWorldMatrix;
				v3 *= rWorldMatrix;

				triangle.v1.x = v1.x;
				triangle.v1.y = v1.y;
				triangle.v1.z = v1.z;

				triangle.v2.x = v2.x;
				triangle.v2.y = v2.y;
				triangle.v2.z = v2.z;

				triangle.v3.x = v3.x;
				triangle.v3.y = v3.y;
				triangle.v3.z = v3.z;

				pIterableData[primitiviIndex] = triangle;
			}
		}
	}
}