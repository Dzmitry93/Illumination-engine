#ifndef __OGE_SCENE_FORMAT_H__
#define __OGE_SCENE_FORMAT_H__

#include "Matrix.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ARGBColor.h"
#include "List.h"
#include <string>


using std::string;
using namespace OGE::resource;
using namespace OGE::Collections;
using OGE::math::Matrix;
//using OGE::math::Vector;

namespace OGE
{
	namespace fileSystem
	{
		struct OGETextureData
		{
			string textureName;
			string texturePath;

			void* pTextureData;
			uint64 textureSize;

			uint32 width;
			uint32 height;
			uint32 bitsPerPixel;
		};

		// Material data containter
		struct OGEMaterialData
		{
			string materialName;

			ARGBColor specularAlbedo;
			float specularPower;
			
			OGETextureData diffuseTexture;
			OGETextureData normalTexture;
		};

		struct OGELightAttenuation
		{
			float constAttenuation;
			float linearAttenuation;
			float quadraticAttenuation;
		};

		struct OGEDirectionalLight
		{
			Vector direction;
			ARGBColor lightColor;
		};

		struct OGESpotlight
		{
			Vector direction;
			Vector position;
			ARGBColor lightColor;
			OGELightAttenuation attenuation;

			float falloffAngle;
			float falloffExp;
		};

		struct OGEPointLight
		{
			Vector position;
			ARGBColor lightColor;
			OGELightAttenuation attenuation;
		};

		struct OGELightData
		{
			string id;

			OGEDirectionalLight* pDirectionalLight;
			OGESpotlight* pSpotlight;
			OGEPointLight* pPointLight;
		};

		struct OGECameraData
		{
			string id;

			float yfov;
			float aspectRatio;
			float znear;
			float zfar;

			Vector cameraPosition;
			Vector cameraDirection;
			Vector upDirection;
		};

		struct OGEModelData
		{
			VertexBuffer* pVerticesBuffer;
			IndexBuffer* pIndexesBuffer;
			string materialID;
			string geometryName;
		};
	
		struct OGESceneNode
		{
			OGEModelData* pModelData;
			OGECameraData* pCameraData;
			OGELightData* pLightData;
			Matrix translation;
			Matrix rotation;
			Matrix scale;
			List<OGESceneNode*> sceneNodes;
			OGESceneNode* pParent;
		};

		struct OGESceneData
		{
			OGESceneNode* pRoot;
		};
	}
}

#endif