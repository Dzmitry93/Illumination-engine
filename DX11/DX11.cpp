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
#include "OctreeFactory.h"

using OGE::GraphicsApplication;
using OGE::resource::ResourceManager;
using namespace OGE::render;
using namespace OGE::fileSystem;
using namespace OGE::math;
using namespace OGE::math::IntersectionUtils;
using namespace OGE::input;
using namespace OGE::util;
using namespace OGE;

class DX11 : public GraphicsApplication
{
public :
	virtual void oneTimeGameInit();
	virtual void renderScene();
	virtual void updateInput();
	virtual void updateCamera();
	virtual void updateScene();

private : 
	CameraController* pCameraController;
	List<AABB> aabbList;
	List<SceneObject<IntrusivePtr<BaseMesh>>> renderableList;
};

void DX11::oneTimeGameInit()
{
	IRenderer* pRenderer = new DX11Renderer(PIPELINE_PROCESSOR::RASTERIZER_PIPELINE_PROCESSOR);
	pRenderingManager->setRenderer(pRenderer);
	pRenderingManager->initializeRenderer();

	pCameraController = new CameraController(pInputManager, pSceneManager);
	pCameraController->setStep(5.0f);
	pCameraController->setStepAngle(0.5f);
	string resourcesPath = "../Debug/OGE/scenes/course.dae";
	pSceneManager->setActiveScene(pResourceManager->loadScene(resourcesPath));
}

void DX11::renderScene()
{
	List<pair<SceneObject<BaseLight*>, map<uint64, SceneObject<IntrusivePtr<BaseMesh>>>>> lightData;
	pSceneManager->getViewFrustumLights(lightData);

	pRenderingManager->renderScene(
		pSceneManager->getMeshes(),
		pSceneManager->getBoundingVolumeMeshes(),
		lightData,
		pSceneManager->getActiveCamera());	 		
}												   

void DX11::updateInput()
{
	pInputManager->getKeyboardController()->readKeyboardData();
	pInputManager->getMouserController()->readMouseInput();
}

void DX11::updateCamera()
{
	pCameraController->updateCamera(pSceneManager->getActiveCamera());
}

void DX11::updateScene()
{
}


int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow)
{
	DX11* pApp = new DX11();
	pApp->initialize(hInstance, nCmdShow);
	pApp->oneTimeGameInit();
	pApp->start();
}


