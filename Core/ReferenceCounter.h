#ifndef __REFERENCE_COUNTER_H__
#define __REFERENCE_COUNTER_H__

#include "Debug.h"
#include "TypeDefinitions.h"

namespace OGE
{
	namespace memory
	{
		template<class T>
		class ReferenceCounter
		{
		public:

			ReferenceCounter();
			~ReferenceCounter();

			void addRef();
			void release();

			uint32 getCountReferences() const;

		protected:
			int32* pCountReferences;

			virtual void destroyObject() = 0;
			virtual void shallowCopy(const T& rhs) = 0;
		};

		template<class T>
		inline ReferenceCounter<T>::ReferenceCounter() 
		{
			pCountReferences = new int32;
			*pCountReferences = 1;
		}

		template<class T>
		inline ReferenceCounter<T>::~ReferenceCounter() 
		{
			if(pCountReferences != NULL && *pCountReferences == 0)
			{
				delete pCountReferences;
				pCountReferences = NULL;
			}
		}

		template<class T>
		inline void ReferenceCounter<T>::addRef()
		{
			debug_assert((*pCountReferences) >= 0, "Incorrect value of count references");
			++(*pCountReferences);
		}

		template<class T>
		inline void ReferenceCounter<T>::release()
		{
			debug_assert((*pCountReferences) > 0, "Incorrect value of count references");
			(*pCountReferences)--;

			if(pCountReferences != NULL && *pCountReferences == 0)
			{
				destroyObject();
			}
		}

		template<class T>
		inline uint32 ReferenceCounter<T>::getCountReferences() const
		{
			return *pCountReferences;
		}
	}
}

#endif
