#include "DX11ComputeFactory.h"

namespace OGE
{
	namespace render
	{
		DX11ComputeResource* DX11ComputeFactory::createComputeResourceFormFile(ID3D11Device* pDevice, const string& filePath)
		{
			ifstream is(filePath, ios::binary);

			if(is.is_open())
			{
				DX11ComputeResource* pComputeResource = NULL;
				ID3D11ComputeShader* pCS = NULL;
				ID3D11ClassLinkage* pClassLinkage = NULL;

				// get file size 
				is.seekg(0, ios::end);
				SIZE_T fileSize = is.tellg(); 
				is.seekg(0, ios::beg);
				
				// load compiled shader in memory
				void* pCSBinaryData = new unsigned char[fileSize];
				is.read((char*)pCSBinaryData, fileSize);
				is.close();
				
				// create compute shader
				pDevice->CreateClassLinkage(&pClassLinkage);
				HRESULT result= pDevice->CreateComputeShader(pCSBinaryData,
					fileSize, pClassLinkage, &pCS);

				if(result == S_OK)
				{
					pComputeResource = new DX11ComputeResource(pCS);
					return pComputeResource;
				}
				else
				{
					error_msg("cannot create compute resource");
					return NULL;
				}
			}
			else
			{
				error_msg("Effect file not found");
				return NULL;
			}
		}
	}
}
