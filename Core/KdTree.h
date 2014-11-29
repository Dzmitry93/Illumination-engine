#ifndef __KD_TREE_H__
#define __KD_TREE_H__

/*

TODO : tree destruction
*/

#include "Tree.h"
#include "Format.h"
#include "AABB.h"
#include "OBB.h"
#include "IntrusivePtr.h"
#include "BaseMesh.h"
#include "IntervalTree.h"
#include "Comparator.h"
#include <vector>
#include <list>
#include <algorithm>

using namespace std;
using namespace OGE::math;
using namespace OGE::resource;

namespace OGE
{
	namespace Collections
	{
		template<class T>
		class KdtreeData
		{
		public:
			AABB aabb;
			float split;
			COMPARE_AGREEMENT agr;
			List<T> data;
		};

		template<class T>
		class Kdtree
		{
		public:

			Kdtree(const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxObjectsPerNode, uint32 maxDepth);
			~Kdtree();

			void createKdtree(List<T>&  rData);
			void getAABBList(List<AABB>& rList);
			void getAllAABB(List<AABB>& rList);
			TreeIterator<KdtreeData<T>*> root() const;

		protected:

			void construct(const Vector& rVolume, const Vector& rPosition, 
				TreeIterator<KdtreeData<T>*>& rWhereIt, uint32& depth);

			void getLeavesAABB(TreeIterator<KdtreeData<T>*>& rWhereIt, List<AABB>& rList);
			void separateVolume(const Plane& rSeparatingPlane, 
				const Vector& rVolume, const Vector& rPosition, 
				Vector& rNewVolume, Vector& rNewCenter,
				Vector& rNewVolume2, Vector& rNewCenter2);
			void createAxisSortedData(vector<T>&  rObjects, 
				vector<T>& xObjects, vector<T>& yObjects, vector<T>& zObjects);

			float calculateSAH(const Vector& rVolume, const Vector& rPosition,
				const Plane& separatingPlane, uint32 leftCount, uint32 rightCount);

			void minSAH(const Vector& rVolume, const Vector& rPosition,
				IntervalTree<T>& rTree, float& minSAH, Plane& rPlane);

			virtual IObjectObjectComparator<T, Plane, int>* getComparator(COMPARE_AGREEMENT) = 0;
			virtual void sort(vector<T>& rObjects, COMPARE_AGREEMENT) = 0;

			Tree<KdtreeData<T>*> mTree;

			IObjectObjectComparator<T, Plane, int>* pXComparator;
			IObjectObjectComparator<T, Plane, int>* pYComparator;
			IObjectObjectComparator<T, Plane, int>* pZComparator;

			Vector mVolume;
			Vector mPosition;

			uint32 mMaxObjectsPerNode;
			uint32 mMaxDepth;
			float mStep;
			
			List<pair<float, float>> intersectionStack;
		};

		template<class T>
		inline Kdtree<T>::Kdtree(const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxObjectsPerNode, uint32 maxDepth)
		{
			this->mVolume = rVolume;
			this->mPosition = rPosition;
			this->mStep = step;
			this->mMaxObjectsPerNode = maxObjectsPerNode;
			this->mMaxDepth = maxDepth;
		}

		template<class T>
		inline Kdtree<T>::~Kdtree()
		{
		}

		template<class T>
		TreeIterator<KdtreeData<T>*> Kdtree<T>::root() const
		{
			return mTree.root();
		}

		template<class T>
		inline void Kdtree<T>::createKdtree(List<T>&  rData)
		{
			pXComparator = getComparator(COMPARE_X);
			pYComparator = getComparator(COMPARE_Y);
			pZComparator = getComparator(COMPARE_Z);

			uint32 depth = mMaxDepth;

			KdtreeData<T>* pData = new KdtreeData<T>();
			pData->aabb = AABB(mPosition, mVolume);
			pData->data = rData;

			TreeIterator<KdtreeData<T>*> it = mTree.root();
			it.data() = pData;

			construct(mVolume, mPosition, mTree.root(), depth);
		}

		template<class T>
		void Kdtree<T>::getAllAABB(List<AABB>& rList)
		{
			List<KdtreeData<T>*> nodesList;
			mTree.traverse(PreOrderTraverse<KdtreeData<T>*>(), mTree.root(), nodesList);

			ListIterator<KdtreeData<T>*> it = nodesList.begin();
			ListIterator<KdtreeData<T>*> end = nodesList.end();
			++it;

			while(it != end)
			{
				rList.push_back(it.data()->aabb);
				++it;
			}
		}

		template<class T>
		inline void Kdtree<T>::getAABBList(List<AABB>& rList)
		{
			getLeavesAABB(mTree.root(), rList);
		}

		template<class T>
		void Kdtree<T>::getLeavesAABB(TreeIterator<KdtreeData<T>*>& rWhereIt, List<AABB>& rList)
		{
			TreeIterator<KdtreeData<T>*> it = rWhereIt;
			TreeIterator<KdtreeData<T>*> end = rWhereIt;
			end.backSibling();

			while(it != end)
			{
				it.down();
				getLeavesAABB(it, rList);
				it.up();
				it.nextSibling();
			}
			
			if(it.getCurrentNode()->childs.size() == 0)
			{
				rList.push_back(it.getCurrentNode()->data->aabb);
			}
		}

		template<class T>
		void Kdtree<T>::construct(const Vector& rVolume, const Vector& rPosition,
			TreeIterator<KdtreeData<T>*>& rWhereIt, uint32& depth)
		{
			if(depth > 0)
			{
				ListIterator<T> it = rWhereIt.data()->data.iterator();
				vector<T> rData;

				while(it.hasNext())
				{
					rData.push_back(it.next());
				}

				vector<T> xData;
				vector<T> yData;
				vector<T> zData;

				// create sorted by axis data
				createAxisSortedData(rData, xData, yData, zData);

				// create axis interval binary tree
				IntervalTree<T> xIntervalTree(rPosition.x - rVolume.x, rPosition.x + rVolume.x, pXComparator, COMPARE_X);
				IntervalTree<T> yIntervalTree(rPosition.y - rVolume.y, rPosition.y + rVolume.y, pYComparator, COMPARE_Y);
				IntervalTree<T> zIntervalTree(rPosition.z - rVolume.z, rPosition.z + rVolume.z, pZComparator, COMPARE_Z);

				xIntervalTree.construct(xData, rPosition.x, mStep);
				yIntervalTree.construct(yData, rPosition.y, mStep);
				zIntervalTree.construct(zData, rPosition.z, mStep);

				// finding best separating plane
				float xSAH = FLT_MAX;
				float ySAH = FLT_MAX;
				float zSAH = FLT_MAX;

				Plane xPlane;
				Plane yPlane;
				Plane zPlane;

				KdtreeData<T>* pLeftData = new KdtreeData<T>();
				KdtreeData<T>* pRightData = new KdtreeData<T>();

				minSAH(rVolume, rPosition, xIntervalTree, xSAH, xPlane);
				minSAH(rVolume, rPosition, yIntervalTree, ySAH, yPlane);
				minSAH(rVolume, rPosition, zIntervalTree, zSAH, zPlane);

				Plane separatingPlane;

				if(xSAH < ySAH && xSAH < zSAH) {
					separatingPlane = xPlane;
					Interval interval;
					xIntervalTree.find(-separatingPlane.d, interval);

					for(uint32 i = 0; i < interval.countLeft; ++i)
					{
						pLeftData->data.push_back(xData.at(i));
					}

					for(uint32 i = interval.countRight; i > 0 ; --i)
					{
						pRightData->data.push_back(xData.at(xData.size() - i));
					}
					rWhereIt.getCurrentNode()->data->agr = COMPARE_X;
				}	
				else {
					if(ySAH < zSAH) {
						separatingPlane = yPlane;
						Interval interval;
						yIntervalTree.find(-separatingPlane.d, interval);
						for(uint32 i = 0; i < interval.countLeft; ++i)
						{
							pLeftData->data.push_back(yData.at(i));
						}

						for(uint32 i = interval.countRight; i >0 ; --i)
						{
							pRightData->data.push_back(yData.at(yData.size() - i));
						}
						rWhereIt.getCurrentNode()->data->agr = COMPARE_Y;
					}
					else {
						separatingPlane = zPlane;
						Interval interval;
						zIntervalTree.find(-separatingPlane.d, interval);

						for(uint32 i = 0; i < interval.countLeft; ++i)
						{
							pLeftData->data.push_back(zData.at(i));
						}

						for(uint32 i = interval.countRight; i > 0; --i)
						{
							pRightData->data.push_back(zData.at(zData.size() - i));
						}
						rWhereIt.getCurrentNode()->data->agr = COMPARE_Z;
					}
				}

				rWhereIt.getCurrentNode()->data->split = -separatingPlane.d;

				Vector leftVolume;
				Vector leftCenter;
				Vector rightVolume;
				Vector rightCenter;

				separateVolume(separatingPlane, rVolume, rPosition,
					leftVolume, leftCenter, rightVolume, rightCenter);

				pLeftData->aabb = AABB(leftCenter, leftVolume);
				pRightData->aabb = AABB(rightCenter, rightVolume);

				// insert left node 
				mTree.insert(rWhereIt, pLeftData);

				if(pLeftData->data.size() > mMaxObjectsPerNode)
				{
					depth--;
					rWhereIt.down();
					construct(leftVolume, leftCenter, rWhereIt, depth);
					rWhereIt.up();
					depth++;
				}

				// insert right node
				mTree.insert(rWhereIt, pRightData);

				if(pRightData->data.size() > mMaxObjectsPerNode)
				{
					depth--;
					rWhereIt.down();
					construct(rightVolume, rightCenter, rWhereIt, depth);
					rWhereIt.up();
					depth++;
				}
			}
		}

		template<class T>
		void Kdtree<T>::createAxisSortedData(vector<T>&  rObjects, 
			vector<T>& xObjects, vector<T>& yObjects, vector<T>& zObjects)
		{
			vector<T>::iterator it = rObjects.begin();
			
			while(it != rObjects.end())
			{
				xObjects.push_back(*it);
				yObjects.push_back(*it);
				zObjects.push_back(*it);

				++it;
			}
	
			sort(xObjects, COMPARE_X);
			sort(yObjects, COMPARE_Y);
			sort(zObjects, COMPARE_Z);
		}

		template<class T>
		void Kdtree<T>::minSAH(const Vector& rVolume, const Vector& rPosition, 
			IntervalTree<T>& rTree, float& minSAH, Plane& rPlane)
		{
			COMPARE_AGREEMENT compareTerm  = rTree.getCompareTerm();
		
	/*		float leftBorder;
			float rightBorder;

			switch (compareTerm)
			{
				case OGE::ADT::COMPARE_X: 
						leftBorder = rPosition.x - rVolume.x + mStep; 
						rightBorder = rPosition.x + rVolume.x - mStep;  break;
						
				case OGE::ADT::COMPARE_Y:
						leftBorder = rPosition.y - rVolume.y + mStep; 
						rightBorder = rPosition.y + rVolume.y - mStep; break;

				case OGE::ADT::COMPARE_Z:
						leftBorder = rPosition.z - rVolume.z + mStep; 
						rightBorder = rPosition.z + rVolume.z - mStep;  break;
			default:
				break;
			}
			*/

			List<Interval> intervalList;
			rTree.traverse(intervalList);

			ListIterator<Interval> it = intervalList.iterator();

			while(it.hasNext())
			{
				Interval interval = it.next();
				uint32 leftCount = interval.countLeft;
				uint32 rightCount = interval.countRight;

				Plane plane;

				switch(compareTerm)
				{
					case OGE::Collections::COMPARE_X: 
						plane = Plane(Vector(interval.split, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f));  break;
						
					case OGE::Collections::COMPARE_Y:
						plane = Plane(Vector(0.0f, interval.split, 0.0f), Vector(0.0f, 1.0f, 0.0f));  break;

					case OGE::Collections::COMPARE_Z:
						plane = Plane(Vector(0.0f, 0.0f, interval.split), Vector(0.0f, 0.0f, 1.0f));  break;

					default:
						break;
				}

				float SAH = calculateSAH(rVolume, rPosition, plane, leftCount, rightCount);

				if(SAH < minSAH)
				{
					minSAH = SAH;
					rPlane = plane;
				}
			}
		}

		template<class T>
		float Kdtree<T>::calculateSAH(const Vector& rVolume, const Vector& rPosition, 
			const Plane& rSeparatingPlane, uint32 leftCount, uint32 rightCount)
		{
			float leftSurfaceArea = 0.0f;
			float rightSurfaceArea = 0.0f;
			float split = -rSeparatingPlane.d;

			// calculate left and right volume 
			if(rSeparatingPlane.normal.x >= 1.0f)
			{
				float left = rPosition.x - rVolume.x;
				float right = rPosition.x + rVolume.x;

				leftSurfaceArea = (abs(split - left) * rVolume.y + rVolume.y * rVolume.z + abs(split - left) * rVolume.z) * 2;
				rightSurfaceArea = (abs(right - split) * rVolume.y + rVolume.y * rVolume.z + abs(right - split) * rVolume.y) * 2;
			}

			if(rSeparatingPlane.normal.y >= 1.0f)
			{
				float left = rPosition.y - rVolume.y;
				float right = rPosition.y + rVolume.y;

				leftSurfaceArea = (abs(split - left) * rVolume.z + abs(split - left) * rVolume.x + rVolume.x * rVolume.z) * 2;
				rightSurfaceArea = (abs(right - split) * rVolume.z + abs(right - split) * rVolume.x + rVolume.x * rVolume.z) * 2;
			}

			if(rSeparatingPlane.normal.z >= 1.0f)
			{
				float left = rPosition.z - rVolume.z;
				float right = rPosition.z + rVolume.z;

				leftSurfaceArea = (abs(split - left) * rVolume.x + abs(split - left) * rVolume.y + rVolume.x * rVolume.y) * 2;
				rightSurfaceArea = (abs(right - split) * rVolume.x + abs(right - split)  * rVolume.y + rVolume.x * rVolume.y) * 2;
			} 

			return leftSurfaceArea * leftCount + rightSurfaceArea * rightCount;
		}

		template<class T>
		void Kdtree<T>::separateVolume(const Plane& rSeparatingPlane, 
			const Vector& rVolume, const Vector& rPosition,
			Vector& rNewVolume, Vector& rNewCenter,
			Vector& rNewVolume2, Vector& rNewCenter2)
		{
			if(rSeparatingPlane.normal.x >= 1)
			{
				Vector leftPoint(rPosition.x - rVolume.x, rPosition.y, rPosition.z);
				Vector rightPoint(rPosition.x + rVolume.x, rPosition.y, rPosition.z);
				float leftDistance = rSeparatingPlane.distanceToPoint(leftPoint);
				float rightDistance = rSeparatingPlane.distanceToPoint(rightPoint);

				rNewVolume = rVolume;
				rNewCenter = leftPoint;
				rNewVolume2 = rVolume;
				rNewCenter2 = rightPoint;

				rNewVolume.x = leftDistance / 2.0f;
				rNewCenter.x += rNewVolume.x;

				rNewVolume2.x = rightDistance / 2.0f;
				rNewCenter2.x -= rNewVolume2.x;
			}

			if(rSeparatingPlane.normal.y >= 1)
			{
				Vector leftPoint(rPosition.x, rPosition.y - rVolume.y, rPosition.z);
				Vector rightPoint(rPosition.x, rPosition.y + rVolume.y, rPosition.z);
				float leftDistance = rSeparatingPlane.distanceToPoint(leftPoint);
				float rightDistance = rSeparatingPlane.distanceToPoint(rightPoint);

				rNewVolume = rVolume;
				rNewCenter = leftPoint;
				rNewVolume2 = rVolume;
				rNewCenter2 = rightPoint;

				rNewVolume.y = leftDistance / 2.0f;
				rNewCenter.y += rNewVolume.y;

				rNewVolume2.y = rightDistance / 2.0f;
				rNewCenter2.y -= rNewVolume2.y;
			}

			if(rSeparatingPlane.normal.z >= 1)
			{
				Vector leftPoint(rPosition.x, rPosition.y, rPosition.z  - rVolume.z);
				Vector rightPoint(rPosition.x, rPosition.y, rPosition.z + rVolume.z);
				float leftDistance = rSeparatingPlane.distanceToPoint(leftPoint);
				float rightDistance = rSeparatingPlane.distanceToPoint(rightPoint);

				rNewVolume = rVolume;
				rNewCenter = leftPoint;
				rNewVolume2 = rVolume;
				rNewCenter2 = rightPoint;

				rNewVolume.z = leftDistance / 2.0f;
				rNewCenter.z += rNewVolume.z;

				rNewVolume2.z = rightDistance / 2.0f;
				rNewCenter2.z -= rNewVolume2.z;
			}
		}
	}
}

#endif