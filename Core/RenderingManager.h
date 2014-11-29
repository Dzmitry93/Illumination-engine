#ifndef __RENDERING_MANAGER_H__
#define __RENDERING_MANAGER_H__

#include "List.h"
#include "Camera.h"
#include "Mesh.h"
#include "BaseLight.h"
#include "IntrusivePtr.h"
#include "IRenderer.h"

using OGE::Collections::List;
using OGE::Collections::ListIterator;
using OGE::resource::Mesh;
using OGE::memory::IntrusivePtr;
using OGE::scene::BaseLight;
using OGE::scene::Camera;

namespace OGE
{
	namespace render
	{
		class RenderingManager
		{
		public:
			RenderingManager(HWND hwnd);
			~RenderingManager();

			void start();
			void shutdown();

			void renderScene(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera);

			void setRenderer(IRenderer* pRenderer);
			void initializeRenderer();
			void shutdownRenderer();

		private:

			IRenderer* pRenderer;
			HWND hwnd;
		};

		inline RenderingManager::RenderingManager(HWND hwnd) : hwnd(hwnd)
		{
		}

		inline RenderingManager::~RenderingManager()
		{
			shutdown();
		}

		inline void RenderingManager::start()
		{
		}

		inline void RenderingManager::shutdown()
		{
			pRenderer->shutdown();
		}

		inline void RenderingManager::setRenderer(IRenderer* pRenderer)
		{
			this->pRenderer = pRenderer;
		}

		inline void RenderingManager::renderScene(
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rQueue, 
				const List<SceneObject<IntrusivePtr<BaseMesh>>>& rPostprocessQueue,
				const List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>>& rLights,
				const SceneObject<Camera>& rCamera)
		{
			pRenderer->renderQueue(
				rQueue,
				rPostprocessQueue,
				rLights,
				rCamera);
		}
		
		inline void RenderingManager::initializeRenderer()
		{
			pRenderer->initialize(hwnd);
		}

		inline void RenderingManager::shutdownRenderer()
		{
			pRenderer->shutdown();
		}
	}
}

#endif

