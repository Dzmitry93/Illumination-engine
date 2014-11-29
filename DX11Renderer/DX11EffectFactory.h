#ifndef __EFFECT_FACTORY_H__
#define __EFFECT_FACTORY_H__

/*
======================================================

	This class creates directx effects from file 

======================================================
*/

#include "DX11EffectResource.h"
#include "DX11PipelineDescriptor.h"
#include "StringUtil.h"
#include <list>
#include <fstream>
#include <string>
#include <D3Dcompiler.h>

using std::list;
using std::string;
using std::ifstream;

using OGE::util::StringUtil;

#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)


namespace OGE
{
	namespace render
	{
		class DX11EffectFactory
		{
		public:

			DX11EffectResource* createEffectFromFile(ID3D11Device* pDevice, const string& path);
			DX11EffectResource* compileEffectFromFile(ID3D11Device* pDevice, 
				const string& path, const DX11PipelineDescriptor& rDecs);

		private:

			static const char* LightDefines[];
			static const char* GeneralDefines[];

			static const char* empty;
		};
	}
}

#endif

