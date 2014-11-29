#ifndef __TERRAIN_OCTREE_H__
#define __TERRAIN_OCTREE_H__

#include "AABB.h"
#include "ViewFrustum.h"
#include "SceneObject.h"
#include "DampedLight.h"
#include "MathUtils.h"
#include "List.h"

using OGE::math::AABB;
using OGE::math::ViewFrustum;
using OGE::math::CalculateTangentSpace;

using OGE::scene::SceneObject;
using OGE::scene::DampedLight;

using OGE::Collections::List;

namespace OGE
{
	namespace terrain
	{
		enum TerrainQuadreeNode
		{
			NODE_TOP_LEFT,
			NODE_BOTTOM_LEFT,
			NODE_BOTTOM_RIGHT,
			NODE_TOP_RIGHT,
			NODE_ROOT
		};

		class TerrainQuadtree
		{
		public:
			TerrainQuadtree(const AABB& rRoot, uint32 minLOD);
			~TerrainQuadtree(void);

			void insert(
				const SceneObject<IntrusivePtr<BaseMesh>>& rObject, 
				const List<TerrainQuadreeNode>& rPath);

			void remove(uint64 GUID);

			List<SceneObject<IntrusivePtr<BaseMesh>>> getViewFrustumObjects(const ViewFrustum& rFrustum) const;
			List<SceneObject<IntrusivePtr<BaseMesh>>> getLightVisibleObjects(const DampedLight* pLight) const;

		private:

			struct TerrainNode
			{
				TerrainNode() : hasChildren(false) {}
				~TerrainNode() {}

				AABB mAABB;
				SceneObject<IntrusivePtr<BaseMesh>> mChunkData;
				TerrainNode* mChild[4];	
				bool hasChildren;
			};

			TerrainNode* pRootNode;
			uint32 minLOD;

			void createNodes(TerrainNode* pRoot, uint32 LODLevel);
			void insert(
				const SceneObject<IntrusivePtr<BaseMesh>>& rObject,
				TerrainNode* pRoot,
				List<TerrainQuadreeNode>::cit it);

			void remove(TerrainNode* pRoot, uint64 GUID);
		};		
						
		inline TerrainQuadtree::TerrainQuadtree(const AABB& rRoot, uint32 minLOD)
		{
			this->pRootNode = new TerrainNode();
			this->pRootNode->mAABB = rRoot;
			this->minLOD = minLOD;

			createNodes(pRootNode, minLOD); 
		}

		inline void TerrainQuadtree::insert(
			const SceneObject<IntrusivePtr<BaseMesh>>& rObject, 
			const List<TerrainQuadreeNode>& rPath)
		{
			insert(rObject, pRootNode, rPath.citerator());
		}

		inline void TerrainQuadtree::remove(uint64 GUID)
		{
			remove(pRootNode, GUID);
		}
	}
}

#endif

