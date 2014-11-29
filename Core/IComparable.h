#ifndef __ICOMPARABLE_H__
#define __ICOMPARABLE_H__

#include "TypeDefinitions.h"

namespace OGE
{
	template<class T>
	class IComparable
	{
	public:
		virtual uint32 compare(const T& rOther) const = 0;
	};
}

#endif