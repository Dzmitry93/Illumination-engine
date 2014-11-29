#ifndef __DX11_MESH_FACTORY_H__
#define __DX11_MESH_FACTORY_H__

#include "DX11RenderableMesh.h"
#include "DX11BufferFactory.h"
#include "Mesh.h"
#include "HashUtil.h"
#include "GUIDUtil.h"

using OGE::resource::Mesh;
using OGE::resource::TRIANGLE_LIST;

using namespace OGE::util;

namespace OGE
{
	namespace render
	{
		class DX11MeshFactory
		{
		public:
			
			static DX11RenderableMesh* createRenderableMesh(
				ID3D11Device* pDevice,
				const IntrusivePtr<BaseMesh>& pSrcSystemMesh,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
			
			static DX11RenderableMesh* createRenderableMesh(
				ID3D11Device* pDevice,
				DX11ResourceBuffer* pVertices, DX11ResourceBuffer* pIndexes,
				const IntrusivePtr<Material>& pMaterial, 
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);

			static DX11RenderableMesh* createRenderableMesh(
				ID3D11Device* pDevice,
				DX11ResourceBuffer* pVertices, DX11ResourceBuffer* pIndexes,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
				PRIMITIVE_TOPOLOGY primitiveTopology = TRIANGLE_LIST);
		};
	}
}

#endif

