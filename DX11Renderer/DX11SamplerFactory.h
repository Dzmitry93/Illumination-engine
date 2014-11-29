#ifndef __DX11SAMPLER_FACTORY_H__
#define __DX11SAMPLER_FACTORY_H__

#include "TypeDefinitions.h"
#include <float.h>
#include <D3D11.h>

namespace OGE
{
	namespace render
	{
		class DX11SamplerFactory
		{
		public:
			static ID3D11SamplerState* createDefaultSamplerState(ID3D11Device* pDevice, uint8 maxAnisotropy);
		};
	}
}

#endif