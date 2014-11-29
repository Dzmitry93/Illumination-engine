#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "StringUtil.h"

#include <vector>
#include <list>

using OGE::util::StringUtil;

using std::list;
using std::vector;

namespace OGE
{
	namespace memory 
	{
		class MemoryManager
		{
		public:
			static MemoryManager* getInstance();

			template<class T>
			void* allocateInStack();
			void deallocateFromStack();

			template<class T>
			void* allocateInPool();

			template<class T>
			void deallocateFromPool(T* ptr);

			uint32 getAllocatedMemorySize();
			
		private:

			MemoryManager(void);
			~MemoryManager(void);

			vector<IPoolAllocator*> poolAllocators; 
			StackAllocator* pStackAllocator;

			static MemoryManager* pMemoryManager;

			const uint32 defaultStackAllocatorMemorySize;
			const uint32 defaultPoolGroupMemorySize;
		};
		
		inline MemoryManager* MemoryManager::getInstance() 
		{
			if(pMemoryManager == nullptr)
			{
				pMemoryManager = new MemoryManager();
			}
			return pMemoryManager;
		}
		
		inline MemoryManager::MemoryManager(void) 
			: defaultPoolGroupMemorySize(1 << 20), defaultStackAllocatorMemorySize(1 << 20)
		{
			pStackAllocator = new StackAllocator(defaultStackAllocatorMemorySize);
		}

		inline MemoryManager::~MemoryManager(void)
		{
			safe_release(pStackAllocator);
			vector<IPoolAllocator*>::iterator it = poolAllocators.begin();
			vector<IPoolAllocator*>::iterator end = poolAllocators.end();

			while(it != end)
			{
				(*it)->destroy();
				++it;
			}
		}

		template<class T>
		inline void* MemoryManager::allocateInStack() 
		{
			return pStackAllocator->allocate(sizeof(T));
		}

		inline void MemoryManager::deallocateFromStack()
		{
			pStackAllocator->deallocate();
		}

		template<class T>
		inline void* MemoryManager::allocateInPool()
		{
			uint32 elementSize = sizeof(T);
			IPoolAllocator* pAllocator = nullptr;
			
			if(elementSize < poolAllocators.size()) {
				pAllocator = poolAllocators[elementSize];
			}

			if(pAllocator == nullptr)
			{
				pAllocator = reinterpret_cast<IPoolAllocator*> (new PoolAllocator<T>(defaultPoolGroupMemorySize / elementSize));
				
				if(elementSize > poolAllocators.size()) {
					poolAllocators.resize(elementSize + 1);
				}
				poolAllocators[elementSize] = pAllocator;
			}

			void* pMem = pAllocator->allocate();
			debug_error(pMem != nullptr, "cannot allocate memory");

			return pMem;
		}

		template<class T>
		inline void MemoryManager::deallocateFromPool(T* ptr)
		{
			uint32 elementSize = sizeof(T);
			IPoolAllocator* pAllocator = poolAllocators[elementSize];

			debug_error(pAllocator != nullptr, 
				"cannot find poolAllocator for element size : " + StringUtil::NumberToString((uint64) elementSize));
			pAllocator->deallocate(ptr);
		}
	}
}

#endif
