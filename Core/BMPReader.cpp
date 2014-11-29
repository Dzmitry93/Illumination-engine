#include "BMPReader.h"

namespace OGE
{
	namespace fileSystem
	{
		OGETextureData BMPReader::readImage(const string& rPath)
		{
			OGETextureData data;

			ZeroMemory(&data, sizeof(data));

			data.texturePath = rPath;
			data.textureName = rPath;

			ifstream input(rPath, ifstream::binary);

			if(input.is_open())
			{
				uint32 headerSize = 0;
				uint32 fileSize = 0;

				input.seekg(4, ios::cur);
				input >> data.width;
				input >> data.height;
				input.seekg(2, ios::cur);
				input >> data.bitsPerPixel;
				input.seekg(6, ios::cur);
				input.seekg(0, ios::beg);
				
				input.seekg(0, ios::end);
				data.textureSize = input.tellg();
				input.seekg(0, ios::beg);

				data.pTextureData = new uint8[data.textureSize];
				input.read((char*) data.pTextureData, data.textureSize);

				input.close();
			} else 
			{
				error_msg("cannot open : " + rPath);
			}
			
			return data;
		}
	}
}
