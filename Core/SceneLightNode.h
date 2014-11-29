#ifndef __SCENE_LIGHT_NODE_H__
#define __SCENE_LIGHT_NODE_H__

#include "SceneObjectNode.h"
#include "BaseLight.h"

#include "Mesh.h"
#include "IntrusivePtr.h"

using OGE::memory::IntrusivePtr;
using OGE::resource::Mesh;

namespace OGE
{
	namespace scene
	{
		class SceneLightNode : public SceneObjectNode
		{
		public:
			SceneLightNode(BaseLight* pLight, const Matrix& rLocalMatrix);
			virtual ~SceneLightNode(void);

			BaseLight* getLight() const;

		private:

			BaseLight* pLight;
		};

		inline SceneLightNode::SceneLightNode(BaseLight* pLight, const Matrix& rLocalMatrix) : 
						SceneObjectNode(NODE_TYPE_LIGHT, rLocalMatrix), pLight(pLight)
		{
		}

		inline SceneLightNode::~SceneLightNode(void) 
		{
			if(pLight != NULL)
			{
				delete pLight;
			}
		}

		inline BaseLight* SceneLightNode::getLight() const
		{
			return pLight;
		}
	}
}

#endif
