#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__

#include "FontLibraryFactory.h"
#include "GraphicsStringFactory.h"
#include "SceneObject.h"
#include "ISystemManager.h"
#include <map>

using OGE::scene::SceneObject;

using std::map;

namespace OGE
{
	namespace render
	{
		class FontManager : public ISystemManager
		{
		public :

			FontManager();
			~FontManager();

			virtual void start();
			virtual void shutdown();

			// set path to font library
			bool loadLibrary(const string& fontLibPath);
	
			// get vb with texture coordinates for msg
			SceneObject<IntrusivePtr<BaseMesh>> getString(int32 posX, int32 posY, const Viewport& viewport, const string& msg);
			IntrusivePtr<FontLibrary> getFontLibrary();

		private:
			map<string, SceneObject<IntrusivePtr<BaseMesh>>> strings;
			IntrusivePtr<FontLibrary> pFontLibrary;
		};

		inline FontManager::FontManager() 
			: pFontLibrary(NULL) {}
		inline FontManager::~FontManager() { shutdown(); }

		inline IntrusivePtr<FontLibrary> FontManager::getFontLibrary()
		{
			return pFontLibrary;
		}

		inline void FontManager::start()
		{
		}

		inline void FontManager::shutdown()
		{
		}
	}
}

#endif
