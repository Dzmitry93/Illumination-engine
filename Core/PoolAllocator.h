#ifndef __POOL_ALLOCATOR_H__
#define __POOL_ALLOCATOR_H__

#include "TypeDefinitions.h"
#include "Debug.h"
#include "IPoolAllocator.h"

#include <xmemory>
#include <list>
#include <queue>
#include <vector>

using std::list;
using std::vector;
using std::priority_queue;
using std::allocator;

namespace OGE
{
	namespace memory
	{
		template<class T>
		struct PoolItem
		{
			T data;
			PoolItem* pPrevious;
			PoolItem* pNext;
		};

		template<class T>
		struct PoolGroup
		{
			PoolItem<T>* pMemoryBlock;
			PoolItem<T>* pNextItem;

			uint32 countItems;
		};

		template<class T>
		struct PoolGroupNode
		{
			PoolGroup<T> group;
			PoolGroupNode<T>* pNext;
			PoolGroupNode<T>* pLast;
		};

		template<class T>
		class PoolGroupComparator
		{
		public:
			bool operator ()(PoolGroup<T> first, PoolGroup<T> second)
			{
				return first.countItems > second.countItems;
			}
		};

		template<class T>
		class PoolAllocator : public allocator<T>, public IPoolAllocator
		{
		public:
			PoolAllocator(const PoolAllocator<T>& allc) { this = allc;}
			PoolAllocator(uint32 itemsPerGroup = 1000);
			~PoolAllocator(void);

			virtual void* allocate();
			virtual void deallocate(void* ptr);

			T* allocate(size_t count, const void * = 0);
			void deallocate(T* ptr, size_t count);
			
			virtual uint32 getAllocatedSize() const;
			virtual void destroy();

			template <class U>
			struct rebind { typedef PoolAllocator<U> other; };

			template <class U>
			PoolAllocator(const PoolAllocator<U>& p) { itemsPerGroup = 1000; pRootGroup = nullptr; createGroup(); }	

		private:

			void createGroup();
			T* allocateItem(PoolGroup<T>& item);
			bool inGroup(PoolGroup<T>& rGroup, uint32 ptrAddresss);

			PoolGroupNode<T>* pRootGroup;
			uint32 itemsPerGroup;
		};
		
		template<class T>
		inline PoolAllocator<T>::PoolAllocator(uint32 itemsPerGroup) : itemsPerGroup(itemsPerGroup), pRootGroup(nullptr)
		{
			createGroup();
		}

		template<class T>
		inline PoolAllocator<T>::~PoolAllocator()
		{
			destroy();
		}

		template<class T>
		inline void* PoolAllocator<T>::allocate() 
		{
			return allocate(0);
		}

		template<class T>
		inline T* PoolAllocator<T>::allocate(size_t count, const void * ptr) 
		{	
			PoolGroup<T>& group = pRootGroup->group;
			if(group.countItems >= itemsPerGroup)
			{
				createGroup();
				return allocateItem(pRootGroup->group);
			}
			return allocateItem(group);
		}

		template<class T>
		inline T* PoolAllocator<T>::allocateItem(PoolGroup<T>& group)
		{
			PoolItem<T>* pMem = group.pNextItem;

			group.pNextItem = group.pNextItem->pNext;
			group.countItems++;

			return &pMem->data;
		}
		
		template<class T>
		inline void PoolAllocator<T>::deallocate(void* ptr)
		{
			deallocate((T*)ptr, 0);
		}

		template<class T>
		inline void PoolAllocator<T>::deallocate(T* ptr, size_t count)
		{
			PoolGroupNode<T>* pFirst = pRootGroup;

			while(pFirst != nullptr)
			{
				uint32 ptrAddress = (uint32) ptr;

				if(inGroup(pFirst->group, ptrAddress))
				{
					break;
				}
				
				pFirst = pFirst->pNext;
			}

			if(pFirst == nullptr)
			{
				return;
			}

			PoolItem<T>* pItem = (PoolItem<T>*) ptr;

			pItem->pNext = pFirst->group.pNextItem;
			pItem->pPrevious = pItem;
			pItem->pNext->pPrevious = pItem;

			pFirst->group.pNextItem = pItem;
			pFirst->group.countItems--;

			if(pFirst->group.countItems == 0)
			{
				if(pFirst->pLast != nullptr)
				{
					pFirst->pLast->pNext = pFirst->pNext;
				}

				if(pFirst->pNext != nullptr)
				{
					pFirst->pNext->pLast = pFirst->pLast;
					if(pFirst == pRootGroup)
					{
						pRootGroup = pFirst->pNext;
					}
				}

				if(pFirst != pRootGroup)
				{
					free(pFirst->group.pMemoryBlock);
					free(pFirst);
				}
			}
		}

		template<class T>
		inline void PoolAllocator<T>::createGroup()
		{
			PoolGroup<T> group;

			group.countItems = 0;
			group.pMemoryBlock = (PoolItem<T>*) malloc(itemsPerGroup * sizeof(PoolItem<T>));
			group.pNextItem = group.pMemoryBlock;

			for(uint32 i = 0; i < itemsPerGroup - 1; i++)
			{
				group.pNextItem[i].pNext = &group.pNextItem[i+1];
				group.pNextItem[i].pNext->pPrevious = &group.pNextItem[i];
			}

			group.pNextItem[0].pPrevious = &group.pNextItem[0];
			group.pNextItem[itemsPerGroup - 1].pNext = &group.pNextItem[itemsPerGroup - 1];

			PoolGroupNode<T>* pNewNode = new PoolGroupNode<T>();
			pNewNode->pNext = pRootGroup;
			pNewNode->pLast = nullptr;
			pNewNode->group = group;

			if(pRootGroup != nullptr)
			{
				pRootGroup->pLast = pNewNode;
			}
			
			pRootGroup = pNewNode;
		}
		
		template<class T>
		inline bool PoolAllocator<T>::inGroup(PoolGroup<T>& rGroup, uint32 ptrAddress)
		{
			uint32 lastAddress = (uint32) &rGroup.pMemoryBlock[itemsPerGroup-1];
			uint32 firstAddress = (uint32) rGroup.pMemoryBlock;

			return ptrAddress >= firstAddress && ptrAddress <= lastAddress;
		}

		template<class T>
		inline uint32 PoolAllocator<T>::getAllocatedSize() const
		{
			PoolGroupNode<T>* pFirst = pRootGroup;
			uint32 size = 0;

			while(pFirst->pNext != nullptr)
			{
				size += pFirst->group.countItems * sizeof(T);
				pFirst = pFirst->pNext;
			}

			return size;
		}

		template<class T>
		void PoolAllocator<T>::destroy() 
		{
			PoolGroupNode<T>* pFirst = pRootGroup;

			while(pFirst->pNext != nullptr)
			{
				delete pFirst->group.pMemoryBlock;
				pFirst = pFirst->pNext;
			}
		}
	}
}

#endif