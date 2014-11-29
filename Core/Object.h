#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "TypeDefinitions.h"
#include "Debug.h"

namespace OGE
{
	class Object 
	{
	public:
		virtual bool equals(const Object& object) const;
		virtual uint64 hashCode() const;
	};
}

#endif