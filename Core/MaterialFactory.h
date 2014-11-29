#ifndef __MATERIAL_FACTORY_H__
#define __MATERIAL_FACTORY_H__

#include "Material.h"
#include "TextureFactory.h"
#include "ImportStructures.h"
#include "HashUtil.h"
#include "GUIDUtil.h"

using namespace OGE::fileSystem;
using namespace OGE::util;

namespace OGE
{
	namespace resource
	{
		class MaterialFactory
		{
		public:

			static Material* createMaterial(const OGEMaterialData& rMaterialData, bool generateName = false);

		private :
			static string material;
		};
	}
}

#endif

