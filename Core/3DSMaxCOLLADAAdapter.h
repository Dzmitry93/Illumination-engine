#ifndef __COLLADA_ADAPTER_H__
#define __COLLADA_ADAPTER_H__

#include "ISceneLoaderAdapter.h"
#include "COLLADAReader.h"
#include "MathUtils.h"

using namespace OGE::math;

namespace OGE
{
	namespace fileSystem
	{
		class Max13COLLADAAdapter : public ISceneLoaderAdapter
		{
		public:
			Max13COLLADAAdapter(void);
			~Max13COLLADAAdapter(void);

			void loadScene(const string& path, List<OGEMaterialData>& materials, 
				List<OGEModelData*>& models, OGESceneData& sceneData);

		private:
			COLLADAReader* pReader;
			
			struct TangentSum
			{
				Vector tangent;
				Vector binormal;
				uint32 countPrimitives;
			};

			void createMaterials(CLibraryEffects* pEffectsLibrary, CLibraryImages* pImagesLibrary, 
				CLibraryMaterial* pMaterialsLibrary, List<OGEMaterialData>& materials);

			void COLLADAColorToEngineColor(const CColor& colorNode, ARGBColor& rColor);
			void createGeometry(CLibraryGeometries* pLibraryGeometries, List<OGEModelData*>& models);
			void createLights(CLibraryLights* pLibraryLights, List<OGELightData*>& rLightData);
			void createCameras(CLibraryCameras* pLibraryCameras, List<OGECameraData*>& rCameraData);

			void createScene(CLibraryVisualScenes* pVisualSceeLibrary, OGESceneData* pSceneData,
												List<OGEModelData*>& modelsData, 
												List<OGELightData*>& rLightsData, 
												List<OGECameraData*>& rCameraData);
 
			void parseNodes(CNode* pNode, OGESceneNode* pSceneNode,
								List<OGEModelData*>& models, 
								List<OGELightData*>& rLightsData,
								List<OGECameraData*>& rCameraData);

			void combineVerticesData(const CSource& vertices, const CSource& normals, const CSource& texcoords,
				const CPolygons& polygons,
				vector<V3TNT2Vertex>& vertexData, 
				vector<uint32>& trianglesData);

			void releaseCOLLADAData(COLLADADocument* pColladaDocument);
			void releaseCOLLADASceneNodeData(CNode* pNode);
		};

		inline Max13COLLADAAdapter::Max13COLLADAAdapter(void)
		{
			pReader = new COLLADAReader();
		}

		inline Max13COLLADAAdapter::~Max13COLLADAAdapter(void)
		{
			safe_release(pReader);
		}
	}
}

#endif
