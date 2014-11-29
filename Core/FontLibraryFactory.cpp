#include "FontLibraryFactory.h"

namespace OGE
{
	namespace render
	{
		FontLibrary* FontLibraryFactory::loadLibrary(const string& libPath)
		{
			ifstream in(libPath);
			FontLibrary* pFontLib = NULL;

			if(in.is_open())
			{
				
				FontChar* pChars = new FontChar[256];
				ZeroMemory(pChars, sizeof(FontChar) * 256);

				uint16 index = 0;
				
				string texturePath;
				in >> texturePath;

				uint16 countChars = 0;
				in >> countChars;

				while(!in.eof() && index < countChars)
				{
					uint32 code;
					in >> code;
					pChars[code].code = code;
					in >> pChars[code].leftTextureCoord;
					in >> pChars[code].rightTextureCoord;
					in >> pChars[code].size;
					 
					index++;
				}

				IntrusivePtr<Texture2D> pTexture = TextureFactory::createTextureFromFile(texturePath);

				if(!pTexture.isNULL())
				{
					pFontLib = new FontLibrary();
					pFontLib->createLibrary(pChars, IntrusivePtr<Texture2D>(pTexture));
				}
				else
				{
					error_msg("cannot create texture and font library respectively");
				}

				delete[] pChars;
				in.close();
			}
			else
			{
				error_msg("file in not found");	
			}

			return pFontLib;
		}
	}
}