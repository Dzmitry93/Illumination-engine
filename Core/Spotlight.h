#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#include "DampedLight.h"

namespace OGE
{
	namespace scene
	{
		class Spotlight : public DampedLight
		{
		public:
			Spotlight(const Vector& rPosition, 
				const Vector& rDirection, 
				const ARGBColor& rDiffuseColor, 
				const QuadraticAttenuationLaw& rLaw,
				float falloffAngle,
				float falloffExponent);

			Spotlight(const Spotlight& rOther);
			~Spotlight();

			void setDirection(const Vector& rDirection);
			const Vector& getDirection() const;

			float getFalloffAngle() const;
			float getFalloffExponent() const;

			void operator = (const Spotlight& rOther);

		private:

			Vector direction;

			float falloffAngle;
			float falloffExponent;

			void destroyObject();
			void shallowCopy(const ResourceItem& rOther);
		};
		
		inline Spotlight::Spotlight(const Vector& rPosition, 
							const Vector& rDirection,
							const ARGBColor& rDiffuseColor, 
							const QuadraticAttenuationLaw& rLaw,
							float falloffAngle,
							float falloffExponent) : 
					DampedLight(
						LIGHT_TYPE_SPOTLIGHT,
						rPosition,
						rDiffuseColor,
						rLaw),
					direction(rDirection), 
					falloffAngle(falloffAngle), 
					falloffExponent(falloffExponent) {}

		inline Spotlight::Spotlight(const Spotlight& rOther) :
			DampedLight(
				LIGHT_TYPE_SPOTLIGHT,
				rOther.position,
				rOther.diffuseColor,
				rOther.attenuationLaw)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline Spotlight::~Spotlight() {}

		inline void Spotlight::destroyObject() {}

		inline void Spotlight::operator = (const Spotlight& rOther)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline void Spotlight::setDirection(const Vector& rDirection)
		{
			this->direction = rDirection;
		}

		inline const Vector& Spotlight::getDirection() const
		{
			return direction;
		}

		inline float Spotlight::getFalloffAngle() const
		{
			return falloffAngle;
		}

		inline float Spotlight::getFalloffExponent() const
		{
			return falloffExponent;
		}

		inline void Spotlight::shallowCopy(const ResourceItem& rOther)
		{
			DampedLight::shallowCopy(rOther);
			Spotlight& rTemp = (Spotlight&) rOther;

			this->direction = rTemp.direction;
			this->falloffAngle = rTemp.falloffAngle;
			this->falloffExponent = rTemp.falloffExponent;
		}
	}
}

#endif