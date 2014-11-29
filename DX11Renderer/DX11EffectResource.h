#ifndef __EFFECT_RESOURCE_H__
#define __EFFECT_RESOURCE_H__

/*
================================================================

	This class represents wrapper of DirectX effect

================================================================
*/


#include "Matrix.h"
#include "DX11ShaderVariableHelper.h"
#include "DX11Texture2D.h"
#include "DX11RenderResource.h"
#include <d3dx11effect.h>
#include <string>
#include <fstream>

using std::string;
using std::ifstream;
using OGE::math::Matrix;
using OGE::math::Vector;

namespace OGE
{
	namespace render
	{
		#define NOT_HAVE_PASSES 0;

		enum PARAMETER_HANDLE
		{
			AMBIENT_FACTOR,
			DIFFUSE_FACTOR,
			SPECULAR_FACTOR,

			LIGHT_POSITION,
			LIGHT_DIRECTION,
			LIGHT_COLOR,

			VIEW_DIRECTION,
			VIEW_POSITION,
			VIEW_UP,

			COUNT_PARAMETER_HANDLES
		};

		enum EFFECT_ERROR
		{
			EFFECT_NOT_CREATED
		};
		
		class DX11EffectResource : public DX11RenderResource
		{
		public:

			DX11EffectResource(ID3DX11Effect* pEffect);
			~DX11EffectResource();

			void setFloat(const string& name, float value);
			void setVector(const string& sematic, const Vector& v);
			void setVectorByName(const string& name, const Vector& v);
			void setMatrix(const string& semantic, const Matrix& data);
			void setSampler(const string& samplerName, ID3D11SamplerState* pSamplerState);
			void setTexture(const string& textureName, ID3D11ShaderResourceView* pSRV);
			void setCBuffer(const string& cbufferName, ID3D11Buffer* pBuffer);

			ID3DX11Effect* getPointer();

			void release();

		private :
			ID3DX11Effect* pEffect;
		};

		inline DX11EffectResource::DX11EffectResource(ID3DX11Effect* pEffect)
		{
			this->pEffect = pEffect;
		}

		inline DX11EffectResource::~DX11EffectResource()
		{
			release();
		}
		
		inline void DX11EffectResource::release()
		{
			safe_dx_resource_release(pEffect);
		}

		inline ID3DX11Effect* DX11EffectResource::getPointer()
		{
			return pEffect;
		}

		inline void DX11EffectResource::setMatrix(const string& semantic, const Matrix& matrix)
		{
			debug_error(pEffect != NULL, "Effect is not created");

			ID3DX11EffectVariable* pVariable =  pEffect->GetVariableBySemantic(semantic.data());

			const float* pMatrix = (const float*)&(matrix.data);
			HRESULT result = pVariable->AsMatrix()->SetMatrix( pMatrix);
			
			debug_error(!FAILED(result), "cannot set matrix constant in shader"); 			
		}

		inline void DX11EffectResource::setFloat(const string& name, float value)
		{
			debug_error(pEffect != NULL, "Effect is not created");

			ID3DX11EffectVariable* pVariable =  pEffect->GetVariableByName(name.data());
			HRESULT result = pVariable->AsScalar()->SetFloat(value);
			
			debug_error(!FAILED(result), "cannot set float constant in shader");				
		}

		inline void DX11EffectResource::setVector(const string& semantic, const Vector& v)
		{
			debug_error(pEffect != NULL, "Effect is not created");

			ID3DX11EffectVariable* pVariable =  pEffect->GetVariableBySemantic(semantic.data());
			HRESULT result = pVariable->AsVector()->SetFloatVector(v.data);
			
			debug_error(!FAILED(result), "cannot set vector constant in shader");
		}

		inline void DX11EffectResource::setVectorByName(const string& name, const Vector& v)
		{
			debug_error(pEffect != NULL, "Effect is not created");

			ID3DX11EffectVariable* pVariable =  pEffect->GetVariableByName(name.data());
			HRESULT result = pVariable->AsVector()->SetFloatVector(v.data);
			
			debug_error(!FAILED(result), "cannot set vector constant in shader");
		}

		inline void DX11EffectResource::setSampler(const string& samplerName, ID3D11SamplerState* pSamplerState)
		{
			debug_error(pEffect != NULL, "Effect is not created");
			debug_error(pSamplerState != NULL, "sampler state cannot be null");

			ID3DX11EffectVariable* pVariable =  pEffect->GetVariableByName(samplerName.data());
			HRESULT result = pVariable->AsSampler()->SetSampler(0, pSamplerState);
			
			debug_error(!FAILED(result), "cannot set sampler state");
		}

		inline void DX11EffectResource::setTexture(const string& textureName, ID3D11ShaderResourceView* pSRV)
		{
			debug_error(pEffect != NULL, "Effect is not created");
			debug_error(pSRV != NULL, "SRV cannot be null");

			ID3DX11EffectVariable* pVariable = pEffect->GetVariableByName(textureName.data());
			HRESULT result = pVariable->AsShaderResource()->SetResourceArray(&pSRV, 0, 1);
			
			debug_error(!FAILED(result), "cannot set texture");
		}

		inline void DX11EffectResource::setCBuffer(const string& cbufferName, ID3D11Buffer* pBuffer)
		{
			debug_error(pEffect != nullptr, "effect is not created");
			debug_error(pBuffer != nullptr, "constant buffer cannot be null");

			ID3DX11EffectConstantBuffer* pCBuffer = pEffect->GetConstantBufferByName(cbufferName.data());
			HRESULT result = pCBuffer->SetConstantBuffer(pBuffer);

			debug_error(!FAILED(result), "cannot set constant buffer");
		}
	}
}

#endif