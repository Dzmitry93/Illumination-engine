#ifndef __TEXTURE_FACTORY_H__
#define __TEXTURE_FACTORY_H__

#include "Texture2D.h"
#include "GUIDUtil.h"
#include "HashUtil.h"
#include "BMPReader.h"
#include "DDSReader.h"
#include <string>
#include <fstream>

using std::string;
using std::ifstream;
using OGE::util::GUIDUtil;
using OGE::util::HashUtil;
using namespace OGE::fileSystem;

namespace OGE
{
	namespace resource
	{
		class TextureFactory
		{
		public:
			TextureFactory(void);
			~TextureFactory(void);

			static Texture2D* createTextureFromFile(const string& filePath);

			static IImageReader* getImageReader(const string& rPath);

		private:
			static string bmp;
			static string dds;
		};
	}
}

#endif

