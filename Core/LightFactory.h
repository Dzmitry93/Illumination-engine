#ifndef __LIGHT_FACTORY_H__
#define __LIGHT_FACTORY_H__

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Spotlight.h"
#include "GUIDUtil.h"
#include "HashUtil.h"
#include "ImportStructures.h"

using OGE::fileSystem::OGELightData;
using OGE::fileSystem::OGESpotlight;
using OGE::fileSystem::OGEDirectionalLight;
using OGE::fileSystem::OGEPointLight;

using namespace OGE::util;

namespace OGE
{
	namespace scene
	{
		class LightFactory
		{
		public:

			static BaseLight* createSpotLight(OGELightData* pLight);
			static BaseLight* createSpotLight(const Vector& rPosition, 
												const Vector& rDirection, 
												const ARGBColor& rDiffuseColor, 
												const QuadraticAttenuationLaw& rLaw,
												float falloffAngle, float falloffExponent);

			static BaseLight* createPointLight(OGELightData* pLight);
			static BaseLight* createPointLight(const Vector& rPosition,
													const ARGBColor& rDiffuseColor,
													const QuadraticAttenuationLaw& rLaw);

			static BaseLight* createDirectionalLight(OGELightData* pLight);
			static BaseLight* createDirectionalLight(const Vector& rDirection,
													const ARGBColor& rDiffuseColor);

			static float calculateMaxAffectedDistance(
						const QuadraticAttenuationLaw& rLaw,
						float minAffectedDistance = 0.01f);

		private:
			static string LIGHT;
		};

		
		inline BaseLight* LightFactory::createDirectionalLight(OGELightData* pLight)
		{
			string id = pLight->id;
			uint64 guid = HashUtil::getHash(id);

			OGEDirectionalLight* pOGEDirectionalLight = pLight->pDirectionalLight;
			DirectionalLight* pDirectionalLight = new DirectionalLight(pOGEDirectionalLight->direction, 
				pOGEDirectionalLight->lightColor);

			pDirectionalLight->setGUID(guid);
			pDirectionalLight->setResourceName(id);

			return pDirectionalLight;
		}

		inline BaseLight* LightFactory::createDirectionalLight(const Vector& rDirection,
													const ARGBColor& rDiffuseColor)
		{
			string guidString = LIGHT + GUIDUtil::generateRandomGUID();

			DirectionalLight* pLight = new DirectionalLight(rDirection, rDiffuseColor);
			pLight->setGUID(HashUtil::getHash(guidString));
			pLight->setResourceName(guidString);

			return pLight;
		}


		inline BaseLight* LightFactory::createPointLight(OGELightData* pLight)
		{
			OGEPointLight* pOGEPointLight = pLight->pPointLight;
			string id = pLight->id;
			uint64 guid = HashUtil::getHash(id);

			QuadraticAttenuationLaw law;
			law.Kc = pOGEPointLight->attenuation.constAttenuation;
			law.Kl = pOGEPointLight->attenuation.linearAttenuation;
			law.Kq = pOGEPointLight->attenuation.quadraticAttenuation;
			law.MaxAffectedDistance = calculateMaxAffectedDistance(law);

			PointLight* pPointLight = new PointLight(
				pOGEPointLight->position,
				pOGEPointLight->lightColor,
				law);

			pPointLight->setGUID(guid);
			pPointLight->setResourceName(id);

			return pPointLight;
		}

		inline BaseLight* LightFactory::createPointLight(const Vector& rPosition,
													const ARGBColor& rDiffuseColor,
													const QuadraticAttenuationLaw& rLaw)
		{
			string guidString = LIGHT + GUIDUtil::generateRandomGUID();
			
			QuadraticAttenuationLaw law = rLaw;
			law.MaxAffectedDistance = calculateMaxAffectedDistance(rLaw);

			PointLight* pLight = new PointLight(rPosition, rDiffuseColor, law);
			pLight->setGUID(HashUtil::getHash(guidString));
			pLight->setResourceName(guidString);

			return pLight;
		}

		inline BaseLight* LightFactory::createSpotLight(OGELightData* pLight)
		{
			string id = pLight->id;
			uint64 guid = HashUtil::getHash(id);

			OGESpotlight* pOGESpotlight = pLight->pSpotlight;

			QuadraticAttenuationLaw law;
			law.Kc = pOGESpotlight->attenuation.constAttenuation;
			law.Kl = pOGESpotlight->attenuation.linearAttenuation;
			law.Kq = pOGESpotlight->attenuation.quadraticAttenuation;
			law.MaxAffectedDistance = calculateMaxAffectedDistance(law);

			Spotlight* pSpotlight = new Spotlight(
				pOGESpotlight->position, 
				pOGESpotlight->direction,
				pOGESpotlight->lightColor, 
				law,pOGESpotlight->falloffAngle, 
				pOGESpotlight->falloffExp);

			pSpotlight->setGUID(guid);
			pSpotlight->setResourceName(id);

			return pSpotlight;
		}

		inline BaseLight* LightFactory::createSpotLight(const Vector& rPosition, const Vector& rDirection,
													const ARGBColor& rDiffuseColor, 
													const QuadraticAttenuationLaw& rLaw, 
													float falloffAngle, float falloffExponent)
		{
			string guidString = LIGHT + GUIDUtil::generateRandomGUID();

			QuadraticAttenuationLaw law = rLaw;
			law.MaxAffectedDistance = calculateMaxAffectedDistance(rLaw);

			Spotlight* pLight = new Spotlight(
				rPosition,
				rDirection,
				rDiffuseColor, 
				law, 
				falloffAngle,
				falloffExponent);

			pLight->setGUID(HashUtil::getHash(guidString));
			pLight->setResourceName(guidString);

			return pLight;
		}

		inline float LightFactory::calculateMaxAffectedDistance(
						const QuadraticAttenuationLaw& rLaw,
						float minAttenuation)
		{
			float kc = rLaw.Kc;
			float kl = rLaw.Kl;
			float kq = rLaw.Kq;

			float maxDistance = 0.0f;

			if((kq - 0.0f) > 0.000001)
			{
				maxDistance = (- kl + sqrt(kl * kl - 4 * kq * (kc - 1.0f / minAttenuation ))) / (2 * kq);
			}
			else
			{
				maxDistance = (1.0f / minAttenuation - kc ) / kl;
			}

			debug_error(maxDistance >= 0.0f, "incorrect attenuation law");

			return maxDistance;
		}
	}
}

#endif
