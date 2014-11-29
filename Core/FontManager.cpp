#include "FontManager.h"

namespace OGE
{
	namespace render
	{
		bool FontManager::loadLibrary(const string& fontLibPath)
		{
			pFontLibrary = FontLibraryFactory::loadLibrary(fontLibPath);
			
			if(pFontLibrary.isNULL())
			{
				return false;
			}

			return true;
		}

		SceneObject<IntrusivePtr<BaseMesh>> FontManager::getString(int32 posX, int32 posY, const Viewport& viewport, const string& msg)
		{
			map<string, SceneObject<IntrusivePtr<BaseMesh>>>::iterator it = strings.find(msg);
		
			if(it != strings.end())
			{
				return it->second;
			}
			else
			{
				SceneObject<IntrusivePtr<BaseMesh>> stringObject = 
					GraphicsStringFactory::createString(posX, posY, msg, viewport, pFontLibrary);

				strings.insert(std::pair<string, SceneObject<IntrusivePtr<BaseMesh>>>(msg, stringObject));
				return stringObject;
			}
		}
	}
}
