#ifndef __DAMPED_LIGHT_H__
#define __DAMPED_LIGHT_H__

#include "BaseLight.h"
#include "BaseMesh.h"

using OGE::resource::BaseMesh;

namespace OGE
{
	namespace scene
	{
		class DampedLight  : public BaseLight
		{
		public:

			DampedLight(
				LIGHT_TYPE lightType,
				const Vector& rPosition,
				const ARGBColor& rDiffuseColor,
				const QuadraticAttenuationLaw& rLaw);

			DampedLight(const DampedLight& rOther);

			~DampedLight();

			void setLightPosition(const Vector& rPosition);
			const Vector& getLightPosition() const;

			void setAttenuationLaw(const QuadraticAttenuationLaw& rLaw);
			void setAttenuationLaw(float Kc, float Kl, float Kq);
			const QuadraticAttenuationLaw& getAttenuationLaw() const;

			void operator = (const DampedLight& rOther);

		protected:
			Vector position;
			QuadraticAttenuationLaw attenuationLaw;

			void destroyObject();
			void shallowCopy(const ResourceItem& rOther);
		};

		inline DampedLight::DampedLight(
							LIGHT_TYPE lightType,
							const Vector& rPosition,
							const ARGBColor& rDiffuseColor, 
							const QuadraticAttenuationLaw& rLaw) : 

					BaseLight(lightType, rDiffuseColor),
					position(rPosition), 
					attenuationLaw(rLaw) {}

		inline DampedLight::DampedLight(const DampedLight& rOther) :
			BaseLight(rOther.lightType, rOther.diffuseColor)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline DampedLight::~DampedLight() {}

		inline void DampedLight::destroyObject() {}

		inline void DampedLight::operator = (const DampedLight& rOther)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline void DampedLight::setLightPosition(const Vector& rPosition)
		{
			this->position = rPosition;
		}

		inline const Vector& DampedLight::getLightPosition() const
		{
			return position;
		}

		inline void DampedLight::setAttenuationLaw(const QuadraticAttenuationLaw& rLaw)
		{
			this->attenuationLaw = rLaw;
		}

		inline void DampedLight::setAttenuationLaw(float Kc, float Kl, float Kq)
		{
			this->attenuationLaw.Kc = Kc;
			this->attenuationLaw.Kl = Kl;
			this->attenuationLaw.Kq = Kq;
		}

		inline const QuadraticAttenuationLaw& DampedLight::getAttenuationLaw() const
		{
			return attenuationLaw;
		}

		inline void DampedLight::shallowCopy(const ResourceItem& rOther)
		{
			BaseLight::shallowCopy(rOther);
			DampedLight& rTemp = (DampedLight&) rOther;

			this->position = rTemp.position;
			this->attenuationLaw = rTemp.attenuationLaw;
		}
	}
}

#endif