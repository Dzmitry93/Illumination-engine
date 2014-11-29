#pragma once
#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "BaseLight.h"

namespace OGE
{
	namespace scene
	{
		class DirectionalLight : public BaseLight
		{
		public:
			DirectionalLight(const Vector& rDirection, const ARGBColor& rDiffuseColor);
			DirectionalLight(const DirectionalLight& rOther);
			~DirectionalLight(void);

			void setDirection(const Vector& rDirection);
			Vector getDirection() const;

		private:

			Vector direction;

			void destroyObject();
			void shallowCopy(const ResourceItem& rOther);
		};

		inline DirectionalLight::DirectionalLight(const Vector& rDirection, const ARGBColor& rDiffuseColor) 
			: BaseLight(LIGHT_TYPE_DIRECTIONAL, rDiffuseColor), direction(rDirection) {}

		inline DirectionalLight::DirectionalLight(const DirectionalLight& rOther)
			: BaseLight(LIGHT_TYPE_DIRECTIONAL, rOther.diffuseColor)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline DirectionalLight::~DirectionalLight() {}

		inline void DirectionalLight::destroyObject() {}

		inline void DirectionalLight::setDirection(const Vector& rDirection)
		{
			this->direction = rDirection;
		}

		inline Vector DirectionalLight::getDirection() const
		{
			return direction;
		}

		inline void DirectionalLight::shallowCopy(const ResourceItem& rOther)
		{
			BaseLight::shallowCopy(rOther);
			
			DirectionalLight& rTemp = (DirectionalLight&) rOther;

			this->direction = rTemp.direction;
		}
	}
}

#endif
