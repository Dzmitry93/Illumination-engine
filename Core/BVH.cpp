#include "BVH.h"

namespace OGE
{
	namespace Collections
	{
		void BVH::construct(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjects,
				const List<SceneObject<BaseLight*>>& rLights)
		{
			List<OBB> xSortedOBBs;
			List<OBB> ySortedOBBs;
			List<OBB> zSortedOBBs;

			ConstListIterator<SceneObject<IntrusivePtr<BaseMesh>>> it = rObjects.citerator();

			while(it.hasNext())
			{
				const SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
				OBB obb = TransformUtils::transform(rObject.getOBB(), rObject.getWorldMatrix());

				xSortedOBBs.push_back(obb);
				ySortedOBBs.push_back(obb);
				zSortedOBBs.push_back(obb);
			}

			construct(pRoot, xSortedOBBs, ySortedOBBs, zSortedOBBs);

			it = rObjects.citerator();
			
			while(it.hasNext())
			{
				const SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
				OBB obb = TransformUtils::transform(rObject.getOBB(), rObject.getWorldMatrix());

				insertObject(rObject, obb, pRoot);
			}

			ConstListIterator<SceneObject<BaseLight*>> lightIt = rLights.citerator();

			while(lightIt.hasNext())
			{
				insertLight(lightIt.next(), pRoot);
			}
		}

		void BVH::construct(
				Node* pRoot,
				List<OBB>& xSortedOBBs, 
				List<OBB>& ySortedOBBs,
				List<OBB>& zSortedOBBs)
		{
			List<pair<float, pair<AABB, AABB>>> xSAHList;		
			List<pair<float, pair<AABB, AABB>>> ySAHList;
			List<pair<float, pair<AABB, AABB>>> zSAHList;

			xSortedOBBs.sort(Comparators::pOBBOBBComparatorX);
			ySortedOBBs.sort(Comparators::pOBBOBBComparatorY);
			zSortedOBBs.sort(Comparators::pOBBOBBComparatorZ);

			calculateSAHList(xSAHList, xSortedOBBs);
			calculateSAHList(ySAHList, ySortedOBBs);
			calculateSAHList(zSAHList, zSortedOBBs);

			uint32 xSplitIndex = 0;
			uint32 ySplitIndex = 0;
			uint32 zSplitIndex = 0;

			pair<float, pair<AABB, AABB>> minXSAH = findMinSAH(xSAHList, xSplitIndex);
			pair<float, pair<AABB, AABB>> minYSAH = findMinSAH(ySAHList, ySplitIndex);
			pair<float, pair<AABB, AABB>> minZSAH = findMinSAH(zSAHList, zSplitIndex);

			List<OBB> leftOBBs;
			List<OBB> rightOBBs;

			if(minXSAH.first < minYSAH.first)
			{
				if(minXSAH.first < minZSAH.first)
				{
					createNewChildren(pRoot, xSplitIndex, xSortedOBBs, leftOBBs, rightOBBs, minXSAH);
				}else
				{
					createNewChildren(pRoot, zSplitIndex, zSortedOBBs, leftOBBs, rightOBBs, minZSAH);
				}
			}else
			{
				if(minYSAH.first < minZSAH.first)
				{
					createNewChildren(pRoot, ySplitIndex, ySortedOBBs, leftOBBs, rightOBBs, minYSAH);
				}else
				{
					createNewChildren(pRoot, zSplitIndex, zSortedOBBs, leftOBBs, rightOBBs, minZSAH);
				}
			}

			if(leftOBBs.size() > 1 && leftOBBs.size() < xSortedOBBs.size())
			{
				construct(pRoot->pLeft, leftOBBs, leftOBBs, leftOBBs);
			}
			if(rightOBBs.size() > 1 && rightOBBs.size() < xSortedOBBs.size())
			{
				construct(pRoot->pRight, rightOBBs, rightOBBs, rightOBBs);
			}
		}

		void BVH::getViewFrustumObjects(
				Node* pNode,
				const ViewFrustum& rFrustum,
				List<SceneObject<IntrusivePtr<BaseMesh>>>& rObjectList,
				map<uint64, uint64>& rGuidCache) const
		{
			if(pNode->pLeft != nullptr)
			{
				if(FrustumOBBIntersection(rFrustum, pNode->pLeft->mAabb) != OUTSIDE)
				{
					getViewFrustumObjects(pNode->pLeft, rFrustum, rObjectList, rGuidCache);
				}
			} 

			if(pNode->pRight != nullptr)
			{
				if(FrustumOBBIntersection(rFrustum, pNode->pRight->mAabb) != OUTSIDE)
				{
					getViewFrustumObjects(pNode->pRight, rFrustum, rObjectList, rGuidCache);
				}
			}
			
			if(pNode->mObjectList.size() > 0)
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

		void BVH::getViewFrustumVolumes(Node* pNode,  
				const ViewFrustum& rFrustum,
				List<OBB>& rResultList,
				map<uint64, uint64>& rGuidCache) const
		{
			if(pNode->pLeft != nullptr)
			{
				if(FrustumOBBIntersection(rFrustum, pNode->pLeft->mAabb) != OUTSIDE)
				{
					getViewFrustumVolumes(pNode->pLeft, rFrustum, rResultList, rGuidCache);
				}
			} 

			if(pNode->pRight != nullptr)
			{
				if(FrustumOBBIntersection(rFrustum, pNode->pRight->mAabb) != OUTSIDE)
				{
					getViewFrustumVolumes(pNode->pRight, rFrustum, rResultList, rGuidCache);
				}
			}
			
			if(pNode->mObjectList.size() > 0)
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
							rResultList.push_back(obb);
							rGuidCache.insert(std::pair<uint64, uint64>(guid, guid));
						}
					}
				}
			}
		}

		void BVH::getLightsData(
				Node* pNode,
				const Vector& rPosition, float radius,
				map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLightsData) const
		{
			if(SphereAABBIntersection(
				rPosition,
				radius,
				pNode->mAabb) == INTERSECT) 
			{
				if(pNode->pLeft != nullptr)
				{
					getLightsData(pNode->pLeft, rPosition, radius, rLightsData);
				}

				if(pNode->pRight != nullptr)
				{
					getLightsData(pNode->pRight, rPosition, radius, rLightsData);
				}

				if(pNode->pLeft == nullptr && pNode->pRight == nullptr)
				{
					ListIterator<SceneObject<BaseLight*>> lightIt = pNode->mLightList.iterator();

					while(lightIt.hasNext())
					{													 
						SceneObject<BaseLight*>& rLightObject =  lightIt.next();
						DampedLight* pDampedLight = (DampedLight*) rLightObject.getSceneObject();

						if(SphereSphereIntersection(
							rPosition,
							radius,
							pDampedLight->getLightPosition(),
							pDampedLight->getAttenuationLaw().MaxAffectedDistance) == INTERSECT)
						{
							map<uint64, pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>::iterator hasLightIt = rLightsData.find(pDampedLight->getGUID());
							pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>> lightData;

							List<SceneObject<IntrusivePtr<BaseMesh>>>::it it = pNode->mObjectList.iterator();
							
							while(it.hasNext())
							{
								SceneObject<IntrusivePtr<BaseMesh>>& rObject = it.next();
								uint64 guid = rObject.getSceneObject()->getGUID();

								map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>::iterator cachedIt 
									=  lightData.second.find(guid);
									
								if(cachedIt == lightData.second.end())
								{
									OBB obb = TransformUtils::transform(
										rObject.getOBB(),
										rObject.getWorldMatrix());

									if(SphereOBBIntersection(
										pDampedLight->getLightPosition(),
										pDampedLight->getAttenuationLaw().MaxAffectedDistance,
										obb) == INTERSECT) 
									{
										if(hasLightIt != rLightsData.end())
										{
											hasLightIt->second.second.insert(
												pair<uint64, SceneObject<IntrusivePtr<BaseMesh>>>(guid, rObject));
										}
										else
										{
											lightData.second.insert(
												pair<uint64, SceneObject<IntrusivePtr<BaseMesh>>>(guid, rObject)); 
										}
									}
								}
							}

							if(hasLightIt == rLightsData.end() && lightData.second.size() > 0)
							{
								lightData.first = rLightObject;

								rLightsData.insert(pair<uint64, pair<SceneObject<BaseLight*>, 
									map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>(pDampedLight->getGUID(), lightData));
							}
						}
					}
				}
			}

		}

		void BVH::insertObject(const SceneObject<IntrusivePtr<BaseMesh>>& rObject, OBB& rOBB, Node* pRoot)
		{
			if(pRoot->pLeft != nullptr)
			{
				if(OverlapOBBOBB(pRoot->pLeft->mAabb, rOBB)) 
				{
					insertObject(rObject, rOBB, pRoot->pLeft);
				}
			}

			if(pRoot->pRight != nullptr)
			{
				if(OverlapOBBOBB(pRoot->pRight->mAabb, rOBB)) 
				{
					insertObject(rObject, rOBB, pRoot->pRight);
				}
			}

			if(pRoot->pRight == nullptr && pRoot->pLeft == nullptr)
			{
				pRoot->mObjectList.push_back(rObject);
			}
		}

		void BVH::insertLight(const SceneObject<BaseLight*>& rLight, Node* pRoot)
		{
			if(pRoot->pLeft != nullptr)
			{
				DampedLight* pLight = (DampedLight*) rLight.getSceneObject();
				if(SphereAABBIntersection(
					pLight->getLightPosition(), 
					pLight->getAttenuationLaw().MaxAffectedDistance,
					pRoot->pLeft->mAabb)) 
				{
					insertLight(rLight, pRoot->pLeft);
				}
			}

			if(pRoot->pRight != nullptr)
			{
				DampedLight* pLight = (DampedLight*) rLight.getSceneObject();
				if(SphereAABBIntersection(
					pLight->getLightPosition(), 
					pLight->getAttenuationLaw().MaxAffectedDistance,
					pRoot->pRight->mAabb)) 
				{
					insertLight(rLight, pRoot->pRight);
				}
			}

			if(pRoot->pRight == nullptr && pRoot->pLeft == nullptr)
			{
				pRoot->mLightList.push_back(rLight);
			}
		}

		void BVH::calculateSAHList(
				List<pair<float, pair<AABB, AABB>>>& rSAHList,
				List<OBB>& rSortedOBBs)
		{
			ListIterator<OBB> it = rSortedOBBs.iterator();
			ListIterator<OBB> begin = rSortedOBBs.iterator();

			uint32 rightCount = rSortedOBBs.size();
			uint32 leftCount = 0;

			while(it.hasNext())
			{
				it.next();

				AABB aabb = BoundingBodyFactory::createAABB(begin, it);
				AABB aabb2 = BoundingBodyFactory::createAABB(it);

				float SAH = leftCount * aabb.getVolume() + rightCount * aabb2.getVolume();

				rSAHList.push_back(pair<float, pair<AABB, AABB>>(SAH, pair<AABB, AABB>(aabb, aabb2)));

				leftCount++;
				rightCount--;
			}
		}

		pair<float, pair<AABB, AABB>> BVH::findMinSAH(
			List<pair<float, pair<AABB, AABB>>>& rSAHList,
			uint32& splitIndex)
		{
			ListIterator<pair<float, pair<AABB, AABB>>> it = rSAHList.iterator();

			pair<float, pair<AABB, AABB>> minSAHPair;
			minSAHPair.first = FLT_MAX;

			uint32 index = 0;
			while(it.hasNext())
			{
				pair<float, pair<AABB, AABB>>& rPair = it.next();

				if(rPair.first <  minSAHPair.first)
				{
					minSAHPair = rPair;
					splitIndex = index;
				}

				index++;
			}

			return minSAHPair;
		}

		void BVH::createNewChildren(
				Node* pRoot,
				uint32 splitIndex,
				List<OBB>& rOBBs,
				List<OBB>& rLeftList,
				List<OBB>& rRightList,
				pair<float, pair<AABB, AABB>>& rMinSAH)
		{
			if(rOBBs.size() > 2)
			{
				ListIterator<OBB> it = rOBBs.iterator();
				uint32 index = 0;

				while(it.hasNext())
				{
					if(index < splitIndex)
					{
						rLeftList.push_back(it.next());
					}
					else
					{
						rRightList.push_back(it.next());
					}

					index++;
				}

				pRoot->pLeft = new Node();
				pRoot->pLeft->pParent = pRoot;
				pRoot->pLeft->pLeft = nullptr;
				pRoot->pLeft->pRight = nullptr;
				pRoot->pLeft->mAabb = rMinSAH.second.first;
					
				pRoot->pRight = new Node();	
				pRoot->pRight->pParent = pRoot;
				pRoot->pRight->pLeft = nullptr;
				pRoot->pRight->pRight = nullptr;
				pRoot->pRight->mAabb = rMinSAH.second.second;
			}
		}

		void BVH::getAABBNodes(List<AABB>& rResultList, Node* pRoot)	const
		{
			if(pRoot->pLeft != nullptr)
			{
				getAABBNodes(rResultList, pRoot->pLeft);
			}

			if(pRoot->pRight != nullptr)
			{
				getAABBNodes(rResultList, pRoot->pRight);
			}

			rResultList.push_back(pRoot->mAabb);
		}
	}
}