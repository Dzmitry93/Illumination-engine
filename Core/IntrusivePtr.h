#ifndef __INTRUSIVE_PTR_H__
#define __INTRUSIVE_PTR_H__

/*
==========================================================================================

	Intrusive pointer class. All instances of T must be inheriting Reference Counter class

==========================================================================================
*/

#include "Debug.h"

namespace OGE
{
	namespace memory
	{
		template<class T>
		class IntrusivePtr
		{
		public:
			IntrusivePtr();
			IntrusivePtr(T* pInst);
			IntrusivePtr(const IntrusivePtr<T>& rhs);

			~IntrusivePtr();

			void operator = (const IntrusivePtr<T>& rhs);
			bool operator == (const IntrusivePtr<T>& rhs) const;

			T* operator -> () const;
			T* getData() const;

			bool isNULL() const;

		private:
			T* pInst;
		};

		template<class T>
		inline IntrusivePtr<T>::IntrusivePtr() : pInst(NULL) {}

		template<class T>
		inline IntrusivePtr<T>::IntrusivePtr(T* pInst) : pInst(pInst) 
		{
			if(pInst)
			{
				pInst->addRef();
			}
		}

		template<class T>
		inline IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr<T>& rhs)
		{
			pInst = rhs.pInst;
			if(pInst != NULL)
			{
				pInst->addRef();
			}
		}

		template<class T>
		inline IntrusivePtr<T>::~IntrusivePtr() 
		{
			if(pInst != NULL)
			{
				pInst->release();
			}
		}

		template<class T>
		inline void IntrusivePtr<T>::operator= (const IntrusivePtr<T>& rhs)
		{
			this->pInst = rhs.pInst;
			if(pInst != NULL)
			{
				pInst->addRef();
			}
		}

		template<class T>
		inline T* IntrusivePtr<T>::operator->() const
		{
			return pInst;
		}

		template<class T>
		inline T* IntrusivePtr<T>::getData() const
		{
			return pInst;
		}

		template<class T>
		inline bool IntrusivePtr<T>::isNULL() const
		{
			return pInst == NULL;
		}

		template<class T>
		inline bool IntrusivePtr<T>::operator == (const IntrusivePtr<T>& rhs) const
		{
			return 	pInst == rhs.pInst;
		}
	}
}

#endif