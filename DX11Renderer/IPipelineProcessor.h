#ifndef __IPIPELINE_PROCESSOR_H__
#define __IPIPELINE_PROCESSOR_H__

#include "List.h"
#include "SceneObject.h"
#include "Camera.h"
#include "BaseLight.h"

using namespace OGE::Collections;
using namespace OGE::scene;

namespace OGE
{
	namespace render
	{
		class IPipelineProcessor
		{
		public:
			virtual void process(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera) = 0;

			virtual void shutdown() = 0;
		};
	}
}

#endif