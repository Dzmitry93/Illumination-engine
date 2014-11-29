#ifndef __COLLADALOADER_H__
#define __COLLADALOADER_H__

#include "COLLADAStructures.h"
#include "pugixml.hpp"
#include "BufferFactory.h"
#include "List.h"
#include <vector>
#include <string>
#include <sstream>

using OGE::Collections::List;
using OGE::Collections::ListIterator;
using namespace OGE::resource;
using namespace OGE;
using std::istringstream;
using std::vector;

namespace OGE
{
	namespace fileSystem
	{
		class COLLADAReader
		{
		public:
			
			COLLADADocument* loadScene(const string& path);

		private:

			// default values
			static string defaultString;
			static uint32 defaultUint;

			// input semantics
			static string vertexSemantic;
			static string normalSemantic;
			static string texcoordSemantic;

			// parse libraries
			CLibraryGeometries* loadLibraryGeometry(pugi::xml_node& rLibraryNode);
			CLibraryMaterial* loadLibraryMaterial(pugi::xml_node& rLibraryNode);
			CLibraryEffects* loadLibraryEffects(pugi::xml_node& rLibraryNode);
			CLibraryVisualScenes* loadLibraryVisualScenes(pugi::xml_node& rLibraryNode);
			CLibraryImages* loadLibraryImages(pugi::xml_node& rLibraryNode);
			CLibraryLights* loadLibraryLights(pugi::xml_node& rLibraryNode);
			CLibraryCameras* loadLibraryCameras(pugi::xml_node& rLibraryNode);

			// parse COLLADA tokens
			void parseSources(pugi::xml_node& rMeshNode, CSource** ppSource, uint32& rCount);
			void parseAccessor(pugi::xml_node& rAccessorNode, CAccessor& rAccessor);
			void parseArray(pugi::xml_node& arrayNode, CFloatArray& floatArray);
			void parseVertices(pugi::xml_node& meshNode, CVertices& vertices);
			void parsePolygons(pugi::xml_node& meshNode, CPolygons** ppPolygons, uint32& count);
			void parseNode(pugi::xml_node& node, CNode* pNodeObj);
			void parseBindMaterial(pugi::xml_node& bintMaterialNode, CBindMaterial& bindMaterial);

			void parseInstanceGeometry(pugi::xml_node& instGeomNode, CInstanceGeometry& instGeom);
			void parseInstanceCamera(pugi::xml_node& rInstanceCameraNode, CInstanceCamera& rInstanceCamera);
			void parseInstanceLight(pugi::xml_node& rInstanceLightNode, CInstanceLight& rInstanceLight);
			void parseInstanceMaterial(pugi::xml_node& instMatNode, CInstanceMaterial& instMat);

			void parseColor(pugi::xml_node& colorNode, CColor& color);
			void parseFloat(pugi::xml_node& floatNode, CFloat& value);
			void parseFloatArray(string& data, float* pFloatArray);
			void parseParam(pugi::xml_node& paramNode, CParam& param);

			void parseRotate(pugi::xml_node& rRotateNode, CRotate& rRotate);
			void parseTranslate(pugi::xml_node& rTranslateNode, CTranslate& rTranslate);
			void parseScale(pugi::xml_node& rScaleNode, CScale& rScale);
			void parseMatrix(pugi::xml_node& matrixNode, CMatrix& matrix);

			void parseInput(pugi::xml_node& inputNode, CInput& input);
			void parseEffectMaterial(pugi::xml_node& phongNode, CEffectMaterial& phong);
			void parseMaterial(pugi::xml_node& materialNode, CMaterial& material);
			void parseImage(pugi::xml_node& imageNode, CImage& image);
			void parseTexture(pugi::xml_node& textureNode, CTexture& texture);
			void parseMaterialColor(pugi::xml_node& materialColorNode,
				CMaterialColorNode& matColor);
			void parseMaterialDegree(pugi::xml_node& materialDegreeNode,
				CMaterialDegreeNode& matDegree);

			// parse light nodes
			void parseDirectionalLight(pugi::xml_node& lightNode, CDirectionalLight& rDirLight);
			void parsePointLight(pugi::xml_node& lightNode, CPointLight& rPointLight);
			void parseSpotlight(pugi::xml_node& lightNode, CSpotlight& rSpotlight);
			void parseLightAttenuation(pugi::xml_node& constantAttenuationNode, 
										pugi::xml_node& linearAttenuationNode,
										pugi::xml_node& quadraticAttenuationNode, 
										CLightAttenuation& rAttenuation);

			void parseOptics(pugi::xml_node& rOpticsNode, COptics& rOptics);
			void parsePerspectiveProjection(pugi::xml_node& rProjNode, CPerspective& rProj);
			// parse data from list
			template<class T>
			void parseListData(List<T>& lst, T** ppData, uint32& count)
			{
				uint32 index = 0;
				ListIterator<T> it = lst.iterator();
				count = lst.size();
				T* pData = NULL;

				if(count > 0)
				{
					pData = new T[count];
			
					while(it.hasNext())
					{
						pData[index++] = it.next();
					}
				}

				*ppData = pData;
			}
		};
	}
}

#endif