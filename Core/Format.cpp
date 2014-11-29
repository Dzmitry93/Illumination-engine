#include "Format.h"

namespace OGE
{
	namespace resource
	{

		uint16 Format::getSize(BUFFER_FORMAT format)
		{
			uint16 size = 0;
			switch(format)
			{
				case BUFFER_FORMAT::VERTEX_FORMAT_V3TNT2 : size = sizeof(V3TNT2Vertex); break;
				case BUFFER_FORMAT::VERTEX_FORMAT_V3N3T2 : size = sizeof(V3N3T2Vertex); break;
				case BUFFER_FORMAT::VERTEX_FORMAT_V3T2 : size = sizeof(V3T2Vertex); break;
				case BUFFER_FORMAT::VERTEX_FORMAT_V4T2 : size = sizeof(V4T2Vertex); break;
				case BUFFER_FORMAT::INDEX_16 : size = sizeof(uint16); break;
				case BUFFER_FORMAT::INDEX_32 : size = sizeof(uint32); break;
				case BUFFER_FORMAT::VERTEX_FORMAT_V4 : size = sizeof(V4Vertex); break;
			};

			return size;
		}

		uint8 Format::getCountVertices(PRIMITIVE_TOPOLOGY topology)
		{
			uint8 size = 1;
			switch(topology)
			{
				case PRIMITIVE_TOPOLOGY::TRIANGLE_LIST : size = 3;
				case PRIMITIVE_TOPOLOGY::LINE_LIST : size = 1;
				case PRIMITIVE_TOPOLOGY::LINE_STRIP : size = 1;
			};

			return size;
		}
	}
}