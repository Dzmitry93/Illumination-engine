#ifndef __TERRAIN_CHUNK_H__
#define __TERRAIN_CHUNK_H__

#include "BaseMesh.h"

using namespace OGE::resource;

namespace OGE
{
	namespace terrain
	{
		class Chunk
		{
		public:
			Chunk(uint32 topX, uint32 topY, uint32 bottomX, uint32 bottomY);
			~Chunk();

			uint32 topX;
			uint32 topY;
			uint32 bottomX;
			uint32 bottomY;

		};

		inline Chunk::Chunk(uint32 topX, uint32 topY, uint32 bottomX, uint32 bottomY)
			: topX(topX), topY(topY), bottomX(bottomX), bottomY(bottomY)
		{
		}

		class TerrainChunk : public BaseMesh
		{
		public:
			TerrainChunk(VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer,
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial);

			TerrainChunk(const TerrainChunk& mesh);
			virtual ~TerrainChunk();

			void operator = (const TerrainChunk& rhs);

		private:

			void destroyObject();
			void shallowCopy(const ResourceItem& rhs);
		};

		inline TerrainChunk::TerrainChunk(VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial) :

				BaseMesh(pVerticesBuffer, pIndexesBuffer,
					primitiveTopology, pMaterial) {}

		inline TerrainChunk::TerrainChunk(const TerrainChunk& rhs) :
			BaseMesh(rhs) 
		{
			shallowCopy(rhs);
			addRef();
		}

		inline TerrainChunk::~TerrainChunk() { release(); }

		inline void TerrainChunk::operator = (const TerrainChunk& rMesh)
		{
			shallowCopy(rMesh);
			addRef();
		}

		inline void TerrainChunk::destroyObject() { BaseMesh::destroyObject(); }

		inline void TerrainChunk::shallowCopy(const ResourceItem& rMesh)
		{
			BaseMesh::shallowCopy(rMesh);
		}
	}
}

#endif