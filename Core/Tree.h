#ifndef __TREE_H__
#define __TREE_H__

#include "List.h"

namespace OGE
{
	namespace Collections
	{
		template<class T>
		class TreeNode
		{
		public:
			void* operator new(size_t size);
			void operator delete(void* ptr);

			TreeNode<T>* pParent;
			List<TreeNode<T>*> childs;
			T data;
		};

		/*
		====================================================================================
		TreeIterator class 
		====================================================================================
		*/

		template<class T>
		class TreeIterator : public Iterator<T>
		{
		public:
			TreeIterator(TreeNode<T>* pNode);
			TreeIterator(const TreeIterator<T>& rhs);
			~TreeIterator();

			void up();
			void down();

			virtual T& next();
			virtual T& previous();

			virtual void front();
			virtual void back();

			void toRoot();

			bool isValid() const;
			bool isRoot() const;

			TreeNode<T>* getCurrentNode();
			const ListIterator<TreeNode<T>*>& getListIterator() const;

			virtual T& data();

			virtual bool equals(const TreeIterator<T>& rhs) const;
			virtual bool hasNext() const;

			void operator = (const TreeIterator<T>& rhs);

			void* operator new(size_t size);
			void operator delete(void* ptr);

		private:
			TreeNode<T>* pNode;
			List<ListIterator<TreeNode<T>*>> childsItStack;
			ListIterator<TreeNode<T>*> childsIt;

			void reset();
		};

		/*
		====================================================================================
		ConstTreeIterator class
		====================================================================================
		*/

		template<class T> 
		class ConstTreeIterator : public ConstIterator<T>
		{
		public:
			ConstTreeIterator(TreeNode<T>* pNode);
			ConstTreeIterator(const ConstTreeIterator<T>& rhs);
			~ConstTreeIterator();

			void up();
			void down();

			virtual T& next();
			virtual T& previous();

			virtual void front();
			virtual void back();

			void toRoot();

			bool isValid() const;
			bool isRoot() const;

			const TreeNode<T>* getCurrentNode() const;
			const ConstListIterator<TreeNode<T>*>& getListIterator() const;

			virtual const T& data() const;

			virtual bool equals(const ConstTreeIterator<T>& rhs) const;
			virtual bool hasNext() const;

			void operator = (const ConstTreeIterator<T>& rhs);

			void* operator new(size_t size);
			void operator delete(void* ptr);

		private:
			TreeNode<T>* pNode;
			List<ConstListIterator<TreeNode<T>*>> childsItStack;
			ConstListIterator<TreeNode<T>*> childsIt;

			void reset();
		};

		/*
		====================================================================================
		Traverse strategies classes
		====================================================================================
		*/

		template<class T>
		class TraverseStrategy
		{
		public:
			virtual void traverse(TreeIterator<T>& it, List<T>& nodesQueue) = 0;
		};

		template<class T>
		class PreOrderTraverse : public TraverseStrategy<T>
		{
		public:
			void traverse(TreeIterator<T>& it, List<T>& nodesQueue);
		};

		template<class T>
		class PostOrderTraverse : public TraverseStrategy<T>
		{
		public:
			void traverse(TreeIterator<T>& it, List<T>& nodesQueue);
		};

		/*
		====================================================================================
		Tree class 
		====================================================================================
		*/

		template<class T>
		class Tree
		{
		public:
			Tree();
			Tree(T rRootData);
			~Tree();

			void insert(TreeIterator<T>& it, const T& node);
			void remove(TreeIterator<T>& it);
			void clear();

			TreeIterator<T> root() const; 
			bool isRoot(TreeIterator<T>& it) const;

			void traverse(TraverseStrategy<T>& strategy, TreeIterator<T>& whereIt, List<T>& nodesQueue);

			uint32 countChilds(TreeIterator<T>& it) const;

			void* operator new(size_t size);
			void operator delete(void* ptr);

		private:
			TreeNode<T>* pRoot;
			uint32 countAllChilds;

			void destroy();
		};

		/*
		==================================================================================
		Tree node implementation
		==================================================================================
		*/

		template<class T>
		inline void* TreeNode<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInPool<TreeNode<T>>();
		}

		template<class T>
		inline void TreeNode<T>::operator delete(void* ptr)
		{
			MemoryManager::getInstance()->deallocateFromPool<TreeNode<T>>((TreeNode<T>*) ptr);
		}

		/*
		==================================================================================
		Tree implementation
		==================================================================================
		*/

		template<class T>
		inline Tree<T>::Tree()
		{
			this->pRoot = new TreeNode<T>();
			this->pRoot->pParent = NULL;
			this->countAllChilds = 0;
		}

		template<class T>
		inline Tree<T>::Tree(T rData) 
		{
			this->pRoot = new TreeNode<T>();
			this->pRoot->data = rData;
			this->pRoot->pParent = NULL;
			this->countAllChilds = 0;
		}

		template<class T>
		inline Tree<T>::~Tree()
		{
			destroy();
		}

		template<class T>
		inline TreeIterator<T> Tree<T>::root() const
		{
			return TreeIterator<T>(pRoot);
		}

		template<class T>
		inline void Tree<T>::destroy()
		{
			// remove root data
			remove(TreeIterator<T>(pRoot));
		}

		template<class T>
		void Tree<T>::remove(TreeIterator<T>& it)
		{
			TreeNode<T>* pTreeNode = it.getCurrentNode();
			while(it.hasNext())
			{
				it.down();
				remove(it);
				it.next();
			}

			it.up();
			countAllChilds--;
			delete pTreeNode;
		}

		template<class T>
		inline void Tree<T>::clear()
		{
			destroy();

			this->pRoot = new TreeNode<T>();
			this->pRoot->data = 0;
			this->pRoot->pParent = NULL;
		}

		template<class T>
		void Tree<T>::insert(TreeIterator<T>& it, const T& data)
		{
			TreeNode<T>* pParent = it.getCurrentNode();
			TreeNode<T>* pNode = new TreeNode<T>();
			
			pNode->data = data;
			pNode->pParent = pParent;
			pParent->childs.insertAfter(it.getListIterator(), pNode);

			countAllChilds++;
			
			it.next();
			
		}

		template<class T>
		uint32 Tree<T>::countChilds(TreeIterator<T>& it) const
		{
			while(it.hasNext())
			{
				it.down();
				countChilds(it);
				it.up();
				it.next();
			}

			uint32 count = 0;
			count += it.getCurrentNode()->childs.size();

			return count;
		}

		template<class T>
		inline bool Tree<T>::isRoot(TreeIterator<T>& it) const
		{
			return it.isRoot();
		}

		template<class T>
		inline void Tree<T>::traverse(TraverseStrategy<T>& strategy, TreeIterator<T>& it, List<T>& nodesQueue)
		{
			strategy.traverse(it, nodesQueue);
		}

		template<class T>
		inline void* Tree<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInStack<Tree<T>>();
		}

		template<class T>
		inline void Tree<T>::operator delete(void* ptr)
		{
			MemoryManager::getInstance()->deallocateFromStack();
		}

		/*
		===========================================================================
		Traverse strategies implementation
		===========================================================================
		*/

		template<class T>
		void PreOrderTraverse<T>::traverse(TreeIterator<T>& it, List<T>& nodesQueue)
		{
			TreeNode<T>* pTreeNode = it.getCurrentNode();
			nodesQueue.push_back(pTreeNode->data);

			while(it.hasNext())
			{
				it.down();
				traverse(it, nodesQueue);
				it.up();
				it.next();
			}

		}

		template<class T>
		void PostOrderTraverse<T>::traverse(TreeIterator<T>& it, List<T>& nodesQueue)
		{
			TreeNode<T>* pTreeNode = it.getCurrentNode();

			while(it.hasNext())
			{
				it.down();
				traverse(it, nodesQueue);
				it.up();
				it.next();
			}

			nodesQueue.push_back(pTreeNode->data);
		}
		
		/*
		============================================================================
		TreeIterator implementation
		============================================================================
		*/

		template<class T>
		inline TreeIterator<T>::TreeIterator(TreeNode<T>* pNode) : 
			pNode(pNode), childsIt(pNode->childs.iterator())
		{
			reset();
		}

		template<class T>
		inline TreeIterator<T>::~TreeIterator()
		{
		}

		template<class T>
		inline TreeIterator<T>::TreeIterator(const TreeIterator<T>& rhs) : 
			pNode(rhs.pNode), childsIt(rhs.pNode->childs.iterator())
		{
			reset();
		}

		template<class T>
		inline void TreeIterator<T>::operator = (const TreeIterator<T>& rhs)
		{
			this->pNode = rhs.pNode;
			reset();
		}

		template<class T>
		inline void TreeIterator<T>::reset()
		{
			childsIt = pNode->childs.iterator();
		}

		template<class T>
		void TreeIterator<T>::toRoot()
		{
			while(pNode->pParent != NULL)
			{
				pNode = pNode->pParent;
			}

			reset();
		}

		template<class T>
		inline void TreeIterator<T>::up()
		{
			if(pNode->pParent != NULL)
			{
				if(childsItStack.size() > 0)
				{
					childsIt = childsItStack.pop_back();
				}

				pNode = pNode->pParent;
			}
		}

		template<class T>
		inline void TreeIterator<T>::down()
		{
			if(pNode->childs.size() > 0)
			{
				childsItStack.push_back(ListIterator<TreeNode<T>*>(childsIt));
				pNode = childsIt.next();
				front();
			}
		}

		template<class T>
		inline T& TreeIterator<T>::next()
		{
			return childsIt.next()->data;
		}

		template<class T>
		inline T& TreeIterator<T>::previous()
		{
			return childsIt.previous()->data;	
		}

		template<class T>
		inline void TreeIterator<T>::front()
		{
			childsIt = pNode->childs.iterator();
		}

		template<class T>
		inline void TreeIterator<T>::back()
		{
			childsIt = pNode->childs.iterator();
			childsIt.back();
		}

		template<class T>
		inline void* TreeIterator<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInStack<TreeIterator<T>>();
		}

		template<class T>
		inline void TreeIterator<T>::operator delete(void* ptr) 
		{
			MemoryManager::getInstance()->deallocateFromStack();
		}

		template<class T>
		inline bool TreeIterator<T>::isValid() const
		{
			return pNode != NULL;
		}

		template<class T>
		inline bool TreeIterator<T>::equals(const TreeIterator<T>& rhs) const
		{
			return this->pNode == rhs.pNode && this->childsIt.equals(rhs.childsIt);
		}

		template<class T>
		inline bool TreeIterator<T>::hasNext() const
		{
			return childsIt.hasNext();
		}

		template<class T>
		inline bool TreeIterator<T>::isRoot() const
		{
			return pNode->pParent == NULL;
		}

		template<class T>
		inline TreeNode<T>* TreeIterator<T>::getCurrentNode()
		{
			return pNode;
		}

		template<class T>
		inline const ListIterator<TreeNode<T>*>& TreeIterator<T>::getListIterator() const
		{
			return childsIt;
		}

		template<class T>
		inline T& TreeIterator<T>::data()
		{
			return childsIt.data()->data;
		}

		/*
		===============================================================================
		Const Iterator implementation
		===============================================================================
		*/

		template<class T>
		inline ConstTreeIterator<T>::ConstTreeIterator(TreeNode<T>* pNode) : 
			pNode(pNode), childsIt(pNode->childs.citerator())
		{
		}

		template<class T>
		inline ConstTreeIterator<T>::~ConstTreeIterator()
		{
		}

		template<class T>
		inline ConstTreeIterator<T>::ConstTreeIterator(const ConstTreeIterator<T>& rhs) : 
			pNode(rhs.pNode), childsIt(rhs.pNode->childs.citerator())
		{
			reset();
		}

		template<class T>
		inline void ConstTreeIterator<T>::operator = (const ConstTreeIterator<T>& rhs)
		{
			this->pNode = rhs.pNode;
			reset();
		}

		template<class T>
		inline void ConstTreeIterator<T>::reset()
		{
			childsIt = pNode->childs.citerator();
		}

		template<class T>
		void ConstTreeIterator<T>::toRoot()
		{
			while(pNode->pParent != NULL)
			{
				pNode = pNode->pParent;
			}

			reset();
		}

		template<class T>
		inline void ConstTreeIterator<T>::up()
		{
			if(pNode->pParent != NULL)
			{
				if(childsItStack.size() > 0)
				{
					childsIt = childsItStack.pop_back();
				}

				pNode = pNode->pParent;
			}
		}

		template<class T>
		inline void ConstTreeIterator<T>::down()
		{
			if(pNode->childs.size() > 0)
			{
				childsItStack.push_back(ListIterator<TreeNode<T>*>(childsIt));
				pNode = childsIt.next();
				front();
			}
		}

		template<class T>
		inline T& ConstTreeIterator<T>::next()
		{
			return childsIt.next();
		}

		template<class T>
		inline T& ConstTreeIterator<T>::previous()
		{
			return childsIt.previous();	
		}

		template<class T>
		inline void ConstTreeIterator<T>::front()
		{
			childsIt = pNode->childs.iterator();
		}

		template<class T>
		inline void ConstTreeIterator<T>::back()
		{
			childsIt = pNode->childs.citerator();
			childsIt.back()
		}

		template<class T>
		inline void* ConstTreeIterator<T>::operator new(size_t size)
		{
			return MemoryManager::getInstance()->allocateInStack<ConstTreeIterator<T>>();
		}

		template<class T>
		inline void ConstTreeIterator<T>::operator delete(void* ptr) 
		{
			MemoryManager::getInstance()->deallocateFromStack();
		}

		template<class T>
		inline bool ConstTreeIterator<T>::isValid() const
		{
			return pNode != NULL;
		}

		template<class T>
		inline bool ConstTreeIterator<T>::equals(const ConstTreeIterator<T>& rhs) const
		{
			return this->pNode == rhs.pNode && this->childsIt.equals(rhs.childsIt);
		}

		template<class T>
		inline bool ConstTreeIterator<T>::hasNext() const
		{
			return childsIt.hasNext();
		}

		template<class T>
		inline bool ConstTreeIterator<T>::isRoot() const
		{
			return pNode->pParent == NULL;
		}

		template<class T>
		inline const TreeNode<T>* ConstTreeIterator<T>::getCurrentNode() const
		{
			return pNode;
		}

		template<class T>
		inline const ConstListIterator<TreeNode<T>*>& ConstTreeIterator<T>::getListIterator() const
		{
			return childsIt;
		}

		template<class T>
		inline const T& ConstTreeIterator<T>::data() const
		{
			return childsIt.data();
		}
	}
}

#endif

