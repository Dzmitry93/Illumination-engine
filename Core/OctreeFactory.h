#ifndef __MESH_OCTREE_FACTORY_H__
#define __MESH_OCTREE_FACTORY_H__

#include "Octree.h"

using OGE::scene::SceneObject;
using OGE::Collections::Octree;

namespace OGE
{
	namespace Collections
	{
		class OctreeFactory
		{
		public:
			Octree* createOctree(List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const AABB& rRootBox, uint32 maxCountPerNode);

		private:
			void split(OctreeNode* pNode);
		};
	}
}

#endif