#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "Object.h"

namespace OGE
{
	template<class T>
	class Iterator : public Object
	{
	public:
		virtual T& next() = 0;
		virtual T& previous() = 0;

		virtual void front() = 0;
		virtual void back() = 0;

		virtual T& data() = 0;

		virtual bool hasNext() const = 0;
	};
}

#endif
