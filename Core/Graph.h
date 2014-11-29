#ifndef __GRAPH_H__ 
#define __GRAPH_H__

#include <list>

using std::list;

namespace OGE
{
	namespace Collections 
	{
		template<class T>
		class GraphItem 
		{
			T data;
		};

		class GraphIterator 
		{
		};

		template<class T>
		class Graph
		{
		public:
			Graph(void);
			~Graph(void);

			void addItem(GraphItem parent, GraphItem child);
		private: 
			list<list<GraphItem<T>>> items;
		};

		template<class T>
		Graph<T>::Graph()
		{
			items.begin();

		}
	}
}

#endif
