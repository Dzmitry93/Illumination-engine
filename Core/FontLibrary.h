#ifndef __FONT_LIBRARY_H__
#define __FONT_LIBRARY_H__

#include "Texture2D.h"
#include "IntrusivePtr.h"

using OGE::memory::IntrusivePtr;
using OGE::resource::Texture2D;

namespace OGE
{
	namespace render
	{
		struct FontChar
		{
			// ascii code
			uint32 code;
			// size in pixels
			uint32 size;

			float leftTextureCoord;
			float rightTextureCoord;
		};

		class FontLibrary : public ReferenceCounter<FontLibrary>
		{
		public:
			FontLibrary(void);
			FontLibrary(const FontLibrary& rhs);
			~FontLibrary(void);

			void createLibrary(FontChar* pChars, const IntrusivePtr<Texture2D>& pFontTexture);
			IntrusivePtr<Texture2D> getFontTexture() const;
			const FontChar& getChar(uint8 index) const;

			void operator = (const FontLibrary& rhs);

		private:
			FontChar chars[256];
			IntrusivePtr<Texture2D> pFontTexture;

			void destroyObject();
			void shallowCopy(const FontLibrary& rhs);
		};

		inline FontLibrary::FontLibrary() : pFontTexture(NULL) {}

		inline FontLibrary::FontLibrary(const FontLibrary& rhs) : pFontTexture(NULL)
		{
			shallowCopy(rhs);
			addRef();
		}

		inline FontLibrary::~FontLibrary() 
		{
			destroyObject();
		}

		inline void FontLibrary::destroyObject() {}

		inline void FontLibrary::shallowCopy(const FontLibrary& rhs)
		{
			this->pCountReferences = rhs.pCountReferences;
			this->pFontTexture  = rhs.pFontTexture;

			memcpy((void*)chars, (void*)rhs.chars, sizeof(FontChar) * 256);
		}

		inline void FontLibrary::operator= (const FontLibrary& rhs)
		{
			shallowCopy(rhs);
			addRef();
		}

		inline void FontLibrary::createLibrary(FontChar* pChars, const IntrusivePtr<Texture2D>& pFontTexture)
		{
			memcpy((void*)&chars, (void*)pChars, 256 * sizeof(FontChar));
			this->pFontTexture = pFontTexture;
		}

		inline const FontChar& FontLibrary::getChar(uint8 index) const
		{
			return chars[index];
		}

		inline IntrusivePtr<Texture2D> FontLibrary::getFontTexture() const
		{
			return pFontTexture;
		}
	}
}

#endif