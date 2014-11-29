#include "CameraFactory.h"

namespace OGE
{
	namespace scene
	{
		Camera CameraFactory::createCamera(OGECameraData* pCameraData)
		{
			Camera camera(HANDEDNESS_LEFT);
			
			camera.lookAt(pCameraData->cameraPosition, 
				pCameraData->cameraPosition + pCameraData->cameraDirection, pCameraData->upDirection);
			camera.setPerspective(pCameraData->yfov, pCameraData->aspectRatio, 
				pCameraData->znear, pCameraData->zfar);

			string resourceName = pCameraData->id;
			uint64 guid = HashUtil::getHash(resourceName);

			camera.setGUID(guid);
			camera.setResourceName(resourceName);

			return camera;
		}
	}
}