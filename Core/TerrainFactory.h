#ifndef __TERRAIN_CHUNK_FACTORY_H__
#define __TERRAIN_CHUNK_FACTORY_H__

#include "TerrainQuadtree.h"
#include "MeshFactory.h"

using OGE::resource::MeshFactory;

namespace OGE
{
	namespace terrain
	{
		class TerrainFactory
		{
		public:

			TerrainQuadtree* createTerrain(const Texture2D& rHeightmap, float scaleFactor, uint32 minLOD = 0);
																						  
		private:

			SceneObject<IntrusivePtr<BaseMesh>> createTerrainChunk(
				uint32 LODLevel,
				pair<uint32, uint32> top,
				pair<uint32, uint32> bottom,	
				const Texture2D& rHeightmap);

			void createNormal(
				const V3TNT2Vertex& v1,
				const V3TNT2Vertex& v2,
				const V3TNT2Vertex& v3,
				Vector& rNormal);

			void calculateTangentSpaceVectors(
				const V3TNT2Vertex& v1,
				const V3TNT2Vertex& v2,
				const V3TNT2Vertex& v3,
				Vector& rTangent,
				Vector& rBinormal);

			void createLODLevel(
				pair<uint32, uint32> top,
				pair<uint32, uint32> bottom,
				const Texture2D& rHeightmap,
				List<TerrainQuadreeNode>& rPath,
				TerrainQuadtree* pQuadtree,
				uint32 LODLevel);
		};
	}
}

#endif
