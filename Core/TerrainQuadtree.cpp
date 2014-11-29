#include "TerrainQuadtree.h"

namespace OGE
{
	namespace terrain
	{
		void TerrainQuadtree::createNodes(TerrainNode* pRoot, uint32 LODLevel)
		{
			if(LODLevel > 0)
			{
				pRoot->hasChildren  = true;

				const AABB& mRootBB = pRoot->mAABB;

				float x = mRootBB.halfDistance.x / 2.0f;
				float y = mRootBB.halfDistance.y / 2.0f;
				float z = mRootBB.halfDistance.z;

				float cx = mRootBB.centerPoint.x;
				float cy = mRootBB.centerPoint.y;
				float cz = mRootBB.centerPoint.z;

				Vector halfD(x, y, z);

				// top left chunk
				pRoot->mChild[0] = new TerrainNode();
				pRoot->mChild[0]->mAABB = AABB(cx - x, cy + y, cz, halfD);

				// bottom left chunk
				pRoot->mChild[1] = new TerrainNode();
				pRoot->mChild[1]->mAABB = AABB(cx - x, cy - y, cz, halfD);

				// bottom right chunk
				pRoot->mChild[2] = new TerrainNode();
				pRoot->mChild[2]->mAABB = AABB(cx + x, cy - y, cz, halfD);

				// top right chunk
				pRoot->mChild[3] = new TerrainNode();
				pRoot->mChild[3]->mAABB = AABB(cx + x, cy + y, cz, halfD);

				createNodes(pRoot->mChild[0], LODLevel-1);
				createNodes(pRoot->mChild[1], LODLevel-1);
				createNodes(pRoot->mChild[2], LODLevel-1);
				createNodes(pRoot->mChild[3], LODLevel-1);
			}
		}

		void TerrainQuadtree::insert(
			const SceneObject<IntrusivePtr<BaseMesh>>& rObject, 
			TerrainNode* pRoot,
			List<TerrainQuadreeNode>::cit it)
		{
			TerrainQuadreeNode value = it.next();

			if(value == TerrainQuadreeNode::NODE_ROOT || !pRoot->hasChildren) 
			{
				pRoot->mChunkData = rObject;	
			}
			else
			{
				if(value == TerrainQuadreeNode::NODE_TOP_LEFT) {
					insert(rObject, pRoot->mChild[0], it);
				}

				if(value == TerrainQuadreeNode::NODE_TOP_RIGHT) {
					insert(rObject, pRoot->mChild[3], it);
				}

				if(value == TerrainQuadreeNode::NODE_BOTTOM_LEFT) {
					insert(rObject, pRoot->mChild[1], it);
				}

				if(value == TerrainQuadreeNode::NODE_BOTTOM_RIGHT) {
					insert(rObject, pRoot->mChild[2], it);
				}
			}		
		}
		
		void TerrainQuadtree::remove(TerrainNode* pRoot, uint64 GUID)
		{
			bool found = false;

			if (pRoot->hasChildren)
			{
				for(uint32 i = 0; i < 4 && !found; i++) 
				{
					if(pRoot->mChild[i]->mChunkData.getSceneObject()->getGUID() == GUID)
					{
						pRoot->mChild[i]->mChunkData = SceneObject<IntrusivePtr<BaseMesh>>();
					}
					else
					{
						remove(pRoot->mChild[i], GUID);
					}
				}
			}
		}
	}
}