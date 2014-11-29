#ifndef __RESOURCE_ITEM_H__
#define __RESOURCE_ITEM_H__

/*
=========================================================================

	Base class for any resources that are managed of resource manager

==========================================================================
*/

#include "ReferenceCounter.h"
#include "IComparable.h"
#include "BitFlags.h"
#include <string>

using std::string;
using OGE::memory::ReferenceCounter;

namespace OGE
{
	namespace resource
	{
		class ResourceItem : public ReferenceCounter<ResourceItem>, public IComparable<ResourceItem>
		{
		public:

			ResourceItem();
			virtual ~ResourceItem();

			void setGUID(uint64 GUID);
			uint64 getGUID() const;

			void destroyResource();

			void setResourcePath(const string& resourcePath);
			const string& getResourcePath() const; 

			void setResourceName(const string& rName);
			const string& getResourceName() const;

			u32BitsFlags flags() const;

			virtual uint32 compare(const ResourceItem& rOther) const;

		protected:

			uint64 GUID;	
			string resourcePath;
			string resourceName;
			u32BitsFlags resourceFlags;

			virtual void shallowCopy(const ResourceItem& rOther);
		};

		inline ResourceItem::ResourceItem() : GUID(0), resourcePath("")
		{
			
		}

		inline ResourceItem::~ResourceItem() {}

		inline void ResourceItem::destroyResource() 
		{
			release();
		}

		inline void ResourceItem::setGUID(uint64 GUID)
		{
			this->GUID = GUID;
		}

		inline uint64 ResourceItem::getGUID() const
		{
			return GUID;
		}

		inline void ResourceItem::setResourcePath(const string& resourcePath)
		{
			this->resourcePath = resourcePath;
		}

		inline const string& ResourceItem::getResourcePath() const
		{
			return resourcePath;
		}

		inline void ResourceItem::setResourceName(const string& rName)
		{
			this->resourceName = rName;
		}

		inline const string& ResourceItem::getResourceName() const
		{
			return resourceName;
		}

		inline uint32 ResourceItem::compare(const ResourceItem& rOther) const
		{
			return rOther.GUID == this->GUID;
		}

		inline void ResourceItem::shallowCopy(const ResourceItem& rOther)
		{
			this->GUID = rOther.GUID;
			this->resourcePath = rOther.resourcePath;
			this->resourceName = rOther.resourceName;
			this->resourceFlags = rOther.resourceFlags;
		}
	}
}

#endif

