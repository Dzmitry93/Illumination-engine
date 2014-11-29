#include "MeshUtil.h"

namespace OGE
{
	namespace util
	{

		BaseMesh* MeshUtil::createScreenQuad(float zNear)
		{
			V4T2Vertex screenQuad[6] = 
			{
				{-1.0f, 1.0f, zNear, 1.0f, 0.0f, 0.0f},
				{1.0f, 1.0f, zNear, 1.0f, 1.0f, 0.0f},
				{1.0f, -1.0f, zNear, 1.0f, 1.0f, 1.0f},

				{-1.0f, 1.0f, zNear, 1.0f, 0.0f, 0.0f},
				{1.0f, -1.0f, zNear, 1.0f, 1.0f, 1.0f},
				{-1.0f, -1.0f, zNear, 1.0f, 0.0f, 1.0f},
			};

			BaseMesh* pScreenQuadMesh = NULL;
			IndexBuffer* pIndexesBuffer = new IndexBuffer(0);
			VertexBuffer* pVerticesBuffer = BufferFactory::createVertexBuffer(VERTEX_FORMAT_V4T2, screenQuad, 
																Format::getSize(VERTEX_FORMAT_V4T2) * 6, 
																Format::getSize(VERTEX_FORMAT_V4T2));
			
			pScreenQuadMesh = MeshFactory::createMesh(pVerticesBuffer, pIndexesBuffer);
			return pScreenQuadMesh;
		}

		BaseMesh* MeshUtil::createFontVB(const IntrusivePtr<FontLibrary>& pFontLib, const string& msg, const Viewport& viewport)
		{
			uint32 msgLength = msg.size();
			BaseMesh* pVB = NULL; 
				
			if(msgLength > 0 && msgLength < ( 1 << 10 ))
			{
				V3T2Vertex* pFontVB = new V3T2Vertex[msgLength * 6];
				
				float zNear = viewport.minDepth;

				float accruedShift = 0;
				float currentX = 0;
				float currentY = 0;
				uint32 index = 0;

				for(uint32 i = 0; i < msgLength; i++)
				{
					FontChar fontChar = pFontLib->getChar(msg.at(i));

					V3T2Vertex v1;
					V3T2Vertex v2;
					V3T2Vertex v3;
					V3T2Vertex v4;

					//  top left
					v1.x = currentX;  
					v1.y = currentY;
					v1.z = 0.0f;
					v1.tx = fontChar.leftTextureCoord;
					v1.ty = 0.0f;

					// bottom left
					v2.x = currentX;
					v2.y = currentY - 16.0f;
					v2.z = 0.0f;
					v2.tx = fontChar.leftTextureCoord;
					v2.ty = 1.0f;

					// bottom right
					v3.x = currentX + fontChar.size;
					v3.y = currentY - 16.0f;
					v3.z = 0.0f;
					v3.tx = fontChar.rightTextureCoord;
					v3.ty = 1.0f;

					//top right
					v4.x = currentX + fontChar.size;
					v4.y = currentY;
					v4.z = 0.0f;
					v4.tx = fontChar.rightTextureCoord;
					v4.ty = 0.0f;

					pFontVB[index++] = v1;
					pFontVB[index++] = v3;
					pFontVB[index++] = v2;

					pFontVB[index++] = v1;
					pFontVB[index++] = v4;
					pFontVB[index++] = v3;

					accruedShift += fontChar.size + 1;
					currentX = accruedShift;
				}

				IndexBuffer* pIndexesBuffer = NULL;
				VertexBuffer* pVerticesBuffer = BufferFactory::createVertexBuffer(VERTEX_FORMAT_V3T2, pFontVB, 
																Format::getSize(VERTEX_FORMAT_V3T2) * 6 * msgLength,
																Format::getSize(VERTEX_FORMAT_V3T2));
			
				pVB = MeshFactory::createMesh(pVerticesBuffer, pIndexesBuffer);
			}

			return pVB;
		}

		BaseMesh* MeshUtil::createOBBMesh(const OBB& rOBB) 
		{
			Vector point = rOBB.middlePoint;

			float x = (rOBB.planes[0].dir * rOBB.planes[0].halfDistance).x;
			float y = (rOBB.planes[1].dir * rOBB.planes[1].halfDistance).y;
			float z = (rOBB.planes[2].dir * rOBB.planes[2].halfDistance).z;

			V4Vertex data[16] = 
			{
				{point.x + x, point.y + y, point.z + z, 1.0f},
				{point.x - x, point.y + y, point.z + z, 1.0f},
				{point.x - x, point.y - y, point.z + z, 1.0f},
				{point.x + x, point.y - y, point.z + z, 1.0f},
				{point.x + x, point.y + y, point.z + z, 1.0f},
				{point.x + x, point.y + y, point.z - z, 1.0f},
				{point.x + x, point.y - y, point.z - z, 1.0f},
				{point.x + x, point.y - y, point.z + z, 1.0f},
				{point.x + x, point.y - y, point.z - z, 1.0f},
				{point.x - x, point.y - y, point.z - z, 1.0f},
				{point.x - x, point.y - y, point.z + z, 1.0f},
				{point.x - x, point.y - y, point.z - z, 1.0f},
				{point.x - x, point.y + y, point.z - z, 1.0f},
				{point.x - x, point.y + y, point.z + z, 1.0f},
				{point.x - x, point.y + y, point.z - z, 1.0f},
				{point.x + x, point.y + y, point.z - z, 1.0f}
			};

			//IntrusivePtr<Material> pMaterial = MaterialFactory::createMaterial(OGEMaterialData(), true);

			// creates mesh
			IndexBuffer* pIndexesBuffer = new IndexBuffer(0);
			VertexBuffer* pVerticesBuffer = BufferFactory::createVertexBuffer(VERTEX_FORMAT_V4, &data, 
															Format::getSize(VERTEX_FORMAT_V4) * 16,
															Format::getSize(VERTEX_FORMAT_V4));
			
			return MeshFactory::createMesh(pVerticesBuffer, pIndexesBuffer, 
				IntrusivePtr<Material>(new Material()), PRIMITIVE_TOPOLOGY::LINE_STRIP);
		}

		BaseMesh* MeshUtil::createAABBMesh(const AABB& rAABB)
		{
			return createOBBMesh(OBB(rAABB));
		}

		BaseMesh* MeshUtil::createViewFrustumMesh(const ViewFrustum& rViewfrustum)
		{
			// near points
			Vector nPoint1;
			Vector nPoint2;
			Vector nPoint3;
			Vector nPoint4;

			// far points
			Vector fPoint1;
			Vector fPoint2;
			Vector fPoint3;
			Vector fPoint4;

			PlanesIntersection(rViewfrustum.planes[4], rViewfrustum.planes[3], rViewfrustum.planes[0], nPoint1);
			PlanesIntersection(rViewfrustum.planes[4], rViewfrustum.planes[3], rViewfrustum.planes[1], nPoint2);
			PlanesIntersection(rViewfrustum.planes[4], rViewfrustum.planes[2], rViewfrustum.planes[1], nPoint3);
			PlanesIntersection(rViewfrustum.planes[4], rViewfrustum.planes[2], rViewfrustum.planes[0], nPoint4);

			PlanesIntersection(rViewfrustum.planes[5], rViewfrustum.planes[3], rViewfrustum.planes[0], fPoint1);
			PlanesIntersection(rViewfrustum.planes[5], rViewfrustum.planes[3], rViewfrustum.planes[1], fPoint2);
			PlanesIntersection(rViewfrustum.planes[5], rViewfrustum.planes[2], rViewfrustum.planes[1], fPoint3);
			PlanesIntersection(rViewfrustum.planes[5], rViewfrustum.planes[2], rViewfrustum.planes[0], fPoint4);

			V4Vertex vertices[12] = 
			{
				// near plane
				{nPoint1.x, nPoint1.y, nPoint1.z, 1.0f},
				{nPoint4.x, nPoint4.y, nPoint4.z, 1.0f},
				{nPoint3.x, nPoint3.y, nPoint3.z, 1.0f},

				{nPoint1.x, nPoint1.y, nPoint1.z, 1.0f},
				{nPoint3.x, nPoint3.y, nPoint3.z, 1.0f},
				{nPoint2.x, nPoint2.y, nPoint2.z, 1.0f},

				// far plane
				{fPoint1.x, fPoint1.y, fPoint1.z, 1.0f},
				{fPoint4.x, fPoint4.y, fPoint4.z, 1.0f},
				{fPoint3.x, fPoint3.y, fPoint3.z, 1.0f},

				{fPoint1.x, fPoint1.y, fPoint1.z, 1.0f},
				{fPoint4.x, fPoint4.y, fPoint4.z, 1.0f},
				{fPoint3.x, fPoint3.y, fPoint3.z, 1.0f}	 
			};

			IndexBuffer* pIndexesBuffer = new IndexBuffer(0);
			VertexBuffer* pVerticesBuffer = BufferFactory::createVertexBuffer(VERTEX_FORMAT_V4, &vertices, 
															Format::getSize(VERTEX_FORMAT_V4) * 12,
															Format::getSize(VERTEX_FORMAT_V4));
			
			return MeshFactory::createMesh(pVerticesBuffer, pIndexesBuffer, 
				IntrusivePtr<Material>(new Material()), PRIMITIVE_TOPOLOGY::TRIANGLE_LIST);
		}
	}
}