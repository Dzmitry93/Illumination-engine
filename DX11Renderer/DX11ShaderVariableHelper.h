#ifndef __SHADER_VARIABLE_MANAGER_H__
#define __SHADER_VARIABLE_MANAGER_H__

#include "Debug.h"
#include "TypeDefinitions.h"
#include <string.h>

using std::string;

namespace OGE
{
	namespace render
	{
		// Shader variable semantics 
		enum SHADER_VARIABLE_SEMANTIC
		{
			// Matrix variables
			SEMANTIC_WORLD_MATRIX = 0,
			SEMANTIC_VIEW_MATRIX,
			SEMANTIC_PROJECTION_MATRIX,
			SEMANTIC_SCREEN_MATRIX,
			SEMANTIC_WORLD_VIEW__MATRIX,
			SEMANTIC_VIEW_PROJECTION_MATRIX,
			SEMANTIC_WORLD_VIEW_PROJECTION_MATRIX,

			SEMANTIC_LIGHT_DIR,
			SEMANTIC_LIGHT_COLOR,

			SEMANTIC_VIEW_DIR,

			SEMANTIC_MODEL_WORLD_MATRIX,

			COUNT_SEMANTICS
		};

		enum SHADER_CBUFFER
		{
			SHADER_CBUFFER_TRANSFORM,
			SHADER_CBUFFER_CAMERA,
			SHADER_CBUFFER_LIGHTS,
			SHADER_CBUFFER_SHADOW
		};

		class DX11ShaderVariableHelper
		{
		public:
			string getSemanticName(SHADER_VARIABLE_SEMANTIC semantic, uint8 resourceNumber);
			string getSemanticName(SHADER_VARIABLE_SEMANTIC semantic);
			string getCBufferName(SHADER_CBUFFER);
		};
	}
}

#endif

