#ifndef __MODEL_LOADER_H__
#define __MODEL_LOADER_H__

/*
==================================

	General model data loader

==================================
*/

#include "ImportStructures.h"
#include "OBJLoadMethod.h"
#include "COLLADAReader.h"
#include "3DSMaxCOLLADAAdapter.h"
#include "ISceneLoaderAdapter.h"
#include "Scene.h"
#include <string>

using std::string;
using OGE::scene::Scene;
using namespace OGE::resource;

namespace OGE
{
	namespace fileSystem
	{
		enum FILE_FORMAT
		{
		//	FILE_FORMAT_WAVEFRONT,
			FILE_FORMAT_COLLADA
		};

		class ModelLoader
		{
		public:

			// load scene of specified format
			void loadScene(const string& path, 
				FILE_FORMAT extension, List<OGEMaterialData>& materials,
				List<OGEModelData*>& models, OGESceneData& sceneData);

		private:

			// creates loader adapter for specified file extension
			ISceneLoaderAdapter* createAdapter(FILE_FORMAT extension);
		};
	}
}

#endif

