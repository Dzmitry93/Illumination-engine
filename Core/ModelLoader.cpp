#include "ModelLoader.h"

namespace OGE
{
	namespace fileSystem
	{
		ISceneLoaderAdapter* ModelLoader::createAdapter(FILE_FORMAT extension)
		{
			ISceneLoaderAdapter* pAdapter = NULL;

			switch(extension)
			{
				//case FILE_FORMAT::FILE_FORMAT_WAVEFRONT : loadMethod = new OBJLoadMethod();
				case FILE_FORMAT::FILE_FORMAT_COLLADA : pAdapter = new Max13COLLADAAdapter();
			}

			debug_assert(pAdapter != NULL, "not fount specified loader adapter");
			return pAdapter;
		}

		void ModelLoader::loadScene(const string& path, 
				FILE_FORMAT extension, List<OGEMaterialData>& materials,
				List<OGEModelData*>& models, OGESceneData& sceneData)
		{
			Scene* scene = NULL;
			ISceneLoaderAdapter* pAdapter = createAdapter(extension);

			if(pAdapter != NULL)
			{
				pAdapter->loadScene(path, materials, models, sceneData);
				delete pAdapter;
			}
		}
	}
}