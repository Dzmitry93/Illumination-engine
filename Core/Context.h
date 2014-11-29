#ifndef __GAME_HOST_H__
#define __GAME_HOST_H__

/*
=========================================================

	This class is a access point to engine managers

=========================================================
*/

#include "ResourceManager.h"
#include "FontManager.h"
#include "SceneManager.h"
#include "RenderingManager.h"
#include "InputManager.h"
#include "MemoryManager.h"

using OGE::resource::ResourceManager;
using OGE::render::RenderingManager;
using OGE::scene::SceneManager;
using OGE::input::InputManager;
using OGE::memory::MemoryManager;

namespace OGE
{
	class Context
	{
	public:

		SceneManager* getSceneManager();
		ResourceManager* getResourceManager();
		FontManager* getFontManager();
		RenderingManager* getRenderingManager();
		InputManager* getInputManager();
		MemoryManager* getMemoryManager();

	protected :

		SceneManager* pSceneManager;
		ResourceManager* pResourceManager;
		FontManager* pFontManager;
		RenderingManager* pRenderingManager;
		InputManager* pInputManager;
		MemoryManager* pMemoryManager;

		bool startManagers(HINSTANCE hInstance, HWND hwnd); 
		void shutdownManagers();

	};

	inline SceneManager* Context::getSceneManager()
	{
		return pSceneManager;
	}

	inline ResourceManager* Context::getResourceManager()
	{
		return pResourceManager;
	}

	inline FontManager* Context::getFontManager()
	{
		return pFontManager;
	}

	inline MemoryManager* Context::getMemoryManager() 
	{
		return pMemoryManager;
	}
}

#endif