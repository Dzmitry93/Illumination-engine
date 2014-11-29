#include "Octree.h"

namespace OGE
{
	namespace Collections
	{
		void Octree::insert(OctreeNode* pRoot, const SceneObject<IntrusivePtr<BaseMesh>>& rObject)
		{
			const AABB& rAABB = pRoot->mAabb;
			OBB obb = TransformUtils::transform(rObject.getOBB(), rObject.getWorldMatrix());

			if(OverlapOBBOBB(obb, rAABB))
			{
				if(pRoot->mObjectList.size() >= mMaxCountPerNode)
				{
					pRoot->mObjectList.push_back(rObject);
					split(pRoot);
				}

				if(pRoot->hasChildren)
				{
					ListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = pRoot->mObjectList.iterator();

					while(it.hasNext()) {
						SceneObject<IntrusivePtr<BaseMesh>>& rTempRenderable = it.next();

						for(uint32 i = 0; i < 8; i++)
						{
							insert(pRoot->mNodes[i], rTempRenderable);
						}
					}

					pRoot->mObjectList.clear();
				}
				else
				{
					pRoot->mObjectList.push_back(rObject);
				}
			}
		}

		void Octree::update(const Matrix& rTransforamtion, uint32 guid)
		{
			SearchResult result = find(pRoot, guid);

			if(result.pNode != nullptr) 
			{
				OctreeNode* pNode = result.pNode;
				SceneObject<IntrusivePtr<BaseMesh>>& rObject = result.renderableObject;

				OctreeNode* pParent = pNode->pParent == nullptr ? pNode : pNode->pParent;

				const AABB& rAABB = pNode->mAabb;

				const Matrix& rLocal = rObject.getLocalMatrix();
				const Matrix& rWorld = rObject.getWorldMatrix();

				rObject.setLocalMatrix(rLocal * rTransforamtion);

				OBB rOBB = TransformUtils::transform(rObject.getOBB(), rWorld * rLocal);

				if(!OverlapOBBOBB(rOBB, OBB(rAABB)))
				{
					pNode->mObjectList.remove(rObject);				
				}

				for(uint32 i = 0; i < 8; i++)
				{
					insert(pParent->mNodes[i], rObject);
				}
				
				updateNode(pParent);
			}
		}

		void Octree::getViewFrustumObjects(
				OctreeNode* pNode,
				const ViewFrustum& rFrustum,
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjectList,
				map<uint64, uint64>& rGuidCache) const
		{
			if(pNode->hasChildren)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					OctreeNode* pChild = pNode->mNodes[i];
					if(FrustumAABBIntersection(rFrustum, pChild->mAabb) != OUTSIDE)
					{
						getViewFrustumObjects(pChild, rFrustum, rObjectList, rGuidCache);
					}
				}
			} 
			else 
			{
				ListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = pNode->mObjectList.iterator();

				while(it.hasNext())
				{
					const SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
					uint64 guid = rObject.getSceneObject()->getGUID();

					map<uint64, uint64>::iterator it = rGuidCache.find(guid);

					if(it == rGuidCache.end())
					{
						OBB obb = TransformUtils::transform(
							rObject.getOBB(),
							rObject.getWorldMatrix());

						if(FrustumOBBIntersection(rFrustum, obb) != OUTSIDE)
						{
							rObjectList.push_back(rObject);
							rGuidCache.insert(std::pair<uint64, uint64>(guid, guid));
						}
					}
				}
			}
		}

		void Octree::getLightVisibleObjects(
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const DampedLight* pLight,
				OctreeNode* pNode,
				map<uint64, uint64>& rGuidCache) const
		{
			if(pNode->hasChildren)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					OctreeNode* pChild = pNode->mNodes[i];
					if(SphereAABBIntersection(
						pLight->getLightPosition(),
						pLight->getAttenuationLaw().MaxAffectedDistance,
						pChild->mAabb) != OUTSIDE)
					{
						getLightVisibleObjects(rObjects, pLight, pNode, rGuidCache);
					}
				}
			} 
			else 
			{
				ListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = pNode->mObjectList.iterator();

				while(it.hasNext())
				{
					const SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
					uint64 guid = rObject.getSceneObject()->getGUID();

					map<uint64, uint64>::iterator it = rGuidCache.find(guid);

					if(it == rGuidCache.end())
					{
						OBB obb = TransformUtils::transform(
							rObject.getOBB(), 
							rObject.getWorldMatrix());

						if(SphereOBBIntersection(
							pLight->getLightPosition(),
							pLight->getAttenuationLaw().MaxAffectedDistance,
							obb) != OUTSIDE)
						{
							rObjects.push_back(rObject);
							rGuidCache.insert(std::pair<uint64, uint64>(guid, guid));
						}
					}
				}
			}
		}

		void Octree::remove(uint32 guid)
		{
			SearchResult result = find(pRoot, guid);
			result.pNode->mObjectList.remove(result.renderableObject);

			updateNode(result.pNode);
		}

		void Octree::updateNode(OctreeNode* pNode)
		{
			uint32 count = 0;
			bool emptyChildren = true;

			for(uint32 i = 0; i < 8; i++)
			{
				OctreeNode* pChild = pNode->mNodes[i];
				if(pChild->hasChildren)
				{
					updateNode(pChild);
					emptyChildren = false;
				}
				else
				{
					count +=  pChild->mObjectList.size();
				}
			}

			if(emptyChildren)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					safe_release(pNode->mNodes[i]);
				}
			}
		}

		Octree::SearchResult Octree::find(OctreeNode* pRoot, uint32 guid)
		{
			for(uint32 i = 0; i <8; i++)
			{
				if(pRoot->hasChildren)
				{
					return find(pRoot->mNodes[i], guid);
				}

				ListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = pRoot->mObjectList.iterator();
					
				while(it.hasNext())
				{
					SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
					if(rObject.getSceneObject()->getGUID() == guid)
					{
						return SearchResult(pRoot, rObject);
					}
				}
			}
		}

		void Octree::split(OctreeNode* pRoot) 
		{
			pRoot->hasChildren = true;

			const AABB& mRootBB = pRoot->mAabb;
			float x = mRootBB.halfDistance.x / 2.0f;
			float y = mRootBB.halfDistance.y / 2.0f;
			float z = mRootBB.halfDistance.z / 2.0f;

			float cx = mRootBB.centerPoint.x;
			float cy = mRootBB.centerPoint.y;
			float cz = mRootBB.centerPoint.z;

			Vector halfD(x, y, z);
		
			AABB aabb(cx - x, cy - y, cz - z, halfD);
			AABB aabb2(cx + x, cy - y, cz - z, halfD);
			AABB aabb3(cx - x, cy + y, cz - z, halfD);
			AABB aabb4(cx + x, cy + y, cz - z, halfD);
			AABB aabb5(cx + x, cy + y, cz + z, halfD);
			AABB aabb6(cx - x, cy + y, cz + z, halfD);
			AABB aabb7(cx + x, cy - y, cz + z, halfD);
			AABB aabb8(cx - x, cy - y, cz + z, halfD);

			for(uint32 i = 0; i < 8; i++)
			{
				pRoot->mNodes[i] = new OctreeNode();
				pRoot->mNodes[i]->hasChildren = false;
				pRoot->mNodes[i]->pParent = pRoot;
			}

			pRoot->mNodes[0]->mAabb = aabb;
			pRoot->mNodes[1]->mAabb = aabb2;
			pRoot->mNodes[2]->mAabb = aabb3;
			pRoot->mNodes[3]->mAabb = aabb4;
			pRoot->mNodes[4]->mAabb = aabb5;
			pRoot->mNodes[5]->mAabb = aabb6;
			pRoot->mNodes[6]->mAabb = aabb7;
			pRoot->mNodes[7]->mAabb = aabb8;
		}

		void Octree::getAABBChild(OctreeNode* pRoot, List<AABB>& rChilds)  const
		{
			if(pRoot->hasChildren) 
			{
				for(uint32 i = 0;  i < 8; i++) 
				{
					getAABBChild(pRoot->mNodes[i], rChilds);
				}
			}  else 
			{
				rChilds.push_back(pRoot->mAabb);
			}
		}
	}
}