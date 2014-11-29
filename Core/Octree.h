#ifndef __OCTREE_H__
#define __OCTREE_H__

#include "List.h"
#include "SceneObject.h"
#include "ViewFrustum.h"
#include "TransformUtils.h"
#include "MathUtils.h"
#include "DampedLight.h"

#include <map>

using OGE::scene::DampedLight;

using OGE::resource::BaseMesh;

using OGE::scene::SceneObject;

using OGE::math::Matrix;
using OGE::math::AABB;
using OGE::math::OBB;

using OGE::util::TransformUtils;

using std::map;

using namespace OGE::math::IntersectionUtils;

namespace OGE
{
	namespace Collections
	{
		class OctreeNode
		{
		public:
			AABB mAabb;
			OctreeNode* pParent;
			OctreeNode* mNodes[8];	 
			List<SceneObject<IntrusivePtr<BaseMesh>>> mObjectList;
			bool hasChildren;
		};
			
		class Octree
		{
		public:
			Octree(const AABB& rRoot, uint32 maxCountPerNode);
			~Octree(void);

			void insert(const SceneObject<IntrusivePtr<BaseMesh>>& rObject);
			void update(const Matrix& rLocal, uint32 GUID);
			void remove(uint32 GUID);
			void clear();

			List<SceneObject<IntrusivePtr<BaseMesh>>> getViewFrustumObjects(const ViewFrustum& rFrustum) const;
			List<SceneObject<IntrusivePtr<BaseMesh>>> getLightVisibleObjects(const DampedLight* pLight) const;

			List<AABB> getAABBNodes() const;

		private:
			
			struct SearchResult
			{
				SearchResult() : pNode(nullptr) {}
				SearchResult(OctreeNode* pNode, const SceneObject<IntrusivePtr<BaseMesh>>& rObject)
					: pNode(pNode), renderableObject(rObject)	{}

				OctreeNode* pNode;
				SceneObject<IntrusivePtr<BaseMesh>> renderableObject;
			};

			OctreeNode* pRoot;
			uint32 mMaxCountPerNode;

			void destroy(OctreeNode* pRoot);
			void insert(OctreeNode* pRoot, const SceneObject<IntrusivePtr<BaseMesh>>& rObject);

			void split(OctreeNode* pRoot);
			void updateNode(OctreeNode* pNode);

			SearchResult find(OctreeNode* pRoot, uint32 guid);

			void getAABBChild(OctreeNode* pRoot, List<AABB>& rChilds) const;

			void getViewFrustumObjects(
				OctreeNode* pNode,
				const ViewFrustum& rFrustum,
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjectList,
				map<uint64, uint64>& rGuidCache) const;

			void getLightVisibleObjects(
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const DampedLight* pLight,
				OctreeNode* pRoot,
				map<uint64, uint64>& rGuidCache) const;
		};

		inline Octree::Octree(const AABB& rRoot, uint32 maxCountPerNode) 
		{
			this->pRoot = new OctreeNode();
			this->pRoot->mAabb = rRoot;
			this->pRoot->hasChildren = false;
			this->mMaxCountPerNode = maxCountPerNode;
		}

		inline Octree::~Octree() 
		{
			if(pRoot != nullptr)
			{
				destroy(pRoot);
				safe_release(pRoot);
			}
		}

		inline void Octree::clear() 
		{
			destroy(pRoot);
		}

		inline void Octree::insert(const SceneObject<IntrusivePtr<BaseMesh>>& rObject)
		{
			insert(pRoot, rObject);
		}

		inline List<AABB> Octree::getAABBNodes()  const
		{
			List<AABB> AABBnodes;
			getAABBChild(pRoot, AABBnodes);

			return AABBnodes;
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> Octree::getViewFrustumObjects(const ViewFrustum& rFrustum) const
		{
			map<uint64, uint64> guidCache;
			List<SceneObject<IntrusivePtr<BaseMesh>>> resultList;

			if(FrustumAABBIntersection(rFrustum, pRoot->mAabb) != OUTSIDE)
			{
				getViewFrustumObjects(pRoot, rFrustum, resultList, guidCache);
			}
			
			return resultList;
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> Octree::getLightVisibleObjects(const DampedLight* pLight) const
		{
			map<uint64, uint64> guidCache;
			List<SceneObject<IntrusivePtr<BaseMesh>>> resultList;

			getLightVisibleObjects(resultList, pLight, pRoot, guidCache);

			return resultList;
		}

		inline void Octree::destroy(OctreeNode* pRoot)
		{
			if(pRoot != NULL)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					destroy(pRoot->mNodes[i]);
					safe_release(pRoot->mNodes[i]);
				}
			}
		}
	}
}

#endif								  