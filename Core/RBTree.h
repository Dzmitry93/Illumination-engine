#ifndef __RBTREE_H__
#define __RBTREE_H__

namespace OGE
{
	namespace Collections
	{
		template<class K, class V>
		struct Node
		{
			K mKey;
			V mValue;

			Node<K, V>* pParent;

			Node<K, V>* pLeft;
			Node<K, V>* pRight;

			// false - black
			// true - red
			bool mColor;
		};

		template<class K, class V>
		class RBTree
		{
		public:
			RBTree(void);
			~RBTree(void);

			void insert(const K& rKey, const V& rValue);
			void remove(const K& rKey);

			V minValue() const;
			V maxValue() const;

			void traverse(List<std::pair<K, V>>& rValues);

		private:

			Node<K, V>* pRoot;

			void rightRotate(Node<K, V>* pRoot);
			void leftRotate(Node<K, V>* pRoot);

			void fixupInsert(Node<K, V>* pRoot);
			void fixupRemove(Node<K, V>* pRoot);

			Node<K, V>* treeSuccessor(Node<K, V>* pNode);

			Node<K, V>* minNode(Node<K, V>* pNode);
			Node<K, V>* maxNode(Node<K, V>* pNode);

			void traverse(List<std::pair<K, V>>& rValues, Node<K, V>* pNode);
		};
		
		template<class K, class V>
		inline RBTree<K, V>::RBTree()
		{
			pRoot = nullptr;
		}

		template<class K, class V>
		inline RBTree<K, V>::~RBTree()
		{
			//pRoot = nullptr;
		}

		template<class K, class V>
		void RBTree<K, V>::insert(const K& rKey, const V& rValue)
		{
			if(pRoot == nullptr)
			{
				pRoot = new Node<K, V>();
				pRoot->mColor = false;
				pRoot->mKey = rKey;
				pRoot->mValue = rValue;
			}
			else
			{
				Node<K, V>* pNode = pRoot;
				Node<K, V>* pParent = pNode;

				while(pNode != nullptr)
				{
					if(rKey < pNode->mKey) { pParent = pNode; pNode = pNode->pLeft; } 
					else { pParent = pNode; pNode = pNode->pRight; } 
				}

				pNode = pParent;

				Node<K, V>* pNewNode = new Node<K, V>();

				pNewNode->pParent = pNode;
				pNewNode->mKey = rKey;
				pNewNode->mValue = rValue;
				pNewNode->mColor = true;

				if(rKey >= pNode->mKey) { pNode->pRight = pNewNode; }
				else { pNode->pLeft = pNewNode; }

				fixupInsert(pNewNode);
			}
		}

		template<class K, class V>
		void RBTree<K, V>::fixupInsert(Node<K, V>* pNode)
		{
			while(pNode->pParent != nullptr && pNode->pParent->mColor == true)
			{
				if(pNode->pParent == pNode->pParent->pParent->pLeft)
				{
					Node<K, V>* pUncle = pNode->pParent->pParent->pRight;

					if(pUncle != nullptr && pUncle->mColor == true)
					{
						pNode->pParent->mColor = false;
						pUncle->mColor = false;
						pUncle->pParent->mColor = true;
						pNode = pUncle->pParent;
					}
					else
					{
						if(pNode->pParent->pRight == pNode)
						{
							pNode = pNode->pParent;
							leftRotate(pNode);
						}
						
						pNode->pParent->mColor = false;
						pNode->pParent->pParent->mColor = true;	

						rightRotate(pNode->pParent->pParent);
					}
				}
				else
				{
					Node<K, V>* pUncle = pNode->pParent->pParent->pLeft;

					if(pUncle != nullptr && pUncle->mColor == true)
					{
						pNode->pParent->mColor = false;
						pUncle->mColor = false;
						pUncle->pParent->mColor = true;
						pNode = pUncle->pParent;
					}
					else
					{
						if(pNode->pParent->pLeft == pNode)
						{
							pNode = pNode->pParent;
							rightRotate(pNode);
						}
						
						pNode->pParent->mColor = false;
						pNode->pParent->pParent->mColor = true;	

						leftRotate(pNode->pParent->pParent);
					}
				}

				pRoot->mColor = false;
			}
		}

		template<class K, class V>
		void RBTree<K, V>::remove(const K& rKey)
		{
			Node<K, V>* pNode = pRoot;

			while(pNode != nullptr && pNode->mKey != rKey)
			{
				if(rKey < pNode->mKey) { pNode = pNode->pLeft; }
				else { pNode = pNode->pRight; }
			}


		/*	if(pNode != nullptr)
			{
				Node<K, V>* pNodeY = nullptr;
				Node<K, V>* pNodeX = nullptr;

				if(pNode->pLeft == nullptr || pNode->pRight == nullptr) { pNodeY = pNode; }
				else { pNodeY = treeSuccessor(pNode); }	

				if(pNodeY->pLeft != nullptr) { pNodeX = pNodeY->pLeft; }
				else { pNodeX = pNodeY->pRight; }

				if(pNodeX != nullptr)
				{
					pNodeX->pParent = pNodeY->pParent;
				}

				if(pNodeY->pParent == nullptr) { pRoot = pNodeX; }
				else 
				{
					if(pNodeY == pNodeY->pParent->pLeft) { pNodeY->pParent->pLeft = pNodeX; }
					else {pNodeY->pParent->pRight = pNodeX; }
				}

				if(pNodeY != pNode)
				{
					pNode->mKey = pNode->mKey;
					pNode->mValue = pNodeY->mValue;
				}

				if(pNodeY->mColor == false && pNodeX != nullptr)
				{
					fixupRemove(pNodeX);
				}

				delete pNodeY;
			}		*/
		}

		template<class K, class V>
		void RBTree<K, V>::fixupRemove(Node<K, V>* pNode)
		{
			Node<K, V>* pChild = pNode->pRight == nullptr ? pNode->pLeft : pNode->pRight;

			/*while(pNode != pRoot && pNode->mColor == false)
			{
				Node<K, V>* pNodeW;

				if(pNode->pParent->pLeft == pNode)
				{
					pNodeW = pNode->pParent->pRight;

					if(pNodeW != nullptr)
					{
						if(pNodeW->mColor == true)
						{
							pNodeW->mColor = false;
							pNode->pParent->mColor = true;
							leftRotate(pNode->pParent);

							pNodeW = pNode->pParent->pRight;
						}

						if(pNodeW->pLeft != nullptr && pNodeW->pLeft->mColor == false && pNodeW->pRight->mColor == false)
						{
							pNodeW->mColor = true;
							pNode = pNode->pParent;
						}
						else 
						{
							if(pNodeW->pRight->mColor == false)
							{
								if(pNodeW->pLeft != nullptr)
								{
									pNodeW->pLeft->mColor = false;
								}

								pNodeW->mColor = true;
								rightRotate(pNodeW);

								pNodeW = pNode->pParent->pRight;
							}

							pNodeW->mColor = pNode->pParent->mColor;
							pNode->pParent->mColor = false;
							pNodeW->pRight->mColor = false;

							leftRotate(pNode->pParent);

							pNode = pRoot;
						} 
						
					}
				}
				else
				{
					pNodeW = pNode->pParent->pLeft;

					if(pNodeW != nullptr)
					{
						if(pNodeW->mColor == true)
						{
							pNodeW->mColor = false;
							pNode->pParent->mColor = true;
							rightRotate(pNode->pParent);

							pNodeW = pNode->pParent->pLeft;
						}

						if(pNodeW->pRight != nullptr && pNodeW->pRight->mColor == false && pNodeW->pLeft->mColor == false)
						{
							pNodeW->mColor = true;
							pNode = pNode->pParent;
						}
						else 
						{
							if(pNodeW->pLeft->mColor == false)
							{
								if(pNodeW->pRight != nullptr)
								{
									pNodeW->pRight->mColor = false;
								}

								pNodeW->mColor = true;
								leftRotate(pNodeW);

								pNodeW = pNode->pParent->pLeft;
							}

							pNodeW->mColor = pNode->pParent->mColor;
							pNode->pParent->mColor = false;
							pNodeW->pLeft->mColor = false;

							rightRotate(pNode->pParent);

							pNode = pRoot;
						}
						
					}
				}

				pNode->mColor = false;
			}	   */
		}

		template<class K, class V>
		inline Node<K, V>* RBTree<K, V>::treeSuccessor(Node<K, V>* pNode)
		{
			if(pNode->pRight != nullptr)
			{
				return minNode(pNode->pRight);
			}

			Node<K, V>* pSuccessor = pNode->pParent;
			Node<K, V>* pTemp = pNode;

			while(pSuccessor != nullptr && 	pTemp == pSuccessor->pRight)
			{
				pTemp = pSuccessor;
				pSuccessor = pSuccessor->pParent;
			}

			return pSuccessor;
		}

		template<class K, class V>						  
		inline Node<K, V>* RBTree<K, V>::minNode(Node<K, V>* pRoot)
		{
			Node<K, V>* pNode = pRoot;

			if(pNode != nullptr)
			{
				while(pNode->pLeft != nullptr)
				{	 
					pNode = pNode->pLeft;
				}
			}

			return pNode;
		}

		template<class K, class V>
		inline V RBTree<K, V>::minValue() const
		{
			Node<K, V>* pNode = min(pRoot);

			if(pNode != nullptr)
			{
				return pNode->mValue;
			}

			return V();
		}

		template<class K, class V>
		inline V RBTree<K, V>::maxValue() const
		{
			Node<K, V>* pNode = max(pRoot);

			if(pNode != nullptr)
			{
				return pNode->mValue;
			}

			return V();
		}

		template<class K, class V>						  
		inline Node<K, V>* RBTree<K, V>::maxNode(Node<K, V>* pRoot)
		{
			Node<K, V>* pNode = pRoot;

			if(pNode != nullptr)
			{
				while(pNode->pRight != nullptr)
				{	 
					pNode = pNode->pRight;
				}
			}

			return pNode;
		}

		template<class K, class V>
		inline void RBTree<K, V>::rightRotate(Node<K, V>* pRoot)
		{
			Node<K, V>* pNewRoot = pRoot->pLeft;

			pRoot->pLeft = pNewRoot->pRight;

			if(pNewRoot->pRight != nullptr) { pNewRoot->pRight->pParent = pRoot; }

			pNewRoot->pParent = pRoot->pParent;

			if(pRoot->pParent == nullptr) { this->pRoot = pNewRoot; } 
			else if(pRoot->pParent->pRight == pRoot) { pRoot->pParent->pRight = pNewRoot; } 
			else { pRoot->pParent->pLeft = pNewRoot; }

			pNewRoot->pRight = pRoot;
			pRoot->pParent = pNewRoot;
		}

		template<class K, class V>									
		inline void RBTree<K, V>::leftRotate(Node<K, V>* pRoot)
		{
			Node<K, V>* pNewRoot = pRoot->pRight;

			pRoot->pRight = pNewRoot->pLeft;

			if(pNewRoot->pLeft != nullptr) { pNewRoot->pLeft->pParent = pRoot; }

			pNewRoot->pParent = pRoot->pParent;

			if(pRoot->pParent == nullptr) { this->pRoot = pNewRoot; } 
			else if(pRoot->pParent->pLeft == pRoot) { pRoot->pParent->pLeft = pNewRoot; } 
			else { pRoot->pParent->pRight = pNewRoot; }

			pNewRoot->pLeft = pRoot;
			pRoot->pParent = pNewRoot;
		}  

		template<class K, class V>
		inline void RBTree<K, V>::traverse(List<std::pair<K, V>>& rValues)
		{
			traverse(rValues, pRoot);	
		} 

		template<class K, class V>
		void RBTree<K, V>::traverse(List<std::pair<K, V>>& rValues, Node<K, V>* pNode)
		{
			if(pNode != nullptr)
			{
				if(pNode->pLeft != nullptr)
				{
					traverse(rValues, pNode->pLeft);
				}

				rValues.push_back(std::pair<K, V>(pNode->mKey, pNode->mValue));

				if(pNode->pRight != nullptr)
				{
					traverse(rValues, pNode->pRight);
				}
			}
		}
	}
}

#endif

