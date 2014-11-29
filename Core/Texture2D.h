#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "Buffer.h"
#include "ResourceItem.h"

namespace OGE
{
	namespace resource
	{
		class Texture2D : public ResourceItem
		{
		public:
			Texture2D(const Buffer* pBuffer, 
				uint32 width, uint32 height, 
				uint32 bitsPerPixel, uint32 size);
			Texture2D(const Texture2D& rOther);
			~Texture2D(void);

			const Buffer* getTextureData() const;

			uint32 getWidth() const;
			uint32 getHeight() const;
			uint32 getSize() const;
			uint32 getBitsPerPixel() const;

			void operator = (const Texture2D& rOther);
			uint32 operator[] (uint32 i) const;
		private:
			Buffer* pTextureData;

			uint32 width;
			uint32 height;
			uint32 bitsPerPixel;
			uint32 size;

			void destroyObject();
			void shallowCopy(const ResourceItem& rOther);
		};

		inline Texture2D::Texture2D(const Buffer* pBuffer, 
				uint32 width, uint32 height, 
				uint32 bitsPerPixel, uint32 size)
				: width(width), height(height), size(size), bitsPerPixel(bitsPerPixel)
		{
			pTextureData = new Buffer(pBuffer->getSize());
			pBuffer->copy(pTextureData);
		}

		inline Texture2D::Texture2D(const Texture2D& rOther)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline Texture2D::~Texture2D() {}

		inline void Texture2D::operator =(const Texture2D& rOther)
		{
			shallowCopy(rOther);
			addRef();
		}

		inline void Texture2D::destroyObject()
		{
			safe_release(pTextureData);
		}

		inline const Buffer* Texture2D::getTextureData() const
		{
			return pTextureData;
		}

		inline uint32 Texture2D::getWidth() const
		{
			return width;
		}

		inline uint32 Texture2D::getHeight() const
		{
			return height;
		}

		inline uint32 Texture2D::getSize() const
		{
			return size;
		}

		inline uint32 Texture2D::getBitsPerPixel() const
		{
			return bitsPerPixel;
		}

		inline uint32 Texture2D::operator[](uint32 i) const
		{
			if(bitsPerPixel == 8) 
			{
				return ((uint8*) pTextureData->getPointer())[i];
			} else
			if(bitsPerPixel == 16) 
			{
				return ((uint16*) pTextureData->getPointer())[i];
			} else
			if(bitsPerPixel == 32) 
			{
				return ((uint32*) pTextureData->getPointer())[i];
			}
		}

		inline void Texture2D::shallowCopy(const ResourceItem& rOther)
		{
			ResourceItem::shallowCopy(rOther);
			Texture2D& rTemp = (Texture2D&) rOther;

			this->pTextureData = rTemp.pTextureData;
		}
	}
}

#endif
