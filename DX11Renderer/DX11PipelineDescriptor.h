#ifndef __PIPELINE_DESCRIPTOR_H__
#define __PIPELINE_DESCRIPTOR_H__

#include "BitFlags.h"
#include "TypeDefinitions.h"

using namespace OGE::util;

namespace OGE
{
	namespace render
	{
		class DX11PipelineDescriptor
		{
		public:
			u64BitsFlags lightFlags;
			u64BitsFlags generalFlags;
		};

		enum PIPELINE_LIGHT_STATE
		{
			POINT_LIGHTS = 2,
			DIRECTIONAL_LIGTHS = 4,
			SPOTLIGHTS = 8 
		};

		enum PIPELINE_GENERAL_STATE
		{
			DEFFERED_SHADING_GEOMETRY_PASS = 1,
			DEFFERED_SHADING_LIGHT_PASS = 1 << 2,
			SHADOW_MAPPING_PASS = 1 << 3,
			BOUNDING_BODY = 1 << 4,
			TEXTURE = 1 << 5
		};
	}
}

#endif