#ifndef __CAMERA_FACTORY_H__
#define __CAMERA_FACTORY_H__

#include "Camera.h"
#include "ImportStructures.h"
#include "HashUtil.h"
#include "GUIDUtil.h"

using namespace OGE::fileSystem;
using namespace OGE::util;

namespace OGE
{
	namespace scene
	{
		class CameraFactory
		{
		public:
			static Camera createCamera(OGECameraData* pCameraData);
		};
	}
}

#endif