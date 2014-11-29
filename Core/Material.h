#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Texture2D.h"
#include "ARGBColor.h"
#include "IntrusivePtr.h"

using OGE::memory::IntrusivePtr;

namespace OGE
{
	namespace resource
	{
		class Material : public ResourceItem
		{
		public:
			Material();
			Material(const Material& rhs);
			~Material();

			void setDiffuseTexture(const IntrusivePtr<Texture2D>& pDiffuseTexture);
			void setNormalTexture(const IntrusivePtr<Texture2D>& pNormalTexture);
			void setSpecular(const ARGBColor& rSpecularColor, float specularPower);

			IntrusivePtr<Texture2D> getDiffuseTexture() const;
			IntrusivePtr<Texture2D> getNormalTexture() const;

			const ARGBColor& getSpecularAlbedo() const;
			float getSpecularPower();

			void operator = (const Material& rhs);

		private:
			ARGBColor specularAlbedo;

			float specularPower;

			IntrusivePtr<Texture2D> pDiffuseTexture;
			IntrusivePtr<Texture2D> pNormalTexture;
			
			void destroyObject();
			void shallowCopy(const ResourceItem& rhs);
		};

		inline Material::Material() {} 

		inline Material::Material(const Material& rhs)
		{
			shallowCopy(rhs);
			addRef();
		}

		inline Material::~Material() { destroyObject(); } 

		inline void Material::destroyObject() {}

		inline void Material::operator = (const Material& rhs)
		{
			shallowCopy(rhs);
			addRef();
		}

		inline void Material::shallowCopy(const ResourceItem& rhs)
		{
			Material& temp = (Material&) rhs;

			this->pCountReferences = temp.pCountReferences;
			this->GUID = temp.GUID;
			this->resourceFlags = temp.resourceFlags;
			this->resourcePath = temp.resourcePath;

			this->specularAlbedo = temp.specularAlbedo;

			this->specularPower = temp.specularPower;

			this->pDiffuseTexture = temp.pDiffuseTexture;
			this->pNormalTexture = temp.pNormalTexture;
		}

		inline void Material::setNormalTexture( const IntrusivePtr<Texture2D>& pNormalTexture)
		{
			this->pNormalTexture = pNormalTexture;
		}

		inline void Material::setDiffuseTexture( const IntrusivePtr<Texture2D>& pDiffuseTexture)
		{
			this->pDiffuseTexture = pDiffuseTexture;
		}

		inline void Material::setSpecular(const ARGBColor& rSpecularAlbedo, float specularPower)
		{
			this->specularAlbedo = rSpecularAlbedo;
			this->specularPower = specularPower;
		}

		inline IntrusivePtr<Texture2D> Material::getDiffuseTexture() const
		{
			return pDiffuseTexture;
		}

		inline IntrusivePtr<Texture2D> Material::getNormalTexture() const
		{
			return pNormalTexture;
		}

		inline const ARGBColor& Material::getSpecularAlbedo() const
		{
			return specularAlbedo;
		}

		inline float Material::getSpecularPower()
		{
			return specularPower;
		}
	}			
}
#endif