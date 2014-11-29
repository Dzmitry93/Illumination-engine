#ifndef __ARRAYITERATOR_H__
#define __ARRAYITERATOR_H__

#include "Iterator.h"
#include "MemoryManager.h"

using OGE::memory::MemoryManager;

namespace OGE
{
	template<class T>
	class ArrayIterator : public Iterator<T>
	{
	public:
		ArrayIterator(T* pIterable, uint32 size);
		~ArrayIterator();
		
		virtual T& next();
		virtual T& previous();

		virtual void front();
		virtual void back();

		virtual T& data();

		virtual bool hasNext() const;
		virtual bool equals(const ArrayIterator<T>& object) const;

		void* operator new(size_t size);
		void operator delete(void* ptr);

	private:
		T* pData;
		uint32 size;
		uint32 current;
	};

	template<class T>
	inline ArrayIterator<T>::ArrayIterator(T* pIterable, uint32 size) :
		size(size), pData(pIterable), current(0) {}

	template<class T>
	inline ArrayIterator<T>::~ArrayIterator() {}

	template<class T>
	inline void* ArrayIterator<T>::operator new(size_t size)
	{
		return MemoryManager::getInstance()->allocateInStack<ArrayIterator<T>>();
	}

	template<class T>
	inline void ArrayIterator<T>::operator delete(void* ptr)
	{
		MemoryManager::getInstance()->deallocateFromStack();
	}

	template<class T>
	inline T& ArrayIterator<T>::next()
	{
		return pData[current++];
	}

	template<class T>
	inline T& ArrayIterator<T>::previous()
	{
		return pData[current--];
	}

	template<class T>
	inline void ArrayIterator<T>::front() 
	{
		current = 0;
	}

	template<class T>
	inline void ArrayIterator<T>::back()
	{
		current = size - 1;
	}

	template<class T>
	inline bool ArrayIterator<T>::hasNext() const
	{
		return current < size;
	}

	template<class T>
	inline bool ArrayIterator<T>::equals(const ArrayIterator<T>& rIterator) const
	{
		return (pData == rIterator.pData) && (current == rIterator.current);
	}

	template<class T>
	inline T& ArrayIterator<T>::data()
	{
		return pData[current];
	}
}

#endif