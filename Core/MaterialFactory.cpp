#include "MaterialFactory.h"

namespace OGE
{
	namespace resource
	{
		string MaterialFactory::material = "material";

		Material* MaterialFactory::createMaterial(const OGEMaterialData& rMaterialData, bool generateName)
		{ 
			Material* pMaterial = new Material();
			string name;

			if(generateName)
			{
				name = material + GUIDUtil::generateRandomGUID();
			}
			else
			{
				name = rMaterialData.materialName;
			}

			uint64 guid = HashUtil::getHash(name);
			pMaterial->setResourceName(name);
			pMaterial->setGUID(guid);

			// set diffuse component
			Texture2D* pDiffuseTexture = TextureFactory::createTextureFromFile(rMaterialData.diffuseTexture.texturePath);
			pMaterial->setDiffuseTexture(pDiffuseTexture);

			// set specular component
			pMaterial->setSpecular(rMaterialData.specularAlbedo, rMaterialData.specularPower);
			
			Texture2D* pNormalTexture = TextureFactory::createTextureFromFile(rMaterialData.normalTexture.texturePath);
			pMaterial->setNormalTexture(pNormalTexture);

			return pMaterial;
		}
	}
}
