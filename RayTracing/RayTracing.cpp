#include "Texture2D.h"
#include "GraphicsApplication.h"
#include "MathUtils.h"
#include "InputManager.h"
#include "CameraController.h"
#include "StringUtil.h"
#include "FPSMeter.h"
#include "MathUtils.h"
#include "DX11Renderer.h"
#include "KdtreeFactory.h"

using OGE::GraphicsApplication;
using OGE::resource::ResourceManager;
using namespace OGE::render;
using namespace OGE::fileSystem;
using namespace OGE::math;
using namespace OGE::math::IntersectionUtils;
using namespace OGE::input;
using namespace OGE::util;
using namespace OGE;

class RayTracer : public GraphicsApplication
{
public :
	bool oneTimeGameInit();
	bool renderScene();
	bool updateInput();
	bool updateCamera();

private : 
	CameraController* pCameraController;
	DLinkedList<AABB> aabbList;
	DLinkedList<RenderableObject*> renderableList;
};

bool RayTracer::oneTimeGameInit()
{
	IRenderer* pRenderer = new DX11Renderer(PIPELINE_PROCESSOR::RAYTRACING_PIPELINE_PROCESSOR);
	pRenderingManager->setRenderer(pRenderer);
	pRenderingManager->initializeRenderer();

	pCameraController = new CameraController(pInputManager, pSceneManager);
	string resourcesPath = "D:/workspace/projects/OsirisGameEngine/Debug/OGE/Models/room.dae";
	Scene* pScene = pResourceManager->loadScene(resourcesPath);
	pSceneManager->setActiveScene(pScene);

	//pSceneManager->setState(SceneState::drawBB, true); 
	return true;
}

bool RayTracer::renderScene()
{	

	DLinkedList<RenderableObject*> meshes = pSceneManager->getMeshes();
	DLinkedList<SceneObject<BaseLight*>> lights = pSceneManager->getLights();
	SceneObject<Camera*> pCamera = pSceneManager->getActiveCamera();
	
	pRenderingManager->renderScene(meshes, lights, pCamera);
	return true;
}

bool RayTracer::updateInput()
{
	pInputManager->getKeyboardController()->readKeyboardData();
	pInputManager->getMouserController()->readMouseInput();

	return true;
}

bool RayTracer::updateCamera()
{
	pCameraController->updateCamera(pSceneManager->getActiveCamera());
	return true;
}


int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow)
{
	RayTracer* pApp = new RayTracer();
	pApp->initialize(hInstance, nCmdShow);
	pApp->oneTimeGameInit();
	pApp->start();
}
