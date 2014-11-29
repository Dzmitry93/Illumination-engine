#include "TextureFactory.h"

namespace OGE
{
	namespace resource
	{
		string TextureFactory::bmp = "bmp";
		string TextureFactory::dds = "dds";

		Texture2D* TextureFactory::createTextureFromFile(const string& filePath)
		{
			Texture2D* pTexture = nullptr;

			IImageReader* pReader = getImageReader(filePath);
			OGETextureData data =  pReader->readImage(filePath);

			Buffer* pBuffer = new Buffer(data.pTextureData, data.textureSize);
			string textureName = "texture" + GUIDUtil::generateRandomGUID();

			pTexture = new Texture2D(pBuffer, 
										data.width,
										data.height,
										data.bitsPerPixel,
										data.textureSize);

			pTexture->setResourcePath(filePath);
			pTexture->setResourceName(textureName);
			pTexture->setGUID(HashUtil::getHash(textureName));
			
			pBuffer->release();
			safe_array_release(data.pTextureData);

			return pTexture;
		}

		IImageReader* TextureFactory::getImageReader(const string& rPath) 
		{
			size_t pos = rPath.find(".");
			IImageReader* pImageReader = nullptr;

			if(++pos != string::npos)
			{
				string extension = rPath.substr(pos, string::npos);

				if(extension == bmp)
				{
					pImageReader = new BMPReader();
				}
				if(extension == dds)
				{
					pImageReader = new DDSReader();
				}
			}

			return pImageReader;
		}
	}
}
