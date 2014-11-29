#include "DDSReader.h"

namespace OGE 
{
	namespace fileSystem 
	{
		OGETextureData DDSReader::readImage(const string& rPath)  
		{
			OGETextureData textureData;

			ZeroMemory(&textureData, sizeof(textureData));

			textureData.texturePath = rPath;
			textureData.textureName = rPath;

			ifstream input(rPath, ios::binary);

			if(input.is_open()) 
			{
				int32 magicNumber;
				int32 headerSize;
				int32 flags;
				int32 linearSize;				  

				input.read((char*)&magicNumber, 4);
				input.read((char*)&headerSize, 4);
				input.read((char*)&flags, 4);

				if(magicNumber != 0x20534444 || headerSize != 124 ) {
					error_msg("file has unsupported format or did damaged");
				} else 
				{
					input.read((char*)&textureData.height, 4);
					input.read((char*)&textureData.width, 4);
					input.read((char*)&linearSize, 4);

					textureData.bitsPerPixel = linearSize / textureData.width;

					input.seekg(0, ios::end);
					textureData.textureSize = input.tellg();
					input.seekg(0, ios::beg);
											
					textureData.pTextureData = new int8[textureData.textureSize];

					input.read((char*)textureData.pTextureData, textureData.textureSize);
					input.close();
				}
			} else 
			{
				error_msg("file has unsupported format or did damaged");
			}

			return textureData;
		}
	}
}