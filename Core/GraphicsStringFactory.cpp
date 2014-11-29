#include "GraphicsStringFactory.h"

namespace OGE
{
	namespace render
	{
		SceneObject<IntrusivePtr<BaseMesh>> GraphicsStringFactory::createString(int32 posX, int32 posY, 
			const string& msg, const Viewport& viewport, const IntrusivePtr<FontLibrary>& pFontLibrary)
		{
			IntrusivePtr<BaseMesh> pFontVB = MeshUtil::createFontVB(pFontLibrary, msg, viewport);
			Matrix translationMatrix = Matrix::asTranslation((float)posX, (float)posY, 0);

			SceneObject<IntrusivePtr<BaseMesh>> stringObject;
			stringObject.setSceneObject(pFontVB);
			stringObject.setLocalMatrix(translationMatrix);
			stringObject.setWorldMatrix(translationMatrix);

			return stringObject;
		}
	}
}