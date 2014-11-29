#ifndef __FONT_LIBRARY_FACTORY_H__
#define __FONT_LIBRARY_FACTORY_H__

#include "FontLibrary.h"
#include "TextureFactory.h"
#include <fstream>
#include <string>

using OGE::resource::TextureFactory;
using std::ifstream;
using std::string;

namespace OGE
{
	namespace render
	{
		class FontLibraryFactory
		{
		public:
			static FontLibrary* loadLibrary(const string& libPath);
		};
	}
}

#endif

