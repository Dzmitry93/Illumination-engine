#ifndef __BVH_H__
#define __BVH_H__

#include "TransformUtils.h"
#include "BoundingBodyFactory.h"
#include "ViewFrustum.h"
#include "DampedLight.h"
#include "MathUtils.h"

using OGE::math::AABB;
using OGE::math::ViewFrustum;
using OGE::util::TransformUtils;
using OGE::util::BoundingBodyFactory;

using namespace OGE::math::IntersectionUtils;
using namespace OGE::scene;
using namespace OGE;

namespace OGE
{
	namespace Collections
	{
		class BVH
		{
		public:

			BVH(const AABB& rRootAABB);
			~BVH(void);

			void construct(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const List<SceneObject<BaseLight*>>& rLights);

			List<SceneObject<IntrusivePtr<BaseMesh>>> getViewFrustumObjects(const ViewFrustum& rFrustum) const;
			List<OBB> getViewFrustumVolumes(const ViewFrustum& rFrustum) const;
			void getLightsData(const Vector& rPosition, float radius,
				List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData) const;

			List<AABB> getAABBNodes() const;

		private:

			struct Node
			{
				AABB mAabb;

				Node* pLeft;
				Node* pRight;

				Node* pParent;

				List<SceneObject<IntrusivePtr<BaseMesh>>> mObjectList;
				List<SceneObject<BaseLight*>> mLightList;
			};

			Node* pRoot;

			void getViewFrustumObjects(
				Node* pNode,
				const ViewFrustum& rFrustum,
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjectList,
				map<uint64, uint64>& rGuidCache) const;

			void getViewFrustumVolumes(Node* pNode, 
				const ViewFrustum& rFrustum,
				List<OBB>& rResultList,
				map<uint64, uint64>& rGuidCache) const;

			void getLightsData(
				Node* pNode,
				const Vector& rPosition, float radius,
				map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData) const;

			void getAABBNodes(List<AABB>& rResultList, Node* pRoot) const;

			void construct(
				Node* pRoot,
				List<OBB>& rXOBBs, 
				List<OBB>& rYOBBs,
				List<OBB>& rZOBBs);

			void insertObject(const SceneObject<IntrusivePtr<BaseMesh>>& rObject, OBB& rOBB, Node* pRoot);
			void insertLight(const SceneObject<BaseLight*>& rLight, Node* pRoot);

			void calculateSAHList(
				List<pair<float, pair<AABB, AABB>>>& rSAHList,
				List<OBB>& rSortedOBBs);

			pair<float, pair<AABB, AABB>> findMinSAH(
				List<pair<float, pair<AABB, AABB>>>& rSAHList,
				uint32& splitIndex);

			void createNewChildren(
				Node* pRoot,
				uint32 splitIndex,
				List<OBB>& rOBBs,
				List<OBB>& rLeftList,
				List<OBB>& rRightList,
				pair<float, pair<AABB, AABB>>& rMinSAH);

			void destroy(Node* pRoot);
		};

		inline BVH::BVH(const AABB& rRootAABB)
		{
			this->pRoot = new Node();
			this->pRoot->pLeft = nullptr;
			this->pRoot->pRight = nullptr;
			this->pRoot->pParent = nullptr;
			this->pRoot->mAabb = rRootAABB;
		}

		inline BVH::~BVH() 
		{ 
			destroy(pRoot);
		}

		inline void BVH::destroy(Node* pRoot)
		{
			if(pRoot->pLeft != nullptr)
			{
				destroy(pRoot->pLeft);
			}
			if(pRoot->pRight != nullptr)
			{
				destroy(pRoot->pRight);
			}

			delete pRoot;
		}

		inline List<SceneObject<IntrusivePtr<BaseMesh>>> BVH::getViewFrustumObjects(const ViewFrustum& rFrustum) const
		{
			map<uint64, uint64> guidCache;
			List<SceneObject<IntrusivePtr<BaseMesh>>> resultList;

			if(FrustumAABBIntersection(rFrustum, pRoot->mAabb) != OUTSIDE)
			{
				getViewFrustumObjects(pRoot, rFrustum, resultList, guidCache);
			}
			
			return resultList;
		}
		
		inline List<AABB> BVH::getAABBNodes() const
		{
			List<AABB> resultList;

			getAABBNodes(resultList, pRoot);
			return resultList;
		}

		inline void BVH::getLightsData(const Vector& rPosition, float radius,
			List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData) const
		{
			map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>> lightsData;
			getLightsData(pRoot, rPosition, radius, lightsData);

			map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>::iterator it = lightsData.begin();
			map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>::iterator end = lightsData.end();

			while(it != end)
			{
				rLightsData.push_back(it->second);
				it++;
			}
		}

		inline List<OBB> BVH::getViewFrustumVolumes(const ViewFrustum& rViewFrustum) const
		{
			map<uint64, uint64> guidCache;
			List<OBB> resultList;

			getViewFrustumVolumes(pRoot, rViewFrustum, resultList, guidCache);

			return resultList;
		}
	}
}

#endif


