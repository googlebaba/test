#include <vector>
template <class T>class Tree;

template<class T>
#pragma once
class TreeNode{
	friend class Tree<T>;
private:
	T m_Value;					//������ֵ
	TreeNode<T>* pChild;		//���ӽ��
	TreeNode<T>* pSibling;		//���ֵܽ��
//	std::vector<T> tags;      //����˵㻹Ҫ���������ֵ
public:
//	TreeNode(const TreeNode&);   //���ƹ��캯��
	TreeNode(const T& value);			//���캯��
//	TreeNode(const T& value,std::vector<T> shuxing); //���캯��
	virtual ~TreeNode(){}
	bool isleaf();				 //��������Ҷ������true;
	T value();					//���ؽ���ֵ
	TreeNode<T>* leftMostChild();			//���ص�һ������
	TreeNode<T>* RightSibling();			//�������ֵ�
	void setValue(T&);						//���ý���ֵ
	void setChild(TreeNode<T>* pointer);	//���ý�������
	void setSibling(TreeNode<T>* pointer);	//�������ֵ�
	void InsertFirst(TreeNode<T>* node);	//�Ե�һ�����ӵ���ݲ�����
	void InsertNext(TreeNode<T>* node);	//�����ֵܵ���ݲ�����
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
