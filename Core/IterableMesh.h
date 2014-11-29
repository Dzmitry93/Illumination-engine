#ifndef __ITERABLE_MESH_H__
#define __ITERABLE_MESH_H__

#include "ArrayIterator.h"
#include "ConstArrayIterator.h"
#include "Matrix.h"
#include "BaseMesh.h"

using OGE::math::Matrix;
using OGE::math::Vector;

namespace OGE
{
	namespace resource
	{
		class IterableMesh : public BaseMesh
		{
		public:
			IterableMesh(VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer, 
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial);
			IterableMesh(const IterableMesh& mesh);
			virtual ~IterableMesh();

			void operator = (const IterableMesh& rhs);

			ArrayIterator<V3TNT2Triangle> iterator();
			ConstArrayIterator<V3TNT2Triangle> citerator() const;

			void createIterableData(const Matrix& rWorldMatrix);
			bool isIterable() const;

		private:
			V3TNT2Triangle* pIterableData;

			void destroyObject();
			void shallowCopy(const ResourceItem& rhs);
		};

		inline IterableMesh::IterableMesh(
				VertexBuffer* pVerticesBuffer, IndexBuffer* pIndexesBuffer,
				PRIMITIVE_TOPOLOGY primitiveTopology,
				const IntrusivePtr<Material>& pMaterial)  :
					BaseMesh(pVerticesBuffer,
						pIndexesBuffer, 
						primitiveTopology,
						pMaterial), 
						pIterableData(NULL) {}

		inline IterableMesh::IterableMesh(const IterableMesh& rMesh)
			:
					BaseMesh(rMesh.pVerticesBuffer,
						rMesh.pIndexesBuffer, 
						rMesh.primitiveTopology,
						rMesh.pMaterial), 
						pIterableData(rMesh.pIterableData)
		{
			shallowCopy(rMesh);
			addRef();
		}

		inline IterableMesh::~IterableMesh() { release(); }

		inline void IterableMesh::destroyObject()
		{
			BaseMesh::destroyObject();
			safe_array_release(pIterableData);
		}

		inline void IterableMesh::shallowCopy(const ResourceItem& rMesh)
		{
			BaseMesh::shallowCopy(rMesh);
		}

		inline void IterableMesh::operator= (const IterableMesh& rMesh)
		{
			shallowCopy(rMesh);
			addRef();
		}

		inline ArrayIterator<V3TNT2Triangle> IterableMesh::iterator()
		{
			return ArrayIterator<V3TNT2Triangle>(pIterableData, getCountPrimitives());
		}

		inline ConstArrayIterator<V3TNT2Triangle> IterableMesh::citerator() const
		{
			return ConstArrayIterator<V3TNT2Triangle>(pIterableData, getCountPrimitives());
		}

		inline bool IterableMesh::isIterable() const
		{
			return pIterableData != NULL;
		}
	}
}

#endif
