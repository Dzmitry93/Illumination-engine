#ifndef __BST_H__
#define __BST_H__

#include "List.h"
#include "Comparator.h"
#include "IntrusivePtr.h"

using OGE::memory::IntrusivePtr;

namespace OGE
{
	namespace Collections
	{
		template<class T, class S>
		struct BSTNode
		{
			BSTNode() {}
			BSTNode(const T& rData, const S& rKey) : data(rData), key(rKey) {}

			S key;
			T data;

			BSTNode<T, S>* pLeft;
			BSTNode<T, S>* pRight;
			BSTNode<T, S>* pParent;
		};

		template<class T, class S>
		class BST
		{
		public:

			BST(IObjectObjectComparator<S, S, COMPARE_RESULT>* pComparator);
			~BST();

			void insert(const T& rData, const S& rKey);
			void deleteNode(const S& rKey);
			bool search(const S& rKey, T& rData) const;
			void traverse(List<T>& list);
			void keyList(List<S>& list);
			void copy( BST<T, S>& rBST);

		private :
			BSTNode<T, S>* mRoot;
			IObjectObjectComparator<S, S, COMPARE_RESULT>* pComparator;

			void insertNode(BSTNode<T, S>** pRoot, const T& rData, const S& rKey);
			void removeAll(BSTNode<T, S>* pRoot);
			void removeNode(BSTNode<T, S>* pRoot);
			bool isLeftChild(BSTNode<T, S>* pRoot);

			void maxNode(BSTNode<T, S>* pRoot, BSTNode<T, S>** pNode);
			void minNode(BSTNode<T, S>* pRoot, BSTNode<T, S>** pNode);
			void getNode(BSTNode<T, S>* pRoot, const S& rKey, BSTNode<T, S>** pNode) const;
			
			void symmetricTraverse(BSTNode<T, S>* pRoot, List<T>& rList);
			void symmetricKeyTraverse(BSTNode<T, S>* pRoot, List<S>& rList);
		};

		template<class T, class S>
		BST<T, S>::BST(IObjectObjectComparator<S, S, COMPARE_RESULT>* pComparator) : pComparator(pComparator)
		{
			mRoot = NULL;
		}

		template<class T, class S>
		BST<T, S>::~BST()
		{
			if(mRoot)
			{
				removeAll(mRoot);
			}
		}

		template<class T, class S>
		inline void BST<T, S>::insert(const T& rData, const S& rKey)
		{
			insertNode(&mRoot, rData, rKey);
		}

		template<class T, class S>
		inline void BST<T, S>::insertNode(BSTNode<T, S>** pRoot, const T& rData, const S& rKey)
		{
			if(*pRoot == NULL)
			{
				*pRoot = new BSTNode<T, S>(rData, rKey);
				(*pRoot)->pLeft = NULL;
				(*pRoot)->pRight = NULL;
			}
			else
			{
				S& key =  (*pRoot)->key;
				if(pComparator->compare(rKey, key) == GREATER 
					|| pComparator->compare(rKey, key) == EQUALS)
				{
					insertNode(&(*pRoot)->pRight, rData, rKey);
					(*pRoot)->pRight->pParent = (*pRoot);
				}
				else
				{
					insertNode(&(*pRoot)->pLeft, rData, rKey);
					(*pRoot)->pLeft->pParent = (*pRoot);
				}
			}
		}

		template<class T, class S>
		inline void BST<T, S>::deleteNode(const S& rKey)
		{
			BSTNode<T, S>* pRoot = NULL;
			getNode(mRoot, rKey, &pRoot);

			if(pRoot != nullptr)
			{
				removeNode(pRoot);
			}
		}

		template<class T, class S>
		inline bool BST<T, S>::search(const S& rKey, T& rData) const
		{
			BSTNode<T, S>* pNode = NULL;
			getNode(mRoot, rKey, &pNode);
			
			if(pNode)
			{
				rData = pNode->data;
				return true;
			}
			return false;
		}

		template<class T, class S>
		inline void BST<T, S>::traverse(List<T>& list)
		{
			symmetricTraverse(mRoot, list);
		}

		template<class T, class S>
		inline void BST<T, S>::keyList(List<S>& rKeyList)
		{
			symmetricKeyTraverse(mRoot, rKeyList);
		}

		template<class T, class S>
		void BST<T, S>::symmetricTraverse(BSTNode<T, S>* pRoot, List<T>& rList)
		{
			if(pRoot != NULL)
			{
				if(pRoot->pLeft != NULL)
				{
					symmetricTraverse(pRoot->pLeft, rList);
				}

				rList.push_back(pRoot->data);

				if(pRoot->pRight != NULL)
				{
					symmetricTraverse(pRoot->pRight, rList);
				}
			}
		}

		template<class T, class S>
		void BST<T, S>::symmetricKeyTraverse(BSTNode<T, S>* pRoot, List<S>& rList)
		{
			if(pRoot != NULL)
			{
				if(pRoot->pLeft != NULL)
				{
					symmetricKeyTraverse(pRoot->pLeft, rList);
				}

				rList.push_back(pRoot->key);

				if(pRoot->pRight != NULL)
				{
					symmetricKeyTraverse(pRoot->pRight, rList);
				}
			}
		}

		template<class T, class S>
		void BST<T, S>::getNode(BSTNode<T, S>* pRoot, const S& rKey, BSTNode<T, S>** pNode) const
		{
			COMPARE_RESULT result = pComparator->compare(rKey, pRoot->key);
			if( result == EQUALS)
			{
				(*pNode) = pRoot;
			}
			if(result == GREATER)
			{
				if(pRoot->pRight != NULL)
				{
					getNode(pRoot->pRight, rKey, pNode);
				}
			}
			else
			{
				if(pRoot->pLeft != NULL)
				{
					getNode(pRoot->pLeft, rKey, pNode);
				}
			}
		}

		template<class T, class S>
		void BST<T, S>::removeNode(BSTNode<T, S>* pRoot)
		{
			if(pRoot == mRoot)
			{
				if(pRoot->pRight)
				{
					BSTNode<T, S>* pMinNode = NULL;
					minNode(pRoot->pRight, &pMinNode);

					pRoot->data = pMinNode->data;
					pRoot->key = pMinNode->key;

					removeNode(pMinNode);
				}
				else
				{
					mRoot = mRoot->pLeft;
					mRoot->pParent = nullptr;

					safe_release(pRoot);
					pRoot = mRoot;
				}
			}
			else
			{
				BSTNode<T, S>* pChild = NULL;
				if(isLeftChild(pRoot))
				{
					maxNode(pRoot->pRight, &pChild);
				}
				else
				{
					minNode(pRoot->pLeft, &pChild);	
				}

				if(pChild == nullptr)
				{
					pChild = pRoot;
				}
				else
				{
					pRoot->key = pChild->key;
					pRoot->data = pChild->data;
				}

				if(isLeftChild(pChild))
				{
					if(pChild->pLeft)
					{
						pChild->pParent->pLeft = pChild->pLeft;
						pChild->pLeft->pParent = pChild->pParent; 
					}
					else 
					if(pChild->pRight)
					{
						pChild->pParent->pLeft = pChild->pRight;
						pChild->pRight->pParent = pChild->pParent; 
					}
					else
					{
						pChild->pParent->pLeft = nullptr;
					}
				}
				else
				{
					if(pChild->pLeft)
					{
						pChild->pParent->pRight = pChild->pLeft;
						pChild->pLeft->pParent = pChild->pParent; 
					}
					else 
					if(pChild->pRight)
					{
						pChild->pParent->pRight = pChild->pRight;
						pChild->pRight->pParent = pChild->pParent; 
					}
					else
					{
						pChild->pParent->pRight = nullptr;
					}
				}

				safe_release(pChild);
			}
		}

		template<class T, class S>
		inline void BST<T, S>::removeAll(BSTNode<T, S>* pRoot)
		{
			if(pRoot->pLeft != NULL)
			{
				removeAll(pRoot->pLeft);
					pRoot->pLeft = NULL;
			}
			if(pRoot->pRight != NULL)
			{
				removeAll(pRoot->pRight);
				pRoot->pRight = NULL;
			}
			if(!(pRoot->pLeft && pRoot->pRight))
			{
				delete pRoot;
			}
		}

		template<class T, class S>
		inline bool BST<T, S>::isLeftChild(BSTNode<T, S>* pNode)
		{
			return pNode->pParent->pLeft == pNode;
		}

		template<class T, class S>
		inline void BST<T, S>::maxNode(BSTNode<T, S>* pRoot, BSTNode<T, S>** pNode)
		{
			if(pRoot == nullptr)
			{
				pNode = nullptr;
				return;
			}

			if(pRoot->pRight != NULL)
			{
				maxNode(pRoot->pRight, pNode);
			}
			else
			{
				(*pNode) = pRoot;
			}
		}

		template<class T, class S>
		inline void BST<T, S>::minNode(BSTNode<T, S>* pRoot, BSTNode<T, S>** pNode)
		{
			if(pRoot == nullptr)
			{
				pNode = nullptr;
				return;
			}
			if(pRoot->pLeft != NULL)
			{
				maxNode(pRoot->pLeft, pNode);
			}
			else
			{
				(*pNode) = pRoot;
			}
		}

		template<class T, class S>
		inline void BST<T, S>::copy(BST<T, S>& rBST)
		{
			List<T> dataList;
			List<S> keyList;

			rBST.traverse(dataList);
			rBST.keyList(keyList);

			ListIterator<T> dataIt = dataList.iterator();
			ListIterator<S> keyIt = keyList.iterator();

			while(dataIt.hasNext())
			{
				insert(dataIt.next(), keyIt.next());
			}
		}
	}
}

#endif