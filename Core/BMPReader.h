#pragma once
#ifndef __BMP_READER_H__
#define __BMP_READER_H__

#include "IImageReader.h"

namespace OGE
{
	namespace fileSystem
	{
		class BMPReader : public IImageReader
		{
		public:
			virtual OGETextureData readImage(const string& rPath);
		};
	}
}

#endif

