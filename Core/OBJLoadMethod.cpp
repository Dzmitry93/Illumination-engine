#include "OBJLoadMethod.h"

/*
namespace OGE
{
	namespace fileSystem
	{

		void OBJLoadMethod::loadModel(const string& path, list<OGEMaterialData>& materials,
				list<OGETextureData>& texturesPaths, OGEModelData& modelData)
		{
			vector<V3N3T2Vertex> vertices;
			vector<uint32> triangles;

			loadOBJData(path, vertices, triangles);

			uint32 verticesSize = vertices.size() * sizeof(V3N3T2Vertex);
			uint32 indexesSize = triangles.size() * sizeof(uint32);

			V3N3T2Vertex* pVertices = new V3N3T2Vertex[verticesSize];
			uint32* pIndexes = new uint32[indexesSize];

			memcpy((void*)pVertices, (void*)vertices.data(), verticesSize);
			memcpy((void*)pIndexes, (void*)triangles.data(), indexesSize);

			Buffer* pVerticesBuffer = BufferFactory::createBuffer(
				VERTEX_FORMAT_V3N3T2, pVertices, verticesSize, 
				Format::getSize(VERTEX_FORMAT_V3N3T2));

			Buffer* pIndexesBuffer = BufferFactory::createBuffer(
				VERTEX_FORMAT_UNKNOW, pVertices, verticesSize, 
				sizeof(uint32));

			ZeroMemory((void*)&modelData, sizeof(ModelData));

			modelData.pVerticesBuffer = pVerticesBuffer;
			modelData.pIndexesBuffer = pIndexesBuffer;

		}

		void OBJLoadMethod::loadOBJData(
				const string& path, 
				vector<V3N3T2Vertex>& vertices, 
				vector<uint32>& triangles)
		{
			ifstream in(path);
			string current = "none";

			vector<Vector> vertexNormals;
			vector<Vector> textureCoordinates;

			while(!in.eof())
			{
				char data[100];
				in.getline(data, 100);

				string strData(data);
				istringstream dataStream(strData);
				
				if(data[0] == 'v')
				{
					if(data[1] == ' ')
					{
						V3N3T2Vertex vertex;

						parseVertexData(vertex, dataStream);
						vertices.push_back(vertex);
					}
					else if(data[1] == 't')
					{
						Vector textureData;

						parseTextureData(textureData, dataStream);
						textureCoordinates.push_back(textureData);
					}
					else if(data[1] == 'n')
					{
						Vector normal;

						parseNormalData(normal, dataStream);
						vertexNormals.push_back(normal);
					}
				}
				else if (data[0] == 'f')
				{
					parseFaceData(triangles, vertices, 
						vertexNormals, textureCoordinates, dataStream);
				}	
			}

			in.close();
		}

		void OBJLoadMethod::parseVertexData(V3N3T2Vertex& vertex, istringstream& dataStream)
		{
			string signature = "";
			dataStream >> signature;

			if(signature == "v")
			{
				dataStream >> vertex.x;
				dataStream >> vertex.y;
				dataStream >> vertex.z;
			}
		}

		void OBJLoadMethod::parseNormalData(Vector& normal, istringstream& dataStream)
		{
			string signature = "";
			dataStream >> signature;

			if(signature == "vn")
			{
				dataStream >> normal.x;
				dataStream >> normal.y;
				dataStream >> normal.z;
			}
		}

		void OBJLoadMethod::parseTextureData(Vector& textureData, istringstream& dataStream)
		{
			string signature = "";
			dataStream >> signature;

			if(signature == "vt")
			{
				dataStream >> textureData.x;
				dataStream >> textureData.y;

				if(!dataStream.eof())
				{
					dataStream >> textureData.z;
				}
			}
		}

		void OBJLoadMethod::parseFaceData(vector<uint32>& trianglesIndexes,
				vector<V3N3T2Vertex>& vertices, vector<Vector>& normals, 
				vector<Vector>& textureData, istringstream& dataStream)
		{
			string signature = "";
			dataStream >> signature;

			if(signature == "f")
			{
				uint32 countVertices = 0;
				uint32 indexData[4];

				while(!dataStream.eof())
				{
					uint32 countSlashes = 0;
					uint32 base = 10;
					uint32 digit = 1;
					uint32 value = 0;
					uint32 vertexIndex = 0;
					uint8 indexNumberBegin = 0;

					string indexes;
					dataStream >> indexes;

					for(uint8 i = 0; i < indexes.size(); i++)
					{
						if(indexes[i] >= 48 && indexes[i] <= 57)
						{
							value += digit * (indexes[i] - 48);
							digit *= base;
						}

						if(indexes[i] == '/' || i == indexes.size() - 1)
						{
							uint32 temp = 0;
							digit /= base;

							while(value > 0)
							{
								temp += value % base * digit;
								digit /= base;
								value /= base;
							}

							value = temp;

							indexNumberBegin = i + 1;
							countSlashes++;
									
							if(countSlashes == 1)
							{
								vertexIndex = value;
							}
							else if(countSlashes == 2 && textureData.size() > 0)
							{
								vertices[vertexIndex-1].tx = textureData[value-1].x;
								vertices[vertexIndex-1].ty = textureData[value-1].y;
							}
							else if(countSlashes == 3 && normals.size() > 0)
							{
								vertices[vertexIndex-1].nx = normals[value-1].x;
								vertices[vertexIndex-1].ny = normals[value-1].y;
								vertices[vertexIndex-1].nz = normals[value-1].z;
							}

							digit = 1;
							value = 0;
						}			
					}

					if(indexes.size() > 0)
					{
						countVertices++;
						indexData[countVertices - 1] = vertexIndex-1;
					}
				}

				trianglesIndexes.push_back(indexData[0]);
				trianglesIndexes.push_back(indexData[1]);
				trianglesIndexes.push_back(indexData[2]);
			}
		}

	}
}
*/