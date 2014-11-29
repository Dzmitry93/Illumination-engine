
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "Texture2D.h"
#include "TerrainFactory.h"

using namespace OGE::resource;

namespace OGE
{
	namespace terrain
	{
		
		class Terrain
		{
		public:
			Terrain(void);
			~Terrain(void);


		private: 
			Texture2D* pHeightmap;
			TerrainQuadtree* pTerrainQuadtree;
		};


		inline Terrain::Terrain() 
		{

		}

		inline Terrain::~Terrain() 
		{
			
		}
	}
}

#endif