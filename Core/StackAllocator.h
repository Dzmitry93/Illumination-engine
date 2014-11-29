#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__

#include "Debug.h"
#include "TypeDefinitions.h"

namespace OGE
{
	namespace memory 
	{
		class StackAllocator
		{
		public:
			StackAllocator(uint32 stackSize);
			~StackAllocator(void);

			void deallocate();
			void* allocate(uint32 size);

			uint32 getStackLength() const;

		private:
			uint32 stackLength;
			void* pBase;
			void* pStackTop;

			const static uint32 addressSize = 4;
		};

		inline StackAllocator::StackAllocator(uint32 stackSize) : stackLength(stackSize)
		{
			pStackTop = malloc(stackLength);
			pBase = pStackTop;
		}

		inline StackAllocator::~StackAllocator()
		{
			free(pStackTop);
		}

		inline void StackAllocator::deallocate()
		{
			if((uint32) pStackTop > (uint32) pBase)
			{
				pStackTop = (void*)((uint32) pStackTop - addressSize);
				pStackTop = (void*)(*((uint32*) pStackTop));
			}
		}

		inline void* StackAllocator::allocate(uint32 size) 
		{
			void* pAddress = nullptr;
			if(size > 0)
			{
				if( ((uint32) pStackTop + addressSize + size) <= ((uint32) pStackTop + stackLength))
				{
					pAddress = pStackTop;
					pStackTop = (void*)((uint32) pStackTop + size); 
					*((uint32*) pStackTop) = (uint32) pAddress;   // set adress of previous element 
					pStackTop = (void*)((uint32) pStackTop + addressSize); // set new top
				}
			}
			return pAddress;
		}

		inline uint32 StackAllocator::getStackLength() const
		{
			return stackLength;
		}
	}
}

#endif
