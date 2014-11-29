#ifndef __CONST_ITERATOR_H__
#define __CONST_ITERATOR_H__

#include "Object.h"

namespace OGE
{
	template<class T>
	class ConstIterator : public Object
	{
	public:
		virtual const T& next() = 0;
		virtual const T& previous() = 0;

		virtual void front() = 0;
		virtual void back() = 0;

		virtual const T& data() const = 0;

		virtual bool hasNext() const = 0;
	};
}

#endif