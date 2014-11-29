#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

/*
=========================================

	Abstract scene  node class

=========================================
*/

namespace OGE
{
	namespace scene
	{
		enum NODE_TYPE
		{
			NODE_TYPE_NOTHING,
			NODE_TYPE_SYSTEM_MODEL,
			NODE_TYPE_ITERABLE_MODEL,
			NODE_TYPE_LIGHT,
			NODE_TYPE_CAMERA,
			NODE_TYPE_TRANSFORM
		};

		class SceneNode
		{
		public:
			SceneNode();
			SceneNode(NODE_TYPE nodeType);
			virtual ~SceneNode();

			NODE_TYPE getNodeType() const;

		protected:
			NODE_TYPE nodeType;
		};

		inline SceneNode::SceneNode() : nodeType(NODE_TYPE::NODE_TYPE_NOTHING) {}
		inline SceneNode::SceneNode(NODE_TYPE nodeType) : nodeType(nodeType) {}
		inline SceneNode::~SceneNode(){}

		inline NODE_TYPE SceneNode::getNodeType() const
		{
			return nodeType;
		}
	}
}

#endif

