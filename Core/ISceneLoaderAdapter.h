#pragma once
#ifndef __ISCENE_LOADER_ADAPTER_H__
#define __ISCENE_LOADER_ADAPTER_H__

#include "ImportStructures.h"
#include "List.h"
#include <string>

using std::string;
using OGE::Collections::List;

namespace OGE
{
	namespace fileSystem
	{
		class ISceneLoaderAdapter
		{
		public:

			virtual void loadScene(const string& path, List<OGEMaterialData>& materials, 
				List<OGEModelData*>& models, OGESceneData& sceneData) = 0;

		};
	}
}

#endif