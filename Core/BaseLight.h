#ifndef __BASE_LIGHT_H__
#define __BASE_LIGHT_H__

#include "Debug.h"
#include "TypeDefinitions.h"
#include "ARGBColor.h"
#include "ResourceItem.h"

#include "Vector.h"

using OGE::math::Vector;
using OGE::resource::ResourceItem;

namespace OGE
{
	namespace scene
	{
		enum LIGHT_TYPE
		{
			LIGHT_TYPE_POINT = 1,
			LIGHT_TYPE_DIRECTIONAL = 1 << 2,
			LIGHT_TYPE_SPOTLIGHT = 1 << 3,
			LIGHT_TYPE_ALL = 1 << 4
		};

		/* attenuation constants 
				
				quadratic attenuation law - 1 / (Kc + Kl * d + Kq * d * d), 
				where d - is a distance between light source and point in space
		*/
		struct QuadraticAttenuationLaw
		{
			float Kc;
			float Kl;
			float Kq;
			float MaxAffectedDistance;
		};

		class BaseLight : public ResourceItem
		{
		public:
			BaseLight(LIGHT_TYPE lightType);
			BaseLight(LIGHT_TYPE lightType, const ARGBColor& rDiffuseColor);
			~BaseLight();

			LIGHT_TYPE getLightType() const;

			void setDiffuseColor(const ARGBColor& rDiffuseColor);
			ARGBColor getDiffuseColor() const;

		protected:

			LIGHT_TYPE lightType;

			ARGBColor diffuseColor;
			void shallowCopy(const ResourceItem& rOther);
		};

		inline BaseLight::BaseLight(LIGHT_TYPE lightType) : lightType(lightType) {} 
		inline BaseLight::BaseLight(LIGHT_TYPE lightType, const ARGBColor& rDiffuseColor) :
			lightType(lightType), diffuseColor(rDiffuseColor) {}

		inline BaseLight::~BaseLight() {}

		inline LIGHT_TYPE BaseLight::getLightType() const
		{
			return lightType;
		}

		inline void BaseLight::setDiffuseColor(const ARGBColor& rDiffuseColor)
		{
			this->diffuseColor = rDiffuseColor;
		}

		inline ARGBColor BaseLight::getDiffuseColor() const
		{
			return diffuseColor;
		}

		inline void BaseLight::shallowCopy(const ResourceItem& rOther)
		{
			ResourceItem::shallowCopy(rOther);
			BaseLight& rTemp = (BaseLight&) rOther;

			this->diffuseColor = rTemp.diffuseColor;
			this->lightType = rTemp.lightType;
		}
	}
}

#endif