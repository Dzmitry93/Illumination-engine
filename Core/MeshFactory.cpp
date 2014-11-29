#include "MeshFactory.h"

namespace OGE
{
	namespace resource
	{
		Mesh* MeshFactory::createMesh(
			VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, 
			const IntrusivePtr<Material>& pMaterial,
			PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			string name = "mesh" + GUIDUtil::generateRandomGUID(); 
			uint64 guid = HashUtil::getHash(name);

			Mesh* pMesh = new Mesh(pVertexBuffer, pIndexBuffer, primitiveTopology, pMaterial);
			
			pMesh->setResourceName(name);
			pMesh->setGUID(guid);

			return pMesh;
		}

		Mesh* MeshFactory::createMesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			return createMesh(pVertexBuffer, pIndexBuffer, IntrusivePtr<Material>(), primitiveTopology);
		}

		Mesh* MeshFactory::createMesh(OGEModelData* pModelData, IntrusivePtr<Material>& pMaterial, PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			Mesh* pMesh = new Mesh(pModelData->pVerticesBuffer, pModelData->pIndexesBuffer, primitiveTopology, pMaterial);

			uint64 guid = HashUtil::getHash(pModelData->geometryName);
			pMesh->setGUID(guid);
			pMesh->setResourceName(pModelData->geometryName);
			
			return pMesh;
		}

		IterableMesh* MeshFactory::createIterableMesh(
				VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, 
				const IntrusivePtr<Material>& pMaterial,
				PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			string name = "mesh" + GUIDUtil::generateRandomGUID();
			uint64 guid = HashUtil::getHash(name);

			IterableMesh* pMesh = new IterableMesh(pVertexBuffer, pIndexBuffer,  primitiveTopology, pMaterial);

			pMesh->createIterableData(Matrix());
			pMesh->setResourceName(name);
			pMesh->setGUID(guid);

			return pMesh;
		}

		IterableMesh* MeshFactory::createIterableMesh(const IntrusivePtr<BaseMesh>& pSrcSystemMesh)
		{
			return createIterableMesh(
				pSrcSystemMesh->getVerticesBuffer(), pSrcSystemMesh->getIndexesBuffer(),
				pSrcSystemMesh->getMaterial(), pSrcSystemMesh->getPrimitiveTopology());

		}
	}
}