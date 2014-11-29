#ifndef __TRIANGLE_KDTREE_H__
#define __TRIANGLE_KDTREE_H__

#include "KdTree.h"
#include "MathUtils.h"

using namespace OGE::math;

namespace OGE
{
	namespace Collections
	{
		struct MeshTriangle 
		{
			IntrusivePtr<Material> pMaterial;
			Vector v1;
			Vector v2;
			Vector v3;
			Vector n1;
			Vector n2;
			Vector n3;
		};

		class TrianglePlaneComparatorX : public IObjectObjectComparator<MeshTriangle, Plane, int>
		{
		public:
			int compare(MeshTriangle  data, Plane plane) const
			{
				bool first = (plane.normal.dotProduct(data.v1.x, 0.0f, 0.0f) + plane.d) >= 0.0f;
				bool second = (plane.normal.dotProduct(data.v2.x, 0.0f, 0.0f) + plane.d) >= 0.0f;
				bool third = (plane.normal.dotProduct(data.v3.x, 0.0f, 0.0f) + plane.d) >= 0.0f;

				bool result = first && second && third;
				if(!result) 
				{
					if(first || second || third) { return 0; }
					else{ return -1; }
				}

				return 1;
			}
		};

		class  TrianglePlaneComparatorY : public IObjectObjectComparator<MeshTriangle, Plane, int>
		{
		public:
			int compare(MeshTriangle  data, Plane plane) const
			{
				bool first = (plane.normal.dotProduct(0.0f, data.v1.y, 0.0f) + plane.d) >= 0.0f;
				bool second = (plane.normal.dotProduct(0.0f, data.v2.y, 0.0f) + plane.d) >= 0.0f;
				bool third = (plane.normal.dotProduct(0.0f, data.v3.y, 0.0f) + plane.d) >= 0.0f;

				bool result = first && second && third;
				if(!result) 
				{
					if(first || second || third) { return 0; }
					else{ return -1; }
				}

				return 1;
			}
		};

		class TrianglePlaneComparatorZ : public IObjectObjectComparator<MeshTriangle, Plane, int>
		{
		public:
			int compare (MeshTriangle  data, Plane plane) const
			{
				bool first = (plane.normal.dotProduct(0.0f, 0.0f, data.v1.z) + plane.d) >= 0.0f;
				bool second = (plane.normal.dotProduct(0.0f, 0.0f, data.v2.z) + plane.d) >= 0.0f;
				bool third = (plane.normal.dotProduct(0.0f, 0.0f, data.v3.z) + plane.d) >= 0.0f;

				bool result = first && second && third;
				if(!result) 
				{
					if(first || second || third) { return 0; }
					else{ return -1; }
				}

				return 1;
			}
		};

		struct IntersectionResult
		{
			MeshTriangle triangle;
			float d;
			float u;
			float v;
			float w;
		};

		class TriangleComparatorX : public binary_function<MeshTriangle, MeshTriangle, bool>
		{
		public:
			bool operator() (MeshTriangle first, MeshTriangle second) 
			{
				return (first.v1.x + first.v2.x + first.v3.x) <
					(second.v1.x + second.v2.x + second.v3.x);
			}
		};
		
		class TriangleComparatorY : public binary_function<MeshTriangle, MeshTriangle, bool>
		{
		public:
			bool operator() (MeshTriangle first, MeshTriangle second) 
			{
				return (first.v1.y + first.v2.y + first.v3.y) < 
					(second.v1.y + second.v2.y + second.v3.y);
			}
		};

		class TriangleComparatorZ : public binary_function<MeshTriangle, MeshTriangle, bool>
		{
		public:
			bool operator() (MeshTriangle first, MeshTriangle second) 
			{
				return (first.v1.z + first.v2.z + first.v3.z) < 
					(second.v1.z + second.v2.z + second.v3.z);
			}
		};

		class TriangleKdtree : public Kdtree<MeshTriangle>
		{
		public:
			TriangleKdtree(const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxObjectsPerNode, uint32 maxDepth);

			void traverseRay(const Vector& rOrigin, const Vector& rDir,  
				IntersectionResult& rResult);
			~TriangleKdtree();

		protected : 
			virtual IObjectObjectComparator<MeshTriangle, Plane, int>* getComparator(COMPARE_AGREEMENT compareTerm);
			void sort(vector<MeshTriangle>& rObjects, COMPARE_AGREEMENT compareTerm);

			void traverseRay(TreeIterator<KdtreeData<MeshTriangle>*>& rWhereIt, 
				const Vector& rOrigin, const Vector& rDir,
				float tnear, float tfar, 
				IntersectionResult& rResult);
		};

		inline TriangleKdtree::TriangleKdtree(const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxObjectsPerNode, uint32 maxDepth) :
					Kdtree(rVolume, rPosition, step, maxObjectsPerNode, maxDepth)
		{
		}

		inline TriangleKdtree::~TriangleKdtree() {}

		inline IObjectObjectComparator<MeshTriangle, Plane, int>* TriangleKdtree::getComparator(COMPARE_AGREEMENT compareTerm)
		{
			switch(compareTerm)
			{
				case COMPARE_X : return new TrianglePlaneComparatorX();
					case COMPARE_Y : return new TrianglePlaneComparatorY();
						case COMPARE_Z : return new TrianglePlaneComparatorZ();

			};

			return false;
		}

		inline void TriangleKdtree::sort(vector<MeshTriangle>& rObjects, COMPARE_AGREEMENT compareTerm)
		{
			switch(compareTerm)
			{
				case COMPARE_X : std::sort(rObjects.begin(), rObjects.end(), TriangleComparatorX()); break;
					case COMPARE_Y : std::sort(rObjects.begin(), rObjects.end(), TriangleComparatorY()); break;
						case COMPARE_Z : std::sort(rObjects.begin(), rObjects.end(), TriangleComparatorZ()); break;
			};
		}
	}
}

#endif