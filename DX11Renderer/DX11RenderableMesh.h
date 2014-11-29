#ifndef __RENDERABLE_MESH_H__
#define __RENDERABLE_MESH_H__

#include "DX11ResourceBuffer.h"
#include "Material.h"
#include "DX11Format.h"

using OGE::resource::PRIMITIVE_TOPOLOGY;
using OGE::resource::Material;

namespace OGE
{
	namespace render
	{
		class DX11RenderableMesh : public DX11RenderResource
		{
		public:
			DX11RenderableMesh (DX11ResourceBuffer* pVerticesBuffer, DX11ResourceBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial);
			DX11RenderableMesh (const DX11RenderableMesh& rMesh);
			virtual ~DX11RenderableMesh();

			DX11ResourceBuffer* getVerticesBuffer() const;
			DX11ResourceBuffer* getIndexesBuffer() const;

			uint32 getCountVertices() const; 
			uint32 getCountPrimitives() const;
			uint32 getCountIndexes() const;

			IntrusivePtr<Material> getMaterial() const;

			BUFFER_FORMAT getVertexFormat() const;
			PRIMITIVE_TOPOLOGY getPrimitiveTopology() const;

			void release();

		private:
			DX11ResourceBuffer* pVerticesBuffer;
			DX11ResourceBuffer* pIndexesBuffer;

			PRIMITIVE_TOPOLOGY primitiveTopology;
			IntrusivePtr<Material> pMaterial;
		};

		inline DX11RenderableMesh::DX11RenderableMesh(DX11ResourceBuffer* pVerticesBuffer, 
				DX11ResourceBuffer* pIndexesBuffer,  PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial) : 
					pVerticesBuffer(pVerticesBuffer), pIndexesBuffer(pIndexesBuffer),
					primitiveTopology(primitiveTopology), pMaterial(pMaterial) {}


		inline DX11RenderableMesh::~DX11RenderableMesh() 
		{ 
			release();
		}

		inline DX11ResourceBuffer* DX11RenderableMesh::getVerticesBuffer() const 
		{
			return (DX11ResourceBuffer*)pVerticesBuffer;
		}

		inline DX11ResourceBuffer* DX11RenderableMesh::getIndexesBuffer() const
		{
			return (DX11ResourceBuffer*)pIndexesBuffer;
		}

		
		inline uint32 DX11RenderableMesh::getCountIndexes() const
		{
			return pIndexesBuffer->getSize() / pIndexesBuffer->getStride();
		}

		inline uint32 DX11RenderableMesh::getCountVertices() const
		{
			return pVerticesBuffer->getSize() / pVerticesBuffer->getStride();
		}

		inline uint32 DX11RenderableMesh::getCountPrimitives() const
		{
			return pIndexesBuffer->getSize() / ( 3 * pIndexesBuffer->getStride());
		}

		inline IntrusivePtr<Material> DX11RenderableMesh::getMaterial() const
		{
			return pMaterial;
		}

		inline BUFFER_FORMAT DX11RenderableMesh::getVertexFormat() const
		{
			return pVerticesBuffer->getFormat();
		}

		inline PRIMITIVE_TOPOLOGY DX11RenderableMesh::getPrimitiveTopology() const
		{
			return primitiveTopology;
		}

		inline void DX11RenderableMesh::release()
		{
			pVerticesBuffer->release();
			pIndexesBuffer->release();
		}
	}
}

#endif
