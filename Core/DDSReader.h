#ifndef __DDS_READER_H__
#define __DDS_READER_H__

#include "IImageReader.h"

namespace OGE
{
	namespace fileSystem 
{

		class DDSReader	: public IImageReader
		{
		public:
			virtual OGETextureData readImage(const string& rPath);

		};
	}
}

#endif
