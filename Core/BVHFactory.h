#ifndef __BVH_FACTORY_H__
#define __BVH_FACTORY_H__

#include "List.h"
#include "SceneObject.h"
#include "AABB.h"
#include "BVH.h"

using OGE::scene::SceneObject;

namespace OGE
{
	namespace Collections
	{
		class BVHFactory
		{
		public:
			static BVH* createBVH(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const List<SceneObject<BaseLight*>>& rLights,
				const AABB& rRootAABB);
		};

		inline BVH* BVHFactory::createBVH(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const List<SceneObject<BaseLight*>>& rLights,
				const AABB& rRootAABB)
		{
			BVH* pBVH = new BVH(rRootAABB);
			pBVH->construct(rObjects, rLights);

			return pBVH;
		}
	}
}

#endif