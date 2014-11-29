#ifndef __PLANE_TREE_H__
#define __PLANE_TREE_H__

#include "BST.h"
#include "Plane.h"
#include "Comparator.h"
#include "IntrusivePtr.h"
#include <vector>

using std::vector;
using OGE::math::Plane;

namespace OGE
{
	namespace Collections
	{
		struct Interval
		{
			float split;

			uint32 countLeft;
			uint32 countRight;
		};

		template<class T>
		class IntervalTree
		{
		public:

			IntervalTree();
			IntervalTree(float leftBorder, float rightBorder, 
				IObjectObjectComparator<T, Plane, int>* pComparator, COMPARE_AGREEMENT compareTerm);
			~IntervalTree();

			void construct(vector<T>& rData, float rootSplit, float step);
			void copy(IntervalTree<T>& rTree);
			bool find(float split, Interval& rInterval) const;
			void traverse(List<Interval>& rList);

			COMPARE_AGREEMENT getCompareTerm() const;

		private :

			void addNode(vector<T>& rObjects, float split);
			Interval createNewIntervalNode(vector<T>& rObjects, float split);
			
			BST<Interval, float> mIntervalTree;
			COMPARE_AGREEMENT mCompareTerm;

			float mLeftBorder;
			float mRightBorder;
			IObjectObjectComparator<T, Plane, int>* pComparator;
		};

		template<class T>
		inline IntervalTree<T>::IntervalTree()
			: 
			mIntervalTree(new FLoatFloatComparator())
		{
		}

		template<class T>
		inline IntervalTree<T>::IntervalTree(float leftBorder, float rightBorder,
				IObjectObjectComparator<T, Plane, int>* pComparator, COMPARE_AGREEMENT compareTerm) : 
			mIntervalTree(new FLoatComparator())
		{
			this->mLeftBorder = leftBorder;
			this->mRightBorder = rightBorder;
			this->pComparator = pComparator;
			this->mCompareTerm = compareTerm;
		}

		template<class T>
		inline IntervalTree<T>::~IntervalTree() 
		{
		}

		template<class T>
		void IntervalTree<T>::construct(vector<T>& rData, float rootSplit, float step)
		{
			float leftSplit = rootSplit;
			float rightSplit = rootSplit;

			while(leftSplit > mLeftBorder)
			{
				addNode(rData, leftSplit);
				leftSplit -= 2 * step;

				if(leftSplit > mLeftBorder)
				{
					addNode(rData, leftSplit);
				}

				leftSplit += step;
			}

			while(rightSplit < mRightBorder)
			{
				addNode(rData, rightSplit);
				rightSplit += 2 * step;

				if(rightSplit < mRightBorder)
				{
					addNode(rData, rightSplit);
				}

				rightSplit -= step;
			}
		}

		template<class T>
		inline void IntervalTree<T>::addNode(vector<T>& rData, float split)
		{
			Interval interval = createNewIntervalNode(rData, split);
			mIntervalTree.insert(interval, split);
		}

		template<class T>
		inline bool IntervalTree<T>::find(float split, Interval& rInterval) const
		{
			return mIntervalTree.search(split, rInterval);
		}

		template<class T>
		Interval IntervalTree<T>::createNewIntervalNode(vector<T>& rData, float split)
		{
			Plane separatingPlane;
			switch (mCompareTerm)
			{
				case COMPARE_X : separatingPlane = Plane(Vector(split, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f)); break;
				case COMPARE_Y : separatingPlane = Plane(Vector(0.0f, split, 0.0f), Vector(0.0f, 1.0f, 0.0f)); break;
				case COMPARE_Z : separatingPlane = Plane(Vector(0.0f, 0.0f, split), Vector(0.0f, 0.0f, 1.0f)); break;

			default:
				break;
			}

			int compareResult = -1;
			uint32 startIndex = 0;
			uint32 endIndex = rData.size() - 1;
			Interval interval;
			interval.countLeft = 0;
			interval.countRight = 0;
			interval.split = split;

			while(startIndex <= endIndex)
			{
				compareResult = pComparator->compare(rData.at(startIndex), separatingPlane);
				if(compareResult == -1)
				{
					interval.countLeft++;
				}
				if(compareResult == 1)
				{
					interval.countRight++;
				}
				if(compareResult == 0)
				{
					interval.countLeft++;
					interval.countRight++;
				} 
				startIndex++;
			}
			
			// create separating plane node

			return interval;
		}

		template<class T>
		inline COMPARE_AGREEMENT IntervalTree<T>::getCompareTerm() const
		{
			return mCompareTerm;
		}

		template<class T>
		inline void IntervalTree<T>::traverse(List<Interval>& rList)
		{
			mIntervalTree.traverse(rList);
		}

		template<class T>
		void IntervalTree<T>::copy(IntervalTree<T>& rTree)
		{
			mIntervalTree.copy(rTree.mIntervalTree);
			mComparator = rTree.mComparator;
			mCompareTerm = rTree.mCompareTerm;
			mLeftBorder = rTree.mLeftBorder;
			mRightBorder = rTree.mRightBorder;
		}
	}
}

#endif