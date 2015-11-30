#include <vector>
template <class T>class Tree;

template<class T>
#pragma once
class TreeNode{
	friend class Tree<T>;
private:
	T m_Value;					//树结点的值
	TreeNode<T>* pChild;		//左子结点
	TreeNode<T>* pSibling;		//右兄弟结点
//	std::vector<T> tags;      //储存此点还要计算的属性值
public:
//	TreeNode(const TreeNode&);   //复制构造函数
	TreeNode(const T& value);			//构造函数
//	TreeNode(const T& value,std::vector<T> shuxing); //构造函数
	virtual ~TreeNode(){}
	bool isleaf();				 //如果结点是叶，返回true;
	T value();					//返回结点的值
	TreeNode<T>* leftMostChild();			//返回第一个左孩子
	TreeNode<T>* RightSibling();			//返回右兄弟
	void setValue(T&);						//设置结点的值
	void setChild(TreeNode<T>* pointer);	//设置结点的左孩子
	void setSibling(TreeNode<T>* pointer);	//设置右兄弟
	void InsertFirst(TreeNode<T>* node);	//以第一个左孩子的身份插入结点
	void InsertNext(TreeNode<T>* node);	//以右兄弟的身份插入结点
};
/*
template<class T>
TreeNode<T>::TreeNode(const TreeNode<T>& rhs)
{
	m_Value = rhs.m_Value;
	
}
*/
/*
template<class T>
TreeNode::TreeNode(const T& value,std::vector<T> shuxing):tags(shuxing)
{
	m_Value = value;
	pChild = NULL;
	pSibling = NULL;
}
*/
/*-----------------implement-------------------*/
template<class T>
TreeNode<T>::TreeNode(const T& value)
{
	m_Value = value;
	pChild = NULL;
	pSibling = NULL;
}

template<class T>
bool TreeNode<T>::isleaf()
{
	if(NULL==pChild)
		return true;
	return false;
}


template<class T>
T TreeNode<T>::value()
{
	return m_Value;
}


template<class T>
TreeNode<T>* TreeNode<T>::leftMostChild()
{
	return pChild;
}


template<class T>
TreeNode<T>* TreeNode<T>::RightSibling()
{
	return pSibling;
}

template<class T>
void TreeNode<T>::setValue(T& value)
{
	m_Value = value;
}

template<class T>
void TreeNode<T>::setChild(TreeNode<T>* pointer)
{
	pChild = pointer;
}


template<class T>
void TreeNode<T>::setSibling(TreeNode<T>* pointer)
{
	pSibling = pointer;
}

template<class T>
void TreeNode<T>::InsertFirst(TreeNode<T>* node)
{
	if(pChild)
		node->pSibling = pChild;
	pChild = node;
}

template<class T>
void TreeNode<T>::InsertNext(TreeNode<T>* node)
{
	if(pSibling)
		node->pSibling = pSibling;
	pSibling = node;
}
