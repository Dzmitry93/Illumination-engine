#include "DX11EffectFactory.h"

namespace OGE
{
	namespace render
	{
		const char* DX11EffectFactory::LightDefines[] = 
		{
			"POINT_LIGHT\0",
			"DIRECTIONAL_LIGHT\0",
			"SPOTLIGHT\0"
		};

		const char* DX11EffectFactory::GeneralDefines[] =
		{
			"GEOMETRY_PASS\0",
			"LIGHT_PASS\0",
			"BOUNDING_BODY\0",
			"TEXTURE\0",
			"SHADOW_MAP_PASS\0"
		};

		const char* DX11EffectFactory::empty = "1";

		DX11EffectResource* DX11EffectFactory::createEffectFromFile(ID3D11Device* pDevice, const string& path)
		{
			ifstream is(path);
			DX11EffectResource* pEffectResource = NULL;

			if(is.is_open())
			{
			
				ID3DX11Effect* pEffect = NULL;
				void* pEffectBinaryData = NULL;

				// get file size 
				is.seekg(0, ios::end);
				std::streamoff fileSize = is.tellg();
				is.seekg(0, ios::beg);

				debug_assert(fileSize > 0 && fileSize < (1 <<  20), "incorrect size of effect file");
				
				pEffectBinaryData = new uint8[fileSize];
				is.read((char*)pEffectBinaryData, fileSize);
				is.close();
				
				HRESULT result = D3DX11CreateEffectFromMemory(pEffectBinaryData,
					fileSize, 0, pDevice, &pEffect);

				if(FAILED(result))
				{
					error_msg("cannot create effect");
					return NULL;
				}

				pEffectResource = new DX11EffectResource(pEffect);
			}
			else
			{
				error_msg("file not found");
			}

			return pEffectResource;
		}

		DX11EffectResource* DX11EffectFactory::compileEffectFromFile(ID3D11Device* pDevice, 
				const string& path, const DX11PipelineDescriptor& rDesc)
		{
			ifstream is(path, ios::binary);
			DX11EffectResource* pEffectResource = NULL;

			if(is.is_open())
			{
				ID3DX11Effect* pEffect = nullptr;
				void* pEffectBinaryData = nullptr;
				list<D3D_SHADER_MACRO> listShaderMacro;
				D3D_SHADER_MACRO shaderMacro;

				is.seekg(0, ios::end);
				std::streamoff fileSize = is.tellg();
				is.seekg(0, ios::beg);

				debug_assert(fileSize > 0 && fileSize < (1 <<  20), "incorrect size of effect file");
				
				pEffectBinaryData = new uint8[fileSize];
				is.read((char*)pEffectBinaryData, fileSize);
				is.close();

				if(rDesc.generalFlags.testFlag(PIPELINE_GENERAL_STATE::BOUNDING_BODY))
				{
					shaderMacro.Name = GeneralDefines[2];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.generalFlags.testFlag(PIPELINE_GENERAL_STATE::TEXTURE))
				{
					shaderMacro.Name = GeneralDefines[3];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.generalFlags.testFlag(PIPELINE_GENERAL_STATE::DEFFERED_SHADING_GEOMETRY_PASS))
				{
					shaderMacro.Name = GeneralDefines[0];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.generalFlags.testFlag(PIPELINE_GENERAL_STATE::DEFFERED_SHADING_LIGHT_PASS))
				{
					shaderMacro.Name = GeneralDefines[1];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.generalFlags.testFlag(PIPELINE_GENERAL_STATE::SHADOW_MAPPING_PASS))
				{
					shaderMacro.Name = GeneralDefines[4];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.lightFlags.testFlag(PIPELINE_LIGHT_STATE::POINT_LIGHTS))
				{
					shaderMacro.Name = LightDefines[0];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.lightFlags.testFlag(PIPELINE_LIGHT_STATE::DIRECTIONAL_LIGTHS))
				{
					shaderMacro.Name = LightDefines[1];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				if(rDesc.lightFlags.testFlag(PIPELINE_LIGHT_STATE::SPOTLIGHTS))
				{
					shaderMacro.Name = LightDefines[2];
					shaderMacro.Definition = empty;
					listShaderMacro.push_back(shaderMacro);
				}

				LPD3D_SHADER_MACRO pShaderMacro = new D3D_SHADER_MACRO[listShaderMacro.size()+1];
				list<D3D_SHADER_MACRO>::iterator it = listShaderMacro.begin();
				list<D3D_SHADER_MACRO>::iterator end = listShaderMacro.end();
				uint32 macroIndex = 0;

				while(it != end)
				{
					pShaderMacro[macroIndex] = *it;
					macroIndex++;
					++it;
				}

				ZeroMemory(&pShaderMacro[macroIndex], sizeof(D3D_SHADER_MACRO));
	
				ID3DBlob* pCode = NULL;
				ID3DBlob* pError = NULL;

				HRESULT result = D3DCompile(pEffectBinaryData, 
					fileSize, path.c_str(), pShaderMacro, 
					D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0",
					D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
					0, &pCode, &pError); 

				if(pError)
				{
					OutputDebugStringA( (char*)pError->GetBufferPointer() );
					pError->Release();
				}
				if(FAILED(result))
				{
					error_msg("cannot create effect");
					return NULL;
				}
				
				uint32 bufferSize = pCode->GetBufferSize();
				result = D3DX11CreateEffectFromMemory(pCode->GetBufferPointer(), pCode->GetBufferSize(), 0, pDevice, &pEffect);

				if(FAILED(result))
				{
					error_msg("cannot create effect");
					return NULL;
				}

				pEffectResource = new DX11EffectResource(pEffect);
			}
			else
			{
				error_msg("file not found");
			}

			return pEffectResource;
		}

	}
}
