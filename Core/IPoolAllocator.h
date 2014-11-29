#ifndef __IALLOCATOR_H__
#define __IALLOCATOR_H__

#include "TypeDefinitions.h"

namespace OGE
{
	namespace memory
	{
		class IPoolAllocator
		{
		public:
			virtual void* allocate() = 0;
			virtual void deallocate(void* ptr) = 0;

			virtual uint32 getAllocatedSize() const = 0;
			virtual void destroy() = 0;
		};
	}
}

#endif