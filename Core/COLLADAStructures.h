#ifndef __COLLADA_STRUCTURES_H__
#define __COLLADA_STRUCTURES_H__
/*
============================================================

	Essential structures for loading COLLADA format data

============================================================
*/

#include "TypeDefinitions.h"
#include "Debug.h"
#include "List.h"
#include <string>

using OGE::Collections::ListIterator;
using OGE::Collections::List;
using std::string;

namespace OGE
{
	namespace fileSystem
	{
		
		/*==========================================
			
			COLLADA geometry library structures

		============================================
		*/

		// COLLADA parameter structure
		struct CParam
		{
			string name;
			string type;
		};

		enum CInputSemantic
		{
			BINORMAL,
			COLOR,
			CONTINUITY,
			IMAGE,
			INPUT, 
			IN_TANGENT,
			INTERPOLATION,
			INV_BIND_MATRIX,
			JOINT,
			LINEAR_STEPS,
			MORPH_TARGET,
			MORPH_WEIGHT,
			NORMAL,
			OUTPUT,
			OUT_TANGENT,
			Chapter,
			POSITION,
			TANGENT,
			TEXBINORMAL, 
			TEXCOORD,
			TEXTANGENT,
			UV,
			VERTEX,
			WEIGHT 
		};

		enum NODE_TYPE
		{
			NODE_TYPE_NODE,
			NODE_TYPE_JOINT
		};

		// COLLADA input semantic of a data source
		struct CInput
		{
			string semantic;
			string sourceId;
			uint8 offset;
		};

		/*  COLLADA accessor describes a stream of values from an array 
			data source

			Count is the number of times the array is accessed. Required
		*/
		struct CAccessor
		{
			string arrayId;
			uint32 count;
			uint32 stride;

			CParam* pParams;
			uint32 countParams;
		};

		// COLLADA structure that describes float array
		struct CFloatArray
		{
			string id;
			string name;
			uint32 count;
			uint8 digits;
			uint16 magnitude;

			float* pData;
		};

		// COLLADA mesh source
		struct CSource
		{
			string id;
			string name;
			CFloatArray arrayEllement;
			CAccessor accessor;
		};

		// COLLADA mesh vertices topology 
		struct CVertices
		{
			string id;
			string name;
			CInput* pInputSemantic;

			uint32 countSemantics;
		};

		struct CPolygon
		{
			uint32* pData;
			uint32 count;
		};

		// COLLADA mesh primitives
		struct CPolygons
		{
			string materialId;
			uint32 countPrimitives;
			CPolygon* pPrimitive;

			CInput* pInputs;
			uint32 countInputs;
		};

		// COLLADA format mesh
		struct CMesh
		{
			CSource* pSources;
			uint32 sourcesCount;

			CVertices vertices;
			CPolygons* pPrimitivesBuffers;
			uint32 countPrimitivesBuffers;
		};

		// COLLADA Geometry structure
		struct CGeometry
		{
			string id;
			string name;
			CMesh mesh;
		};

		// COLLADA Geometry library structure
		struct CLibraryGeometries
		{
			string id;
			string name;
			CGeometry* pGeometry;
			uint32 countGeometries;
		};

		/*==========================================
			
			COLLADA effects library structures

		============================================
		*/

		struct CTexture
		{
			string textureId;
		};

		struct CColor
		{
			string sid;
			float value[4];
		};

		struct CFloat
		{
			string sid;
			float value;
		};

		struct CMaterialColorNode
		{
			CColor* pColor;
			CTexture* pTexture;
		};

		struct CMaterialDegreeNode
		{
			CFloat value;
		};

		struct CEffectMaterial
		{
			CMaterialColorNode emmision;
			CMaterialColorNode ambient;
			CMaterialColorNode diffuse;
			CMaterialColorNode reflective;
			CMaterialDegreeNode reflectivity;
			CMaterialColorNode trasparent;
			CMaterialDegreeNode transparency;
			CMaterialColorNode specular;
			CMaterialDegreeNode shininess;
		};

		struct CEffectTechnique
		{
			string sid;
			CEffectMaterial* pEffectMaterial;
		};

		struct CEffectProfileCommon
		{
			CEffectTechnique technique;
		};

		struct CEffect
		{
			string id;
			string name;
			CEffectProfileCommon profileCommon;
		};

		struct CLibraryEffects
		{
			CEffect* pEffect;
			uint32 countEffects;
		};

		/*
		===========================================

			COLLADA Materials library

		============================================
		*/

		struct CMaterial
		{
			string id;
			string name;
			string effectId;
		};

		struct CLibraryMaterial
		{
			CMaterial* pMaterials;
			uint32 countMaterials;
		};

		/*
		====================================

			Library lights

		====================================
		*/

		struct CLightAttenuation
		{
			float constAttenuation;
			float linearAttenuation;
			float quadraticAttenuation;
		};

		struct CDirectionalLight
		{
			string id;
			CColor lightColor;
		};

		struct CSpotlight
		{
			string id;
			CColor lightColor;
			CLightAttenuation attenuation;

			float falloffAngle;
			float falloffExp;
		};

		struct CPointLight
		{
			string id;
			CColor lightColor;
			CLightAttenuation attenuation;
		};

		struct CLibraryLights
		{
			CPointLight* pPointLights;
			uint32 countPointLights;

			CSpotlight* pSpotlights;
			uint32 countSpotlights;

			CDirectionalLight* pDirectionalLights;
			uint32 countDirectionalLights;
		};

		/*
		====================================

			Library cameras

		====================================
		*/

		struct CPerspective
		{
			// in graduses
			float yfov;
			float aspectRatio;
			float znear;
			float zfar;
		};

		struct COptics
		{
			CPerspective perspectiveProjection;
		};

		struct CCamera
		{
			string id;
			COptics optics;
		};

		struct CLibraryCameras
		{
			CCamera* pCameras;
			uint32 countCameras;
		};
	
		/*
		====================================

			Library scenes

		====================================
		*/

		struct CMatrix
		{
			float data[4][4];
		};

		struct CRotate
		{
			string sid;
			float x, y, z;
			float degree;
		};

		struct CTranslate
		{
			float x, y, z;
		};

		struct CScale
		{
			float x, y, z;
		};

		struct CInstanceMaterial
		{
			string sid;
			string name;
			string targetId;
			string symbolId;
		};

		struct CBindMaterial
		{
			CInstanceMaterial* pInstanceMaterials;
			uint32 countInstances;
		};

		struct CInstanceGeometry
		{
			string name;
			string sid;
			string geometryId;
			CBindMaterial* pBindMaterial;
		};

		struct CInstanceCamera
		{
			string sid;
			string name;
			string url;
		};

		struct CInstanceLight
		{
			string sid;
			string name;
			string url;
		};

		struct CNode
		{
			string id;
			string name;
			string sid;

			CMatrix* pMatrix;
			struct RTS
			{
				CRotate* pRotate;
				CTranslate* pTranslate;
				CScale* pScale;

				uint32 countRotate;
				uint32 countTranslate;
				uint32 countScale;

			}* pRTS;
			
			CInstanceGeometry* pInstanceGeometry;
			CInstanceCamera* pInstanceCamera;
			CInstanceLight* pInstanceLight;
			List<CNode*> nodes;
			NODE_TYPE nodeType;
		};

		struct CVisualScene
		{
			string id;
			string name;
			CNode** ppNodes;
			uint32 countNodes;
		};

		struct CLibraryVisualScenes
		{
			CVisualScene* pScenes;
			uint32 countScenes;
		};

		/*
		=======================================

			Images library

		========================================
		*/

		struct CInitFrom
		{
			string url;
		};

		struct CImage
		{
			string id;
			string sid;
			string name;
			CInitFrom initFromUrl;
		};

		struct CLibraryImages
		{
			CImage* pImages;
			uint32 countImages;
		};

		struct COLLADADocument 
		{
			CLibraryMaterial* pLibraryMaterials;
			CLibraryEffects* pLibraryEffects;
			CLibraryGeometries* pLibraryGeometries;
			CLibraryVisualScenes* pLibraryVisualScenes;
			CLibraryImages* pLibraryImages;
			CLibraryLights* pLibraryLights;
			CLibraryCameras* pLibraryCameras;
		};
	}
}

#endif