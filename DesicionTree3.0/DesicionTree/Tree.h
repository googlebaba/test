#pragma once
#include "TreeNode.h"
#include <queue>
template<class T>
class Tree{
private:	
	TreeNode<T>* root;
	std::size_t *use;
	void DestoryNodes(TreeNode<T>* root);
	void decr_use(){if(--*use==0){delete root;delete use;}}
public:
	Tree(const Tree& rhs);
	Tree();						
	virtual ~Tree();
	TreeNode<T>* getRoot();          //返回树中的根结点
	void CreateRoot(const T& rootvalue); //创建树中的根结点，使结点元素的值为rootvalue
	bool isEmpty();            //判断树是否为空
	TreeNode<T>* Parent(TreeNode<T>* current); //返回current结点的父结点
	TreeNode<T>* PrevSibling(TreeNode<T>* current); //返回current结点的前一个邻居结点
	void DeleteSubTree(TreeNode<T>* subroot);     //删除以subroot为根的结点的子数
};

template<class T>
Tree<T>::Tree(const Tree& rhs):root(rhs.root),use(new std::size_t(1))
{
	++*use;
}

template<class T>
void Tree<T>::DeleteSubTree(TreeNode<T>* subroot)
{
	if(subroot == NULL)return;
	TreeNode<T>* pointer = Parent(subroot); //找subroot的父结点
	/*if(subroot==root){//删除结点为根结点
		DestoryNodes(root);
		return;
	}*/
	if (pointer == NULL) 							// subroot就是森林第一个树根
		root = subroot->RightSibling();             //subroot的右兄弟为根
	else if(pointer->leftMostChild()==subroot)//subroot为最左结点
		pointer->setChild(subroot->RightSibling());
	else{									//subroot有左结点
		pointer = pointer->leftMostChild();
		while(pointer->RightSibling()!=subroot)
			pointer = pointer->RightSibling();
		pointer->setSibling(subroot->RightSibling());
	}
	subroot->setSibling(NULL);
	DestoryNodes(subroot);
}

template<class T>
TreeNode<T>* Tree<T>::PrevSibling(TreeNode<T>* current)
{
	using std::queue;
	queue<TreeNode<T>*>aQueue;
	TreeNode<T>* pointer = root;
	TreeNode<T>* prev=NULL;
	//当前结点为空，数为空，或所求结点为根结点时，返回NULL
	if((NULL==current)||(pointer==NULL)||(current==pointer))
		return NULL;
	while(pointer){
		if (pointer==current)
			return prev;
		aQueue.push(pointer);
		prev=pointer;
		pointer = pointer->pSibling;
	}
	while(!aQueue.empty())
	{
		prev = NULL;
		pointer = aQueue.front();
		aQueue.pop();
		pointer = pointer->leftMostChild();
		while(pointer)
		{
			if(pointer==current)
				return prev;
			aQueue.push(pointer);
			prev = pointer;
			pointer = pointer->pSibling;
		}//end while
	}//end while
	return NULL;
}

template<class T>
TreeNode<T>* Tree<T>::Parent(TreeNode<T>* current)
{
	using std::queue;
	queue<TreeNode<T>*>aQueue;
	TreeNode<T>* pointer = root;
	TreeNode<T>* upperlevelpointer = NULL;
	if(current!=NULL&&pointer!=current)
	{
		aQueue.push(pointer);
		pointer = NULL;
		while(!aQueue.empty()){
			pointer = aQueue.front();
			aQueue.pop();
			upperlevelpointer = pointer;
			pointer = pointer->leftMostChild();
			while(pointer){
				if(current = pointer)
					return upperlevelpointer;
				else{
					aQueue.push(pointer);
					pointer = pointer->RightSibling();
				}
			}//end while
		} //end while
	} //end if
	return NULL;
}

template<class T>
bool Tree<T>::isEmpty()
{
	if(root)
		return false;
	return true;
}

template<class T>
void Tree<T>::CreateRoot(const T& rootvalue)
{
	if(!root)
		root = new TreeNode<T>(rootvalue);
}

template<class T>
TreeNode<T>* Tree<T>::getRoot()
{
	return root;
}

template<class T>
Tree<T>::~Tree()
{
	if(--*use==0)
	{
	while(root){
		DeleteSubTree(root);
	}
	delete use;
	}
}

template<class T>
Tree<T>::Tree():use(new std::size_t(1))
{
	root = NULL;
}

template<class T>
void Tree<T>::DestoryNodes(TreeNode<T>* root)
{
	if(root)
	{
		DestoryNodes(root->leftMostChild());
		DestoryNodes(root->RightSibling());
		delete root;
	}
}
