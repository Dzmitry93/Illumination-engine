#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include "DampedLight.h"

namespace OGE
{
	namespace scene
	{
		class PointLight : public DampedLight
		{
		public:
			PointLight(
				const Vector& rPosition, 
				const ARGBColor& rDiffuseColor,
				const QuadraticAttenuationLaw& rLaw);

			PointLight(const PointLight& rOther);

			~PointLight();
		};

		inline PointLight::PointLight (
				const Vector& rPosition,
				const ARGBColor& rDiffuseColor,
				const QuadraticAttenuationLaw& rLaw) : 

					DampedLight(
						LIGHT_TYPE_POINT, 
						rPosition, 
						rDiffuseColor,
						rLaw)  {}

		inline PointLight::PointLight(const PointLight& rOther) :
			DampedLight(
				LIGHT_TYPE_POINT, 
				rOther.position, 
				rOther.diffuseColor, 
				rOther.attenuationLaw)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline PointLight::~PointLight() {}
	}
}

#endif

