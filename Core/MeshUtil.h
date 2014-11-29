#ifndef __BUFFER_UTIL_H__
#define __BUFFER_UTIL_H__

#include "MeshFactory.h"
#include "MaterialFactory.h"
#include "IntrusivePtr.h"
#include "FontLibrary.h"
#include "RenderingStructures.h"
#include "ViewFrustum.h"
#include "MathUtils.h"
#include "OBB.h"
#include "AABB.h"

using OGE::render::Viewport;
using OGE::memory::IntrusivePtr;
using OGE::render::FontLibrary;
using OGE::render::FontChar;
using OGE::math::IntersectionUtils::PlanesIntersection;
using namespace OGE::math;

namespace OGE
{
	namespace util
	{
		class MeshUtil
		{
		public:

			static BaseMesh* createOBBMesh(const OBB& rOBB);
			static BaseMesh* createAABBMesh(const AABB& rAABB);
			static BaseMesh* createScreenQuad(float zNear);
			static BaseMesh* createViewFrustumMesh(const ViewFrustum& rViewfrustum);
			static BaseMesh* createFontVB(const IntrusivePtr<FontLibrary>& pFontLib, const string& msg, const Viewport& viewport);
		};
	}
}

#endif
