#ifndef __CONST_ARRAY_ITERATOR_H__
#define __CONST_ARRAY_ITERATOR_H__

#include "ConstIterator.h"

namespace OGE
{
	template<class T>
	class ConstArrayIterator : public ConstIterator<T>
	{
	public:
		ConstArrayIterator(T* pIterable, uint32 count);
		~ConstArrayIterator();
		
		virtual const T& next();
		virtual const T& previous();

		virtual void front();
		virtual void back();

		virtual const T& data() const;

		virtual bool hasNext() const;
		virtual bool equals(const ConstArrayIterator<T>& rIterator) const;

		void* operator new(size_t size);
		void operator delete(void* ptr);

	private:
		T* pData;
		uint32 size;
		uint32 current;
	};

	template<class T>
	inline ConstArrayIterator<T>::ConstArrayIterator(T* pIterable, uint32 size) 
		: size(size), pData(pIterable), current(0) {}

	template<class T>
	inline ConstArrayIterator<T>::~ConstArrayIterator() {}

	template<class T>
	inline void* ConstArrayIterator<T>::operator new(size_t size)
	{
		return MemoryManager::getInstance()->allocateInStack<ConstArrayIterator<T>>();
	}

	template<class T>
	inline void ConstArrayIterator<T>::operator delete(void* ptr)
	{
		MemoryManager::getInstance()->deallocateFromStack();
	}

	template<class T>
	inline const T& ConstArrayIterator<T>::next()
	{
		return pData[current++];
	}

	template<class T>
	inline const T& ConstArrayIterator<T>::previous()
	{
		return pData[current--];
	}

	template<class T>
	inline void ConstArrayIterator<T>::front() 
	{
		current = 0;
	}

	template<class T>
	inline void ConstArrayIterator<T>::back()
	{
		current = size - 1;
	}

	template<class T>
	inline bool ConstArrayIterator<T>::hasNext() const
	{
		return current < size;
	}

	template<class T>
	inline bool ConstArrayIterator<T>::equals(const ConstArrayIterator<T>& rIterator) const
	{
		return (pData == rIterator.pData) && (current == rIterator.current);
	}

	template<class T>
	inline const T& ConstArrayIterator<T>::data() const
	{
		return pData[current];
	}
}

#endif