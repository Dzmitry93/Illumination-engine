#ifndef __ISYSTEM_MANAGER_H__
#define __ISYSTEM_MANAGER_H__

namespace OGE
{
	class ISystemManager
	{
	public:
		virtual void start() = 0;
		virtual void shutdown() = 0;
	};
}

#endif