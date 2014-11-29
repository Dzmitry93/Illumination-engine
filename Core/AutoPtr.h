#ifndef __AUTO_PTR_H__
#define __AUTO_PTR_H__

namespace OGE
{
	namespace memory
	{
		template<class T>
		class AutoPtr
		{
		public:
			AutoPtr(void);
			AutoPtr(T* pInst);
			~AutoPtr(void);

			T* operator -> () const;
			T* getData() const;

			bool isNULL() const;

		private:
			T* pInstance;
		};

		template<class T>
		inline AutoPtr<T>::AutoPtr(void) : pInstance(NULL) {}

		template<class T>
		inline AutoPtr<T>::AutoPtr(T* pInst ) : pInstance(pInst) {}

		template<class T>
		inline AutoPtr<T>::~AutoPtr()
		{
			if(pInstance)
			{
				delete pInstance;
			}
		}

		template<class T>
		inline T* AutoPtr<T>::operator -> () const
		{
			return pInstance;
		}

		template<class T>
		inline T* AutoPtr<T>::getData() const
		{
			return pInstance;
		}

		template<class T>
		inline bool AutoPtr<T>::isNULL() const
		{
			return pInstance == NULL;
		}
	}
}

#endif
