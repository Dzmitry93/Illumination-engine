#include "DX11ShaderVariableHelper.h"

namespace OGE
{
	namespace render
	{
		const char* semantics[] = 
		{
			"World",
			"View",
			"Projection",
			"ScreenMatrix",
			"WorldView",
			"ViewProjection",
			"WorldViewProjection",
			"LightDirection",
			"LightColor",
			"ViewDirection",
			"ModelWorld",
		};

		const char* cbuffer[] = 
		{
			"Transform",
			"Camera",
			"LightParams", 
			"ShadowData"

		};

		string DX11ShaderVariableHelper::getSemanticName(SHADER_VARIABLE_SEMANTIC semanticHandle,
			uint8 resourceNumber)
		{
			string result = semantics[semanticHandle];
			result += resourceNumber + 48;
			
			return result;
		}

		string DX11ShaderVariableHelper::getSemanticName(SHADER_VARIABLE_SEMANTIC semanticHandle)
		{
			string result = semantics[semanticHandle];
			return result;
		}

		string DX11ShaderVariableHelper::getCBufferName(SHADER_CBUFFER cbufferName)
		{
			return cbuffer[cbufferName];
		}
	}
}