#ifndef __GAME_APPLICATION_H__
#define __GAME_APPLICATION_H__

#include "WindowsApplication.h"

namespace OGE
{
	class GraphicsApplication : public WindowsApplication
	{
	public:

		virtual void oneTimeGameInit() = 0;
		virtual void updateCamera() = 0;
		virtual void updateScene() = 0;
		virtual void updateInput() = 0;
		virtual void renderScene() = 0;

	private :
		void performApplicationLogic();
	};

	inline void GraphicsApplication::performApplicationLogic()
	{
		updateInput();
		updateCamera();
		updateScene();
		renderScene();
	}
}

#endif

