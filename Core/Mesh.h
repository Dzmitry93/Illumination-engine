#ifndef __MESH_H__
#define __MESH_H__

/*
==============================================

	This class is container of mesh data

==============================================
*/

#include "BaseMesh.h"

namespace OGE
{
	namespace resource
	{
		class Mesh : public BaseMesh
		{
		public:
			Mesh(VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer,
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial);

			Mesh(const Mesh& mesh);
			virtual ~Mesh();

			void operator = (const Mesh& rhs);

		private:

			void destroyObject();
			void shallowCopy(const ResourceItem& rhs);
		};

		inline Mesh::Mesh(VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial) :

				BaseMesh(pVerticesBuffer, pIndexesBuffer,
					primitiveTopology, pMaterial) {}

		inline Mesh::Mesh(const Mesh& rhs) :
			BaseMesh(rhs) 
		{
			shallowCopy(rhs);
			addRef();
		}

		inline Mesh::~Mesh() { release(); }

		inline void Mesh::operator = (const Mesh& rMesh)
		{
			shallowCopy(rMesh);
			addRef();
		}

		inline void Mesh::destroyObject() { BaseMesh::destroyObject(); }

		inline void Mesh::shallowCopy(const ResourceItem& rMesh)
		{
			BaseMesh::shallowCopy(rMesh);
		}
	}
}

#endif