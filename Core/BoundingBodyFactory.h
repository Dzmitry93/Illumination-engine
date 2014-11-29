#ifndef __BB_FACTORY_H__
#define __BB_FACTORY_H__

#include "OBB.h"
#include "AABB.h"
#include "IterableMesh.h"
#include "Format.h"
#include "NumericalMethods.h"
#include "BoundingBodyUtils.h"
#include "SceneObject.h"
#include "List.h"

using namespace OGE::resource;
using namespace OGE::math;

using OGE::Collections::List;
using OGE::Collections::ListIterator;
using OGE::Collections::ConstListIterator;

using OGE::scene::SceneObject;

namespace OGE
{
	namespace util
	{
		class BoundingBodyFactory
		{
		public:
			
			static OBB createOBB(const IterableMesh& rMesh);

			static AABB createAABB(const List<SceneObject<OBB>>& rOBBs);

			static AABB createAABB(ListIterator<OBB> begin, ListIterator<OBB> end);
			static AABB createAABB(ListIterator<OBB> begin);
		};
	}
}

#endif
