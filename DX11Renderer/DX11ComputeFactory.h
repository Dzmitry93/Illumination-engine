#ifndef __COMPUTE_FACTORY_H__
#define __COMPUTE_FACTORY_H__

#include "DX11ComputeResource.h"
#include <fstream>

using std::ifstream;

namespace OGE
{
	namespace render
	{
		class DX11ComputeFactory
		{
		public:

			static DX11ComputeResource* createComputeResourceFormFile(ID3D11Device* pDevice, const string& filePath);

		};
	}
}

#endif

