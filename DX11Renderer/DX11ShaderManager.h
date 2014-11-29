#ifndef __SHADER_MANAGERS_H__
#define __SHADER_MANAGERS_H__

#include "DX11EffectResource.h"
#include "DX11ComputeResource.h"
#include "DX11PipelineDescriptor.h"
#include "DX11EffectFactory.h"
#include "HashUtil.h"
#include <string>
#include <map>

using OGE::util::HashUtil;
using std::string;
using std::map;

namespace OGE
{
	namespace render
	{
		class DX11ShaderManager
		{
		public:
			static DX11ShaderManager* getInstance();
			
			void setRootDir(const string&  rRootDir);
			void setPipelineShaderName(const string& rPipelineName);

			const string& getRootDir() const;
			const string& getPipelineShaderName() const;

			DX11EffectResource* getPipelineShader(ID3D11Device* pDevice, const DX11PipelineDescriptor& rDesc);
			void clearCache();

		private:
			map<uint64, DX11EffectResource*> shadersCache;

			static string rootDir;
			static string pipelinePath;

			DX11EffectFactory effectFactory;
			uint64 calculateDescriptorHash(const DX11PipelineDescriptor& rDesc);

			static DX11ShaderManager* pInstance;
		};

		inline DX11ShaderManager* DX11ShaderManager::getInstance() 
		{
			if(pInstance == NULL)
			{
				pInstance = new DX11ShaderManager();
			}
			return pInstance;
		}

		inline void DX11ShaderManager::setRootDir(const string& rRootDir)
		{
			this->rootDir = rootDir;
		}

		inline void DX11ShaderManager::setPipelineShaderName(const string& rPipelineName)
		{
			this->pipelinePath = rPipelineName;
		}

		inline const string& DX11ShaderManager::getRootDir() const
		{
			return rootDir;
		}

		inline const string& DX11ShaderManager::getPipelineShaderName() const
		{
			return pipelinePath;
		}
	}
}

#endif