#include "DX11MeshFactory.h"

namespace OGE
{
	namespace render
	{
		DX11RenderableMesh* DX11MeshFactory::createRenderableMesh(
			ID3D11Device* pDevice,
			const IntrusivePtr<BaseMesh>& pSrcSystemMesh, D3D11_USAGE usage)
		{
			string name = pSrcSystemMesh->getResourceName();
			uint64 guid = pSrcSystemMesh->getGUID();

			const Buffer* pVerticesBuffer = pSrcSystemMesh->getVerticesBuffer();
			const Buffer* pIndexesBuffer = pSrcSystemMesh->getIndexesBuffer();
			 
			DX11ResourceBuffer* pD3DVerticesBuffer = DX11BufferFactory::createVertexBuffer(pDevice, usage,
				pSrcSystemMesh->getVertexFormat(), 
				pVerticesBuffer->getPointer(), pVerticesBuffer->getSize());

			DX11ResourceBuffer* pD3DIndexesBuffer = NULL;
			
			if(!pIndexesBuffer->isNULL())
			{
				pD3DIndexesBuffer = DX11BufferFactory::createIndexBuffer(pDevice, usage, 
				pIndexesBuffer->getPointer(), pIndexesBuffer->getSize());
			}
			else
			{
				pD3DIndexesBuffer = new DX11ResourceBuffer(NULL, FORMAT_UNKNOW);
			}

			DX11RenderableMesh* pRenderableMesh = new DX11RenderableMesh(
				pD3DVerticesBuffer, pD3DIndexesBuffer,
				pSrcSystemMesh->getPrimitiveTopology(),
				pSrcSystemMesh->getMaterial());

			pRenderableMesh->setResourceName(name);
			pRenderableMesh->setGUID(guid);

			return pRenderableMesh;
		}

		DX11RenderableMesh* DX11MeshFactory::createRenderableMesh(
				ID3D11Device* pDevice,
				DX11ResourceBuffer* pD3DVerticesBuffer, DX11ResourceBuffer* pD3DIndexesBuffer,
				const IntrusivePtr<Material>& pMaterial, 
				D3D11_USAGE usage,
				PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			string name = "mesh" + GUIDUtil::generateRandomGUID();
			uint64 guid = HashUtil::getHash(name);

			DX11RenderableMesh* pRenderableMesh = new DX11RenderableMesh(
				pD3DVerticesBuffer, pD3DIndexesBuffer, primitiveTopology,
				pMaterial);

			pRenderableMesh->setResourceName(name);
			pRenderableMesh->setGUID(guid);

			return pRenderableMesh;

		}

		DX11RenderableMesh* DX11MeshFactory::createRenderableMesh(
				ID3D11Device* pDevice,
				DX11ResourceBuffer* pVertices, DX11ResourceBuffer* pIndexes,
				D3D11_USAGE usage,
				PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			IntrusivePtr<Material> pMaterial;
			return createRenderableMesh(pDevice, pVertices, pIndexes, pMaterial, usage, primitiveTopology);
		}
	}
}