#ifndef __FORMAT_H__
#define __FORMAT_H__

#include "TypeDefinitions.h"
#include "Debug.h"

namespace OGE
{
	namespace resource
	{
		struct V3N3T2Vertex
		{
			float x, y, z;
			float nx, ny, nz;
			float tx, ty;
		};

		struct V3TNT2Vertex
		{
			float x, y, z;
			float nx, ny, nz;
			float tnx, tny, tnz;
			float bnx, bny, bnz;
			float tx, ty;
		};

		struct V3T2Vertex
		{
			float x, y, z;
			float tx, ty;
		};

		struct V4T2Vertex
		{
			float x, y, z, w;
			float tx, ty;
		};

		struct V4Vertex
		{
			float x, y, z, w;
		};

		struct Vertex
		{
			float vx, vy, vz;
		};

		struct Normal
		{
			float nx, ny, nz;
		};

		struct Texcoord2
		{
			float tx, ty;
		};

		struct Texcoord3
		{
			float tx, ty, tz;
		};

		struct V3N3T2Triangle
		{
			V3N3T2Vertex v1;
			V3N3T2Vertex v2;
			V3N3T2Vertex v3;
		};

		struct V3TNT2Triangle
		{
			V3TNT2Vertex v1;
			V3TNT2Vertex v2;
			V3TNT2Vertex v3;
		};

		struct V3T2Triangle
		{
			V3T2Vertex v1;
			V3T2Vertex v2;
			V3T2Vertex v3;
		};

		struct V4T2Triangle
		{
			V4T2Vertex v1;
			V4T2Vertex v2;
			V4T2Vertex v3;
		};

		struct V4Triangle
		{
			V4Vertex v1;
			V4Vertex v2;
			V4Vertex v3;
		};

		enum BUFFER_FORMAT
		{
			VERTEX_FORMAT_V3TNT2,
			VERTEX_FORMAT_V3N3T2,
			VERTEX_FORMAT_V3T2,
			VERTEX_FORMAT_V4T2,
			VERTEX_FORMAT_V4,
			INDEX_32,
			INDEX_16,
			FORMAT_UNKNOW
		};

		enum PRIMITIVE_TOPOLOGY
		{
			TRIANGLE_LIST,
			TRIANGLE_STRIP,
			LINE_LIST,
			LINE_STRIP
		};

		class Format
		{
		public:

			static uint16 getSize(BUFFER_FORMAT format);
			static uint8 getCountVertices(PRIMITIVE_TOPOLOGY);
		};	
	}
}

#endif