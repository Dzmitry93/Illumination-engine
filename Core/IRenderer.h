#ifndef __IRENDERER_H__
#define __IRENDERER_H__

#include "SceneObject.h"
#include "BaseLight.h"
#include "Camera.h"

using namespace OGE::scene;

namespace OGE
{
	namespace render
	{
		class IRenderer
		{
		public:

			virtual bool initialize(HWND hwnd) = 0;

			virtual void renderQueue(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera) = 0;

			virtual void shutdown() = 0;
		};
	}
}

#endif