#ifndef __MESH_FACTORY_H__
#define __MESH_FACTORY_H__

#include "Mesh.h"
#include "IterableMesh.h"
#include "BufferFactory.h"
#include "HashUtil.h"
#include "GUIDUtil.h"
#include "ImportStructures.h"
#include <map>
#include <vector>

using namespace OGE::fileSystem;
using OGE::util::HashUtil;
using OGE::util::GUIDUtil;
using OGE::memory::IntrusivePtr;
using std::vector;
using std::map;

namespace OGE
{
	namespace resource
	{
		class MeshFactory
		{
		public:

			static Mesh* createMesh(
				VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, 
				const IntrusivePtr<Material>& pMaterial, 
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);
			
			static Mesh* createMesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);

			static Mesh* createMesh(OGEModelData* pModelData, IntrusivePtr<Material>& pMaterial,
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);

			static IterableMesh* createIterableMesh(
				VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer, 
				const IntrusivePtr<Material>& pMaterial,
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);

			static IterableMesh* createIterableMesh(const IntrusivePtr<BaseMesh>& pSrcMesh);
			
		};
	}
}

#endif
