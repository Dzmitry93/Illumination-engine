#ifndef __COMPUTE_RESOURCE_H__
#define __COMPUTE_RESOURCE_H__

#include "DX11RenderResource.h"
#include <D3D11.h>

namespace OGE
{
	namespace render
	{
		class DX11ComputeResource : public DX11RenderResource
		{
		public:
			DX11ComputeResource(ID3D11ComputeShader* pCS);
			~DX11ComputeResource(void);

			ID3D11ComputeShader* getD3DComputeShader() const;
			void release();

		private:
			ID3D11ComputeShader* pCS;
		};

		inline DX11ComputeResource::DX11ComputeResource(ID3D11ComputeShader* pCS) : pCS(pCS) {}

		inline DX11ComputeResource::~DX11ComputeResource(void)
		{
			release();
		}

		inline ID3D11ComputeShader* DX11ComputeResource::getD3DComputeShader() const
		{
			return pCS;
		}

		inline void DX11ComputeResource::release()
		{
			safe_release(pCS);
		}
	}
}

#endif
