#ifndef __BASE_MESH_H__
#define __BASE_MESH_H__

#include "BufferFactory.h"
#include "Material.h"
#include "ResourceItem.h"

namespace OGE
{
	namespace resource
	{
		class BaseMesh : public ResourceItem
		{
		public:
			BaseMesh(
				VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial);

			BaseMesh(const BaseMesh& rMesh);

			~BaseMesh();

			VertexBuffer* getVerticesBuffer() const;
			IndexBuffer* getIndexesBuffer() const;

			uint32 getCountVertices() const; 
			uint32 getCountPrimitives() const;
			uint32 getCountIndexes() const;

			IntrusivePtr<Material> getMaterial() const;

			BUFFER_FORMAT getVertexFormat() const;
			PRIMITIVE_TOPOLOGY getPrimitiveTopology() const;

		protected:

			VertexBuffer* pVerticesBuffer;
			IndexBuffer* pIndexesBuffer;

			PRIMITIVE_TOPOLOGY primitiveTopology;
			IntrusivePtr<Material> pMaterial;

			void destroyObject();
			void shallowCopy(const ResourceItem& rMesh);
		};

		inline BaseMesh::BaseMesh(
				VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial) :

				primitiveTopology(primitiveTopology), pMaterial(pMaterial) 
		{
			this->pVerticesBuffer = BufferFactory::createBuffer(pVerticesBuffer);
			if(!pIndexesBuffer->isNULL())
			{
				this->pIndexesBuffer = BufferFactory::createBuffer(pIndexesBuffer);
			}
			else
			{
				this->pIndexesBuffer = new IndexBuffer(0);
			}
		}

		inline BaseMesh::BaseMesh(const BaseMesh& rMesh) :
				pVerticesBuffer(rMesh.pVerticesBuffer), pIndexesBuffer(rMesh.pIndexesBuffer),
				primitiveTopology(rMesh.primitiveTopology), pMaterial(pMaterial) {}

		inline BaseMesh::~BaseMesh() { release(); }

		inline void BaseMesh::destroyObject() 
		{
			if(pVerticesBuffer != NULL)
			{
				pVerticesBuffer->release();
				pVerticesBuffer = NULL;
			}
			if(pIndexesBuffer != NULL)
			{
				pIndexesBuffer->release();
				pVerticesBuffer = NULL;
			}
		}

		inline VertexBuffer* BaseMesh::getVerticesBuffer() const
		{
			return pVerticesBuffer;
		}

		inline IndexBuffer* BaseMesh::getIndexesBuffer() const
		{
			return pIndexesBuffer;
		}

		inline uint32 BaseMesh::getCountIndexes() const
		{
			return pIndexesBuffer->getSize() / pIndexesBuffer->getStride();
		}

		inline uint32 BaseMesh::getCountVertices() const
		{
			return pVerticesBuffer->getSize() / pVerticesBuffer->getStride();
		}

		inline uint32 BaseMesh::getCountPrimitives() const
		{
			if(pIndexesBuffer->isNULL())
			{
				return getCountVertices() / Format::getCountVertices(primitiveTopology);
			}
			else
			{
				return pIndexesBuffer->getSize() / ( 3 * pIndexesBuffer->getStride());
			}
		}

		inline IntrusivePtr<Material> BaseMesh::getMaterial() const
		{
			return pMaterial;
		}

		inline BUFFER_FORMAT BaseMesh::getVertexFormat() const
		{
			return pVerticesBuffer->getFormat();
		}

		inline PRIMITIVE_TOPOLOGY BaseMesh::getPrimitiveTopology() const
		{
			return primitiveTopology;
		}

		inline void BaseMesh::shallowCopy(const ResourceItem& rItem)
		{
			BaseMesh& rMesh = (BaseMesh&) rItem;

			this->pCountReferences = rMesh.pCountReferences;
			this->pIndexesBuffer = rMesh.pIndexesBuffer;
			this->pVerticesBuffer = rMesh.pVerticesBuffer;
			this->pMaterial = rMesh.pMaterial;
		}
	}
}

#endif