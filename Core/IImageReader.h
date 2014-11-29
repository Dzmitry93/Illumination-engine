#ifndef __IIMAGE_READER_H__
#define __IIMAGE_READER_H__

#include "ImportStructures.h"
#include <iostream>

using std::ifstream;

namespace OGE
{
	namespace fileSystem
	{
		class IImageReader
		{
		public:
			virtual OGETextureData readImage(const string& rPath) = 0;
		};
	}
}

#endif