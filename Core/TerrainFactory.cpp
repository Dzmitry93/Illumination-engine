#include "TerrainFactory.h"

namespace OGE
{
	namespace terrain
	{
		TerrainQuadtree* TerrainFactory::createTerrain(
			const Texture2D& rHeightmap,
			float scaleFactor,
			uint32 minLOD)
		{   
			uint32 width = rHeightmap.getWidth();
			uint32 height = rHeightmap.getHeight();

			// calculate z range of aabb

			float minZ = FLT_MAX;
			float maxZ = -FLT_MAX;

			for(uint32 i = 0; i < height; i++)
			{
				for(uint32 j = 0; j < width; j++)
				{
					float z = rHeightmap[i * height + j];

					if(z > maxZ) { maxZ = z; }
					if(z < minZ) { minZ = z; }
				}
			}

			AABB rootAABB(
				Vector(0.0f, 0.0f, 0.0f),
				Vector(
					width * scaleFactor / 2.0f, 
					height * scaleFactor / 2.0f,
					(maxZ - minZ) * scaleFactor / 2.0f));
			
			if(minLOD == 0)
			{
				minLOD = 1;

				uint32 tempWidth = width;
				uint32 tempHeight = height;

				while(tempHeight % 2 == 0 && tempWidth % 2 == 0)
				{
					tempHeight /= 2;
					tempWidth /= 2;

					minLOD++;
				}
			}

			TerrainQuadtree* pQuadtree = new TerrainQuadtree(rootAABB, minLOD);
			List<TerrainQuadreeNode> path;

			createLODLevel(
				pair<uint32, uint32>(0, 0),
				pair<uint32, uint32>(width, height),
				rHeightmap,
				path,
				pQuadtree, 
				minLOD);

			return pQuadtree;
		}

		void TerrainFactory::createLODLevel(
				pair<uint32, uint32> top,
				pair<uint32, uint32> bottom,
				const Texture2D& rHeightmap,
				List<TerrainQuadreeNode>& rPath,
				TerrainQuadtree* pQuadtree,
				uint32 LODLevel)
		{

			if(LODLevel > 0)
			{
				// top left chunk

				pair<uint32, uint32> chunkTop = top;
				pair<uint32, uint32> chunkBottom = pair<uint32, uint32>(top.first + bottom.first / 2, top.second + bottom.second / 2);

				SceneObject<IntrusivePtr<BaseMesh>> topLeftChunk = createTerrainChunk(
					LODLevel,
					chunkTop,
					chunkBottom,
					rHeightmap);

				rPath.push_back(TerrainQuadreeNode::NODE_TOP_LEFT);
				pQuadtree->insert(topLeftChunk, rPath);

				createLODLevel(
					chunkTop,
					chunkBottom,
					rHeightmap,
					rPath,
					pQuadtree,
					LODLevel - 1);

				// bottom left chunk

				chunkTop = 	pair<uint32, uint32>(top.first, top.second + bottom.second / 2);
				chunkBottom	 = pair<uint32, uint32>(top.first + bottom.first / 2, bottom.second);

				SceneObject<IntrusivePtr<BaseMesh>> bottomLeftChunk = createTerrainChunk(
					LODLevel,
					chunkTop,
					chunkBottom,
					rHeightmap);

				rPath.pop_back();
				rPath.push_back(TerrainQuadreeNode::NODE_BOTTOM_LEFT);
				pQuadtree->insert(bottomLeftChunk, rPath);

				createLODLevel(
					chunkTop,
					chunkBottom,
					rHeightmap,
					rPath,
					pQuadtree,
					LODLevel - 1);

				// bottom right chunk

				chunkTop = pair<uint32, uint32>(top.first + bottom.first / 2, top.second + bottom.second / 2);
				chunkBottom	 = bottom;

				SceneObject<IntrusivePtr<BaseMesh>> bottomRightChunk = createTerrainChunk(
					LODLevel,
					chunkTop,
					chunkBottom,
					rHeightmap);

				rPath.pop_back();
				rPath.push_back(TerrainQuadreeNode::NODE_BOTTOM_RIGHT);
				pQuadtree->insert(bottomRightChunk, rPath);

				createLODLevel(
					chunkTop,
					chunkBottom,
					rHeightmap,
					rPath,
					pQuadtree,
					LODLevel - 1);

				// top right chunk

				chunkTop = pair<uint32, uint32>(top.first + bottom.first / 2, top.second);
				chunkBottom = pair<uint32, uint32>(bottom.first, top.second + bottom.second / 2.0f);

				SceneObject<IntrusivePtr<BaseMesh>> topRightChunk = createTerrainChunk(
					LODLevel,
					chunkTop,
					chunkBottom,
					rHeightmap);

				rPath.pop_back();
				rPath.push_back(TerrainQuadreeNode::NODE_TOP_RIGHT);
				pQuadtree->insert(topRightChunk, rPath);
			
				createLODLevel(
					chunkTop,
					chunkBottom,
					rHeightmap,
					rPath,
					pQuadtree,
					LODLevel - 1);
			}
		}

		SceneObject<IntrusivePtr<BaseMesh>> TerrainFactory::createTerrainChunk(
				uint32 LODLevel,
				pair<uint32, uint32> top,
				pair<uint32, uint32> bottom,
				const Texture2D& rHeightmap)
		{
			const Buffer* pTextureData = rHeightmap.getTextureData();
			const void* pData  = pTextureData->getPointer();
			
			uint32 width = bottom.first - top.first;
			uint32 height = bottom.second - top.second;	   

			uint32 textureWidth = rHeightmap.getWidth();
			uint32 textureHeight = rHeightmap.getHeight();

			V3TNT2Vertex v1;
			V3TNT2Vertex v2;
			V3TNT2Vertex v3;

			Vector normal;
			Vector normal2;
			Vector tangent;
			Vector tangent2;
			Vector binormal;
			Vector binormal2;

			Vector dir;
			Vector dir2;

			debug_assert(width > 1 && height > 1, "incorreñt width and height");

			uint32 countIndices = width	* height * 6;
			uint32 countVertices = (width + 1) * (height + 1);

			V3TNT2Vertex* pVertices = new V3TNT2Vertex[countVertices];
			uint32* pIndexes = new uint32[countIndices];

			uint32 vertexIndex = 0;

			// create vertices 
			for(uint32 i = top.second; i < bottom.second; i += LODLevel)
			{
				for(uint32 j = top.first; j < bottom.first; j += LODLevel)
				{
					uint32 indexTL = i * textureWidth + j;

					uint32 z = rHeightmap[indexTL];	

					v1.x = (float) j - width / 2.0f;
					v1.y = (float) i - height / 2.0f;
					v1.z = (float) z;

					v1.tx = (float) (j % 2);
					v1.ty = (float) (i % 2);

					pVertices[vertexIndex] = v1;
					vertexIndex++;
				}
			}

			//generating tangent space vectors
			for(uint32 i = 0; i <= height; i++)
			{
				for(uint32 j = 0; j <= width; j++)
				{
					uint32 vertexIndex = i * width + j;
					V3TNT2Vertex& vertex = pVertices[vertexIndex];
					uint32 countTriangles = 0;

					if((i - 1) >= 0 && (j - 1) >= 0)
					{
						v1 = pVertices[(i - 1) * width + j - 1];
						v2 = pVertices[i * width + j - 1];
						v3 = pVertices[(i - 1) * width + j];

						createNormal(v1, v2, vertex, normal);
						createNormal(vertex, v3, v1, normal2);

						calculateTangentSpaceVectors(vertex, v2, v1, tangent, binormal);
						calculateTangentSpaceVectors(vertex, v1, v3, tangent2, binormal2);

						vertex.nx += normal.x + normal2.x;
						vertex.ny += normal.y + normal2.y;
						vertex.nz += normal.z + normal2.z;
						
						vertex.tnx += tangent.x + tangent2.x;
						vertex.tny += tangent.y + tangent2.y;
						vertex.tnz += tangent.z + tangent2.z;
						
						vertex.bnx += binormal.x + binormal2.x;
						vertex.bny += binormal.y + binormal2.y;
						vertex.bnz += binormal.z + binormal2.z;
						
						countTriangles += 2;
					}

					if((i + 1) <= height && (j - 1) >= 0)
					{
						v1 = pVertices[i * width + j - 1];
						v2 = pVertices[(i + 1) * width + j - 1];
						v3 = pVertices[(i + 1) * width + j];

						createNormal(v1, v2, v3, normal);
						createNormal(v3, vertex, v1, normal2);

						calculateTangentSpaceVectors(vertex, v1, v2, tangent, binormal);
						calculateTangentSpaceVectors(vertex, v2, v3, tangent2, binormal2);

						vertex.nx += normal.x + normal2.x;
						vertex.ny += normal.y + normal2.y;
						vertex.nz += normal.z + normal2.z;

						vertex.tnx += tangent.x + tangent2.x;
						vertex.tny += tangent.y + tangent2.y;
						vertex.tnz += tangent.z + tangent2.z;
						
						vertex.bnx += binormal.x + binormal2.x;
						vertex.bny += binormal.y + binormal2.y;
						vertex.bnz += binormal.z + binormal2.z;

						countTriangles += 2;
					}

					if((i + 1) <= height && (j + 1) <= width)
					{
						v1 = pVertices[(i + 1) * width + j];
						v2 = pVertices[(i + 1) * width + j + 1];
						v3 = pVertices[i * width + j + 1];

						createNormal(vertex, v1, v2, normal);
						createNormal(v2, v3, vertex, normal2);

						calculateTangentSpaceVectors(vertex, v1, v2, tangent, binormal);
						calculateTangentSpaceVectors(vertex, v2, v3, tangent2, binormal2);

						vertex.nx += normal.x + normal2.x;
						vertex.ny += normal.y + normal2.y;
						vertex.nz += normal.z + normal2.z;

						vertex.tnx += tangent.x + tangent2.x;
						vertex.tny += tangent.y + tangent2.y;
						vertex.tnz += tangent.z + tangent2.z;
						
						vertex.bnx += binormal.x + binormal2.x;
						vertex.bny += binormal.y + binormal2.y;
						vertex.bnz += binormal.z + binormal2.z;

						countTriangles += 2;
					}

					if((i - 1) >= 0 && (j + 1) <= width)
					{
						v1 = pVertices[(i - 1) * width + j];
						v2 = pVertices[i * width + j + 1];
						v3 = pVertices[(i - 1) * width + j + 1];

						createNormal(v1, vertex, v2, normal);
						createNormal(v2, v3, v1, normal2);

						calculateTangentSpaceVectors(vertex, v1, v3, tangent, binormal);
						calculateTangentSpaceVectors(vertex, v3, v2, tangent2, binormal2);

						vertex.nx += normal.x + normal2.x;
						vertex.ny += normal.y + normal2.y;
						vertex.nz += normal.z + normal2.z;

						vertex.tnx += tangent.x + tangent2.x;
						vertex.tny += tangent.y + tangent2.y;
						vertex.tnz += tangent.z + tangent2.z;
						
						vertex.bnx += binormal.x + binormal2.x;
						vertex.bny += binormal.y + binormal2.y;
						vertex.bnz += binormal.z + binormal2.z;
					}

					vertex.nx /= countTriangles;
					vertex.ny /= countTriangles;
					vertex.nz /= countTriangles;

					vertex.tnx /= countTriangles;
					vertex.tny /= countTriangles;
					vertex.tnz /= countTriangles;

					vertex.bnx /= countTriangles;
					vertex.bny /= countTriangles;
					vertex.bnz /= countTriangles;
				}
			}

			// generating indices 

			uint32 indicesBufferIndex = 0;

			for(uint32 i = 0; i < height; i++)
			{
				for(uint32 j = 0; j < width; j++)
				{
					// first triangle
					pIndexes[indicesBufferIndex++] = i * width + j;
					pIndexes[indicesBufferIndex++] = (i + 1) * width + j + 1;
					pIndexes[indicesBufferIndex++] = i * width + j + 1;
					
					// second triangle 
					pIndexes[indicesBufferIndex++] = i * width + j;
					pIndexes[indicesBufferIndex++] = (i + 1) * width + j;
					pIndexes[indicesBufferIndex++] = (i	+ 1) * width + j + 1;
				}
			}

			uint32 vertexSize = Format::getSize(BUFFER_FORMAT::VERTEX_FORMAT_V3TNT2);
			uint32 indexSize = sizeof(uint32);

			VertexBuffer* pVerticesBuffer = 
				new VertexBuffer(pVertices, vertexSize * countVertices, 
				vertexSize, BUFFER_FORMAT::VERTEX_FORMAT_V3TNT2);
			
			IndexBuffer* pIndexesBuffer = 
				new IndexBuffer(pIndexes, indexSize * countIndices, 
				indexSize, BUFFER_FORMAT::INDEX_32);

			BaseMesh* pMesh =  
				MeshFactory::createMesh(pVerticesBuffer, pIndexesBuffer, IntrusivePtr<Material>());

			SceneObject<IntrusivePtr<BaseMesh>> sceneObject;
			sceneObject.setSceneObject(pMesh);

			safe_array_release(pVertices);
			safe_array_release(pIndexes);

			safe_release(pVerticesBuffer);
			safe_release(pIndexesBuffer);


			return sceneObject;
		}

		void TerrainFactory::createNormal(
				const V3TNT2Vertex& v1,
				const V3TNT2Vertex& v2,
				const V3TNT2Vertex& v3,
				Vector& rNormal)
		{
			Vector dir;
			Vector dir2;

			dir.x = v2.x - v1.x;
			dir.y = v2.y - v1.y;
			dir.z = v2.z - v1.z;

			dir2.x = v3.x - v2.x;
			dir2.y = v3.y - v2.y;
			dir2.z = v3.z - v2.z;

			rNormal = Vector::CrossProduct(dir, dir2);
		}

		void TerrainFactory::calculateTangentSpaceVectors(
				const V3TNT2Vertex& rV1,
				const V3TNT2Vertex& rV2,
				const V3TNT2Vertex& rV3,
				Vector& rTangent,
				Vector& rBinormal)
		{
			 Vector v0(rV1.x, rV1.y, rV1.z); 
			 Vector v1(rV2.x, rV2.y, rV2.z);
		     Vector v2(rV3.x, rV3.y, rV3.z);

			 CalculateTangentSpace(v0, v1, v2, 
					rV2.tx - rV1.tx, 
					rV2.ty - rV1.ty,  
					rV3.tx - rV1.tx, 
					rV3.ty - rV1.ty,
					rTangent, rBinormal);
		}
	}
}

