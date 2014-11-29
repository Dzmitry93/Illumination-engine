#ifndef __DX11RENDER_RESOURCE_H__
#define __DX11RENDER_RESOURCE_H__

#include "Debug.h"
#include "TypeDefinitions.h"

namespace OGE
{
	namespace render
	{
		class DX11RenderResource
		{
		public:
			DX11RenderResource();
			DX11RenderResource(const string& rName, uint64 guid);
			~DX11RenderResource();

			void setGUID(uint64 GUID);
			uint64 getGUID() const;

			void setResourceName(const string& rName);
			const string& getResourceName() const;

			virtual void release() = 0;

		private:

			uint64 guid;
			string resourceName;
		};

		inline DX11RenderResource::DX11RenderResource() 
			: resourceName(""), guid(0)
		{
		}

		inline DX11RenderResource::DX11RenderResource(const string& rName, uint64 guid)
			: resourceName(rName), guid(guid)
		{
		}

		inline DX11RenderResource::~DX11RenderResource() {}

		inline void DX11RenderResource::setGUID(uint64 guid)
		{
			this->guid = guid;
		}

		inline void DX11RenderResource::setResourceName(const string& rName)
		{
			this->resourceName = rName;
		}

		inline uint64 DX11RenderResource::getGUID() const
		{
			return guid;
		}

		inline const string& DX11RenderResource::getResourceName() const
		{
			return resourceName;
		}
	}
}

#endif