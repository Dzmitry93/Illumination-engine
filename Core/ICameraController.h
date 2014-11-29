#ifndef __ICAMERA_CONTROLLER_H__
#define __ICAMERA_CONTROLLER_H__

#include "SceneObject.h"
#include "Camera.h"

namespace OGE
{
	namespace scene
	{
		class ICameraController
		{
			virtual void updateCamera(SceneObject<Camera*>& rCamera);
		};
	}
}

#endif