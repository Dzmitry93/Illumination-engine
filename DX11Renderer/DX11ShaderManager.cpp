#include "DX11ShaderManager.h"

namespace OGE
{
	namespace render
	{
		string DX11ShaderManager::rootDir = "../Shaders/";
		string DX11ShaderManager::pipelinePath = "DeferredShading.hlsl";
		DX11ShaderManager* DX11ShaderManager::pInstance = NULL;

		uint64 DX11ShaderManager::calculateDescriptorHash(const DX11PipelineDescriptor& rDesc)
		{
			uint64 hash = 
				(rDesc.generalFlags.getFlags() << 1) +
				(rDesc.lightFlags.getFlags() << 2);

			return hash;
		}

		DX11EffectResource* DX11ShaderManager::getPipelineShader(ID3D11Device* pDevice, const DX11PipelineDescriptor& rDesc)
		{
			uint64 descriptorKey = calculateDescriptorHash(rDesc);
			map<uint64, DX11EffectResource*>::iterator it = shadersCache.find(descriptorKey);

			if(it != shadersCache.end())
			{
				return it->second;
			}
			else
			{

				DX11EffectResource* pEffect = effectFactory.compileEffectFromFile(pDevice, rootDir + pipelinePath, rDesc);
				if(pEffect)
				{
					shadersCache.insert(std::pair<uint64, DX11EffectResource*>(descriptorKey, pEffect));
					return pEffect;
				}
				else
				{
					return NULL;
				}
			}
		}

		void DX11ShaderManager::clearCache()
		{
			map<uint64, DX11EffectResource*>::iterator it = shadersCache.begin();
			map<uint64, DX11EffectResource*>::iterator end = shadersCache.end();

			while( it != end)
			{
				it->second->release();
				++it;
			}
		}
	}
}