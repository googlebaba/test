#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <cmath>
#include "Tree.h"
class DiscionTree
{
public:
	typedef std::vector<std::vector<std::pair<std::string,std::string> > >  Example_vec;
//	typedef std::map<std::string,std::vector<std::pair<std::string,std::string> > >  Example_map;
	typedef std::vector<std::pair<std::string,std::string> > Example_vec_inner;
	DiscionTree(const Example_vec&,const std::vector<std::string>&);
	void printTree();              //���������ĺ���
	//TreeNode<std::string> general_tree();    //����һ�ž�����
	Tree<std::string>* ID3(Example_vec Example,std::string Target_attribute,std::vector<std::string> Attribute);
private:
	//ExampleΪѵ��������Target_attribute�������ҪԤ���Ŀ�����ԣ�Attribute�ǳ�Ŀ�����Թ�ѧϰ���ľ��������Ե������б�����һ������ȷ�������Example�ľ�����
	
	std::set<std::string> attribute_value(Example_vec example,const std::string&)const;
	Example_vec select_exp(Example_vec,const std::string&,const std::string&);//���ط�������������������һ������Ϊ��ѡ���������ڶ�������Ϊ�������ƣ�����������Ϊ����ֵ
	std::string Max_Gain(const Example_vec&,std::vector<std::string>&);         //�ҵ�������Ϣ����
	double Entropy(const double&,const double&)const;   //�صļ��㹫ʽ
	double Gain(Example_vec,const std::string&);    //�������Ե���Ϣ����
	std::pair<int,int> zhengfan(const Example_vec&,const std::string&)const;              //�ж�һ������������������pair��һ������Ϊ�����ģ��ڶ�������Ϊ������
	Example_vec example_vector;//����
	int count_day;
	std::vector<std::string>attribute;   //��������ֵ
};

