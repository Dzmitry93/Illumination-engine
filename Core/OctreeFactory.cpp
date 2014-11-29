#include "OctreeFactory.h"

namespace OGE
{
	namespace Collections
	{
		Octree* OctreeFactory::createOctree(List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects, 
			const AABB& rRootBox, uint32 maxCountPerNode)
		{	
			Octree* pOctree = new Octree(rRootBox, maxCountPerNode);
			ListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = rObjects.iterator();

			while(it.hasNext())
			{
				pOctree->insert(it.next());
			}

			return pOctree;
		}
	}
}