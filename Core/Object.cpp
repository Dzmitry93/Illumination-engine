#include "Object.h"

namespace OGE
{
	bool Object::equals(const Object& object) const
	{
		if(&object == nullptr)
		{
			return false;
		}

		return this == &object;
	}

	uint64 Object::hashCode() const
	{
		return (uint64)this;
	}
}