#include "Context.h"

namespace OGE
{	
	bool Context::startManagers(HINSTANCE hInstance, HWND hwnd)
	{
		string libPath = "D:/workspace/projects/OsirisGameEngine/Debug/OGE/font/fontdata.txt";

		// starting managers 
		MemoryManager::getInstance();
		pResourceManager = new ResourceManager();
		pResourceManager->start();
		pSceneManager = new SceneManager();
		pSceneManager->start();
		pFontManager = new FontManager();
		pFontManager->start();
		pRenderingManager = new RenderingManager(hwnd);
		pRenderingManager->start();
		pInputManager = new InputManager();
		pInputManager->start();

		pInputManager->initialize(hInstance, hwnd);
		pFontManager->loadLibrary(libPath);

		return true;
	}

	void Context::shutdownManagers()
	{
		pRenderingManager->shutdown();
		pSceneManager->shutdown();
		pResourceManager->shutdown();
		pInputManager->shutdown();
		pFontManager->shutdown();
	}
}
