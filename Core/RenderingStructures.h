#ifndef __RENDERING_STRUCTURES_H__
#define __RENDERING_STRUCTURES_H__

namespace OGE
{
	namespace render
	{
		struct Viewport
		{
			float width;
			float height;
			float topLeftX;
			float topLeftY;
			float minDepth;
			float maxDepth;
		};
	}
}

#endif