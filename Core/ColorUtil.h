#ifndef __COLOR_UTIL_H__
#define __COLOR_UTIL_H__

#include "ARGBColor.h"
#include "COLLADAStructures.h"
#include "Vector.h"

using OGE::math::Vector;
using OGE::ARGBColor;
using OGE::fileSystem::CColor;

namespace OGE
{
	namespace colorUtil
	{
		inline ARGBColor FromCOLLADAColor(CColor& color)
		{
			return ARGBColor(color.value[0], color.value[1],
				color.value[2], color.value[3]);
		}
	}
}

#endif

