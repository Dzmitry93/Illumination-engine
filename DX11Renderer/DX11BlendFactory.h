#ifndef __DX11_BLEND_FACTORY_H__
#define __DX11_BLEND_FACTORY_H__

#include "Debug.h"
#include "TypeDefinitions.h"

#include <d3d11.h>

namespace OGE
{
	namespace render
	{
		struct BlendState
		{
			ID3D11BlendState* pBlendState;
			FLOAT* blendFactor;
			UINT SampleMask;
		};

		class DX11BlendFactory
		{
		public:

			static BlendState createLightingBlendState(ID3D11Device* pDevice);
		};

	}
}

#endif