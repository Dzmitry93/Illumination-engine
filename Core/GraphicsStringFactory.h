#ifndef __GRAPHICS_STRING_FACTORY_H__
#define __GRAPHICS_STRING_FACTORY_H__

#include "FontLibrary.h"
#include "MeshUtil.h"
#include "SceneObject.h"

using OGE::scene::SceneObject;
using OGE::util::MeshUtil;

namespace OGE
{
	namespace render
	{
		class GraphicsStringFactory
		{
		public:

			static SceneObject<IntrusivePtr<BaseMesh>> createString(int32 posX, int32 posY, 
				const string& msg, const Viewport& viewport, const IntrusivePtr<FontLibrary>& pFontLibrary);
		};
	}
}
 
#endif
