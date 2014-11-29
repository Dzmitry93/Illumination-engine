#ifndef __KDTREE_FACTORY_H__
#define __KDTREE_FACTORY_H__

#include "TriangleKdtree.h"
#include "SceneObject.h"
#include "MeshFactory.h"
#include "TransformUtils.h"
#include "SceneObject.h"
#include "Camera.h"

using OGE::scene::SceneObject;
using OGE::scene::Camera;
using OGE::util::TransformUtils;

namespace OGE
{
	namespace Collections
	{
		class KdtreeFactory
		{
		public:

			Kdtree<MeshTriangle>* createTrianglesKdtree(
				const List<SceneObject<IntrusivePtr<BaseMesh>>*>& rDataList, const SceneObject<Camera*>& rCamera,
				const Vector& rVolume, const Vector& rPosition,
				float step, uint32 maxDepth, uint32 maxObjectsPerNode);
		};
	}
}

#endif
