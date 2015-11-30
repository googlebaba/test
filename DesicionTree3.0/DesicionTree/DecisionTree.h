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
	void printTree();              //画决策树的函数
	//TreeNode<std::string> general_tree();    //产生一颗决策树
	Tree<std::string>* ID3(Example_vec Example,std::string Target_attribute,std::vector<std::string> Attribute);
private:
	//Example为训练样集，Target_attribute是这棵树要预测的目标属性，Attribute是除目标属性供学习到的决策树测试的属性列表。返回一颗能正确分类给定Example的决策树
	
	std::set<std::string> attribute_value(Example_vec example,const std::string&)const;
	Example_vec select_exp(Example_vec,const std::string&,const std::string&);//返回符合条件的样本集，第一个参数为候选样本集，第二个参数为属性名称，第三个参数为属性值
	std::string Max_Gain(const Example_vec&,std::vector<std::string>&);         //找到最大的信息增益
	double Entropy(const double&,const double&)const;   //熵的计算公式
	double Gain(Example_vec,const std::string&);    //计算属性的信息增益
	std::pair<int,int> zhengfan(const Example_vec&,const std::string&)const;              //判断一个样本集的正反个数pair第一个参数为正数的，第二个参数为复数的
	Example_vec example_vector;//样本
	int count_day;
	std::vector<std::string>attribute;   //样本属性值
};

