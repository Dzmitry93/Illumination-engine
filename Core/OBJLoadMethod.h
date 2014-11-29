#ifndef __OBJ_LOAD_METHOD_H__
#define __OBJ_LOAD_METHOD_H__

/* 
=================================================
	
	This class involves obj data parser

=================================================
*/ 

/*

#include "Buffer.h"
#include "BufferFactory.h"
#include "Vector.h"
#include "Format.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

using std::string;
using std::list;
using std::vector;
using std::ifstream;
using std::istringstream;
using OGE::math::Vector;

using namespace OGE::resource;

namespace OGE
{
	namespace fileSystem
	{
		#define TRIANGLE 3

		class OBJLoadMethod : public ILoadMethod
		{
		public:

			// load model from obj file with resource usage specification
			void loadModel(const string& path, list<MaterialData>& materials,
				list<TextureData>& texturesPaths, ModelData& models);
			 
		private:

			// load and parse obj data 
			void loadOBJData(const string& path, 
				vector<V3N3T2Vertex>& vertices, 
				vector<uint32>& triangles);
			
			// parsing data methods
			void parseVertexData(V3N3T2Vertex& objVertex, istringstream& data);
			void parseNormalData(Vector& normal, istringstream& data);
			void parseTextureData(Vector& texture, istringstream& data);
			void parseFaceData(vector<uint32>& triangles,  
				vector<V3N3T2Vertex>& vertices, 
				vector<Vector>& normals, 
				vector<Vector>& textureData, 
				istringstream& data);

		};
	}
}
*/
#endif