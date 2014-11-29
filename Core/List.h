#ifndef __LIST_H__
#define __LIST_H__

#include "ConstIterator.h"
#include "Iterator.h"
#include "MemoryManager.h"
#include "Comparator.h"

#include <set>

using OGE::Iterator;
using OGE::ConstIterator;

using OGE::memory::MemoryManager;

using std::set;

namespace OGE
{
	namespace Collections
	{
		static MemoryManager* pMemoryManager = MemoryManager::getInstance();

		template<class T>
		class ListNode
		{
		public:
			void* operator new(size_t size);
			void operator delete(void* ptr);

			ListNode<T>* pPrev;
			ListNode<T>* pNext;

			T data;
		};

		template<class T>
		class ListIterator : public Iterator<T>
		{
		public:
			ListIterator() {}
			ListIterator(ListNode<T>* pNode, uint32 size);
			~ListIterator();
			
			virtual T& next();
			virtual T& previous();

			virtual void front();
			virtual void back();

			virtual bool hasNext() const;
			virtual bool equals(const ListIterator<T>& rIterator) const;

			virtual T& data();

			void* operator new(size_t size);
			void operator delete(void* ptr);

			bool isValid() const;
			ListNode<T>* getCurrentNode() const;

		private:
			ListNode<T>* pNode;
			ListNode<T>* pBegin;
			
			uint32 size;
			uint32 current;
		};

		template<class T>
		class ConstListIterator : public ConstIterator<T>
		{
		public:
			ConstListIterator() {}
			ConstListIterator(ListNode<T>* pNode, uint32 size);
			~ConstListIterator();

			virtual const T& next();
			virtual const T& previous();
			
			virtual void front();
			virtual void back();

			virtual bool hasNext() const;
			virtual bool equals(const ConstListIterator<T>& rIterator) const;

			virtual const T& data() const;

			void* operator new(size_t size);
			void operator delete(void* ptr);

			bool isValid() const;

			const ListNode<T>* getCurrentNode() const;

		private:
			ListNode<T>* pNode;
			ListNode<T>* pBegin;
			
			uint32 size;
			uint32 current;
		};

		template<class T>
		class List
		{
		public:

			List();
			List(const List<T>& rhs);
			~List();

			void insert(uint32 pos, const T& value);
			void insertAfter(const ListIterator<T>& it, const T& value);
			void remove(const T& value);
			void erase(uint32 pos);
			void clear();

			void sort(IObjectObjectComparator<T, T, COMPARE_RESULT>* pComparator);

			void push_back(const T& value);
			void push_front(const T& value);

			bool contains(const T& value);

			T pop_back();
			T pop_front();

			const T& front() const;
			const T& back() const;
				
			uint32 size() const;
			bool isEmpty() const;
			
			void operator = (const List<T>& rhs);

			ListIterator<T> iterator();
			ConstListIterator<T> citerator() const;

			typedef ListIterator<T> it;
			typedef ConstListIterator<T> cit;

		private:
			ListNode<T>* pHeadNode;

			uint32 count;

			ListNode<T>* find(const T& value);
			void sort(ListNode<T>* pNode, uint32 count, IObjectObjectComparator<T, T, COMPARE_RESULT>* pComarator);

			void destroy();

			void insert(ListNode<T>& pNode, const T& value);

			T remove(ListNode<T>* pNode);

			void copy(const List<T>& rhs);
		};

		template<class T>
		class UniqueList
		{
		public:
			
			void insert(uint32 pos, const T& value);
			void insertAfter(const ListIterator<T>& it, const T& value);
			void remove(const T& value);
			void erase(uint32 pos);
			void clear();

			void sort(IObjectObjectComparator<T, T, COMPARE_RESULT>* pComparator);

			void push_back(const T& value);
			void push_front(const T& value);

			bool contains(const T& value);

			T pop_back();
			T pop_front();

			const T& front() const;
			const T& back() const;
				
			uint32 size() const;
			bool isEmpty() const;
			
			void operator = (const UniqueList<T>& rhs);

			ListIterator<T> iterator();
			ConstListIterator<T> citerator() const;

			typedef ListIterator<T> it;
			typedef ConstListIterator<T> cit;

		private:
			List<T>	mList;
			set<T> mUniqueKeys;
		};

		template<class T>
		inline List<T>::List()
		{
			pHeadNode = new ListNode<T>();
			pHeadNode->pNext = pHeadNode;
			pHeadNode->pPrev = pHeadNode;

			count = 0;
		}

		template<class T>
		inline List<T>::~List()
		{
			if(count > 0)
			{
				destroy();		
			}
			if(pHeadNode != NULL)
			{
				delete pHeadNode;
			}
		}

		template<class T>
		inline List<T>::List(const List<T>& rhs)
		{
			pHeadNode = new ListNode<T>();
			pHeadNode->pNext = pHeadNode;
			pHeadNode->pPrev = pHeadNode;

			count = 0;
			copy(rhs);
		}

		template<class T>
		inline void List<T>::operator = (const List<T>& rhs)
		{
			copy(rhs);
		}

		template<class T>
		inline void List<T>::copy(const List<T>& rhs)
		{
			if(count > 0)
			{
				destroy();
			}

			ListNode<T>* pBegin = rhs.pHeadNode->pNext;
			ListNode<T>* pEnd = rhs.pHeadNode;

			while(pBegin != pEnd)
			{
				push_back(pBegin->data);
				pBegin = pBegin->pNext;
			} 

			count = rhs.count;
		}

		template<class T>
		inline void List<T>::destroy()
		{
			ListNode<T>* pNode = pHeadNode->pNext;

			while(pNode != pHeadNode)
			{
				ListNode<T>* pTempNode = pNode->pNext;
				delete pNode;
				pNode = pTempNode;
			}

			count = 0;
		}

		template<class T>
		inline void List<T>::clear()
		{
			destroy();
		}

		template<class T>
		inline const T& List<T>::front() const
		{
			return pHeadNode->pNext->data;
		}

		template<class T>
		inline const T& List<T>::back() const
		{
			return pHeadNode->pPrev->data;
		}

		template<class T>
		inline ListIterator<T> List<T>::iterator()
		{
			ListNode<T>* pNode = count > 0 ? pHeadNode->pNext : pHeadNode;
			return ListIterator<T>(pNode, count);
		}

		template<class T>
		inline ConstListIterator<T> List<T>::citerator() const
		{
			ListNode<T>* pNode = count > 0 ? pHeadNode->pNext : pHeadNode;
			return ConstListIterator<T>(pNode, count);
		}

		template<class T>
		inline void List<T>::push_back(const T& value)
		{
			insert(*(pHeadNode->pPrev), value);
		}

		template<class T>
		inline void List<T>::push_front(const T& value)
		{
			insert(*pHeadNode, value);
		}

		template<class T>
		inline T List<T>::pop_back()
		{
			return remove(pHeadNode->pPrev);
		}

		template<class T>
		inline T List<T>::pop_front()
		{
			return remove(pHeadNode->pNext);
		}

		template<class T>
		inline void List<T>::insert(ListNode<T>& pPrevNode, const T& value)
		{
			ListNode<T>* pNewNode = new ListNode<T>();
			pNewNode->data = value;
	
			pNewNode->pNext = pPrevNode.pNext;
			pPrevNode.pNext->pPrev = pNewNode;

			pNewNode->pPrev = &pPrevNode;
			pPrevNode.pNext = pNewNode;

			count++;
		};

		template<class T>
		inline void List<T>::insert(uint32 pos, const T& value)
		{
			if(pHeadNode == pHeadNode->pNext)
			{
				insert(*pHeadNode, value);
			}
			else
			{
				ListNode<T>* pTempNode = pHeadNode;
				for(uint32 i = 0; i < pos; ++i)
				{
					pTempNode = pTempNode->pNext;
				}
				insert(*pTempNode, value);
			}
		}

		template<class T>
		inline void List<T>::insertAfter(const ListIterator<T>& it, const T& value)
		{
			insert(*(it.getCurrentNode()), value);
		}
		
		template<class T>
		inline void List<T>::remove(const T& value)
		{
			ListNode<T>* pBegin = pHeadNode->pNext;
			ListNode<T>* pEnd = pHeadNode;

			for(pBegin; pBegin != pEnd; pBegin = pBegin->pNext)
			{
				if(pBegin->data == value) break;
			}

			if(pBegin->data == value)
			{
				remove(pBegin);
			}
		}

		template<class T>
		inline void List<T>::erase(const uint32 pos)
		{
			ListNode<T>* pNode = pHeadNode;

			for(uint32 i = 0; i < pos; i++)
			{
				pNode = pNode->pNext;
			}

			remove(pNode);
		}
		
		template<class T>
		inline T List<T>::remove(ListNode<T>* pNode)
		{
			T data = pHeadNode->data;
			if(pNode != pHeadNode)
			{
				ListNode<T>* pPrev = pNode->pPrev;
				ListNode<T>* pNext = pNode->pNext;

				pPrev->pNext = pNext;
				pNext->pPrev = pPrev;

				data = pNode->data;
				delete pNode;			
				count--;
			}

			return data;
		}

		template<class T>
		inline uint32 List<T>::size() const 
		{
			return count;
		}

		template<class T>
		inline bool List<T>::isEmpty() const
		{
			return count == 0;
		}

		template<class T>
		inline bool List<T>::contains(const T& value)
		{
			if(find(value) != nullptr) 
			{
				return true;
			}

			return false;
		}

		template<class T>
		ListNode<T>* List<T>::find(const T& value)
		{
			ListIterator<T> it = iterator();

			while(it.hasNext())
			{
				if(it.next().compare(value) == EQUALS)
				{
					return it.getCurrentNode();
				}
			}
		}

		template<class T>
		inline void List<T>::sort(IObjectObjectComparator<T, T, COMPARE_RESULT>* pComparator)
		{
			if(size() > 1)
			{
				sort(pHeadNode->pNext, size(), pComparator);
			}
		}

		template<class T>
		void List<T>::sort(ListNode<T>* pNode, uint32 count, IObjectObjectComparator<T, T, COMPARE_RESULT>* pComparator)
		{
			uint32 i = 0; 
			int32 leftCount = 1;
			int32 rightCount = 0;

			ListNode<T>* pCurrentNode = pNode;
			ListNode<T>* pBegin = nullptr;

			count--;

			while(i < count)
			{
				pCurrentNode = pCurrentNode->pNext;

				if(pComparator->compare(pCurrentNode->data, pNode->data) == COMPARE_RESULT::GREATER)
				{
					ListNode<T>* pTemp = pCurrentNode->pPrev;

					pCurrentNode->pPrev->pNext = pCurrentNode->pNext;
					pCurrentNode->pNext->pPrev = pCurrentNode->pPrev;

					pNode->pPrev->pNext = pCurrentNode;
					pCurrentNode->pPrev	= pNode->pPrev;
					pCurrentNode->pNext = pNode;
					pNode->pPrev = pCurrentNode;

					if(pBegin == nullptr)
					{
						pBegin = pCurrentNode;
					}

					pCurrentNode = pTemp;

					leftCount++;

				}

				i++;
			}

			rightCount = count - leftCount + 1; 

			if(leftCount > 1)
			{
				sort(pBegin, leftCount, pComparator);
			}
			if(rightCount > 1)
			{
				sort(pNode->pNext, rightCount, pComparator);
			}
		}

		template<class T>
		inline void* ListNode<T>::operator new(size_t size)
		{
			return pMemoryManager->allocateInPool<ListNode<T>>();
		}

		template<class T>
		inline void ListNode<T>::operator delete(void* ptr)
		{
			pMemoryManager->deallocateFromPool<ListNode<T>>((ListNode<T>*) ptr);
		}

		/*
		===============================================================================
		ListIterator implementation
		===============================================================================
		*/

		template<class T>
		inline ListIterator<T>::ListIterator(ListNode<T>* pNode, uint32 size) :
			size(size), pNode(pNode), pBegin(pNode), current(0)
		{
		}

		template<class T>
		inline ListIterator<T>::~ListIterator(){}
		
		template<class T>
		inline T& ListIterator<T>::next()
		{
			current++;
			T& data = pNode->data;
			pNode = pNode->pNext;
			return data;
		}

		template<class T>
		inline T& ListIterator<T>::previous()
		{
			T& data = pNode->data;
			pNode = pNode->pPrev;
			current--;

			return data;
		}

		template<class T>
		inline void ListIterator<T>::front()
		{
			current = 0;
			pNode = pBegin;
		}

		template<class T>
		inline void ListIterator<T>::back()
		{
			current = size - 1;
			pNode = pBegin->pPrev;
		}

		template<class T>
		inline bool ListIterator<T>::hasNext() const
		{
			return current < size;
		}

		template<class T>
		inline bool ListIterator<T>::equals(const ListIterator<T>& rhs) const
		{
			return this->pNode == rhs.pNode;
		}

		template<class T>
		inline T& ListIterator<T>::data() 
		{
			return pNode->data;
		}

		template<class T>
		void* ListIterator<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInStack<ListIterator<T>>();
		}

		template<class T>
		void ListIterator<T>::operator delete(void* ptr)
		{
			return MemoryManager::getInstance()->deallocateFromStack(); 
		}

		template<class T>
		inline ListNode<T>* ListIterator<T>::getCurrentNode() const
		{
			return pNode;
		}
		
		template<class T>
		inline bool ListIterator<T>::isValid() const
		{
			return pNode != NULL;
		}

		/*
		===============================================================================
		ConstListIterator implementation
		===============================================================================
		*/

		template<class T>
		inline ConstListIterator<T>::ConstListIterator(ListNode<T>* pNode, uint32 size) :
			pNode(pNode), pBegin(pNode), size(size), current(0)
		{
		}

		template<class T>
		inline ConstListIterator<T>::~ConstListIterator(){}

		template<class T>
		inline const T& ConstListIterator<T>::next()
		{
			current++;
			T& data = pNode->data;
			pNode = pNode->pNext;
			return data;
		}

		template<class T>
		const T& ConstListIterator<T>::previous()
		{
			current--;
			T& data = pNode->data;
			pNode = pNode->pPrev;
			return data;
		}

		template<class T>
		inline void ConstListIterator<T>::front()
		{
			current = 0;
			pNode = pBegin;
		}

		template<class T>
		inline void ConstListIterator<T>::back()
		{
			current = size - 1;
			pNode = pBegin->pPrev;
		}

		template<class T>
		inline bool ConstListIterator<T>::hasNext() const
		{
			return current < size ;
		}

		template<class T>
		inline const T& ConstListIterator<T>::data() const
		{
			return pNode->data;
		}

		template<class T>
		inline bool ConstListIterator<T>::equals(const ConstListIterator<T>& rhs) const
		{
			return this->pNode == rhs.pNode;
		}

		template<class T>
		inline void* ConstListIterator<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInStack<ConstListIterator<T>>();
		}

		template<class T>
		inline void ConstListIterator<T>::operator delete(void* ptr)
		{
			MemoryManager::getInstance()->deallocateFromStack();
		}

		template<class T>
		inline const ListNode<T>* ConstListIterator<T>::getCurrentNode() const
		{
			return pNode;
		}
		
		template<class T>
		inline bool ConstListIterator<T>::isValid() const
		{
			return pNode != NULL;
		}
	}
}
#endif