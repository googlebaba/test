#include "DecisionTree.h"
using namespace std;
DiscionTree::DiscionTree(const Example_vec& rhs,const vector<string>& lhs):example_vector(rhs),attribute(lhs)//�洢�������������
{ 
	count_day = example_vector.size();
}
double chengfa(const double& a,const double&log)
{
	if(0.0==a)
		return 0.0;
	return a*log;
}
double logbase(double a, double base)
{
	return log(a) / log(base);
}
double DiscionTree::Entropy(const double& a,const double& b)const
{	
	double sum = a+b;
	double result;
	double a_result = -chengfa(a/sum,logbase(a/sum,2));
	double b_result = -chengfa(b/sum,logbase(b/sum,2));
	return result = a_result+b_result;
}

double DiscionTree::Gain(Example_vec Ex_list,const std::string& attr)
{
	multimap<string,string>attr_multimap;  //
	int S_size = Ex_list.size();     //��¼S�ж���������
	int s_yes_num = 0,s_no_num = 0;
	int att_yes_num = 0,att_no_num = 0;
	Example_vec::iterator vec_iter = Ex_list.begin();
	set<string>attr_value;//���Ե�ֵ
	for(;vec_iter!=Ex_list.end();++vec_iter)
	{
		string attri_first;//�������Ե�ֵ
		string playTennis;//�����Ӧ�Ƿ�ȥ������
		//Example_map::iterator iter_map = vec_iter->begin();
		Example_vec_inner::iterator iter_vec_inner = vec_iter->begin();
		for(;iter_vec_inner!=vec_iter->end();++iter_vec_inner)
		{
			if(iter_vec_inner->first==attr)
			{
				attr_value.insert(iter_vec_inner->second);
				attri_first = iter_vec_inner->second;
			}else if(iter_vec_inner->first=="PlayTennis"){
				playTennis = iter_vec_inner->second;
				if(iter_vec_inner->second=="Yes")
					++s_yes_num;
				else if(iter_vec_inner->second=="No")
					++s_no_num;
			}
		}
		attr_multimap.insert(make_pair(attri_first,playTennis)); //�����Ե�ֵ��PlayTennis��Pair����ʽ�洢
	}
	vector<vector<int> > attr_val_vec;
	set<string>::iterator iter_set = attr_value.begin();
	for(;iter_set!=attr_value.end();++iter_set)
	{
		int attr_val_yes = 0;
		int attr_val_no = 0;
		vector<int>attr_val_num;
		typedef multimap<string,string>::size_type sz_type;
		typedef multimap<string,string>::iterator multi_iter;
		sz_type entries = attr_multimap.count(*iter_set);   //��¼����ֵ�м���
		pair<multi_iter,multi_iter> attr_val_val = attr_multimap.equal_range(*iter_set);
		while(attr_val_val.first!=attr_val_val.second)
		{
			if(attr_val_val.first->second=="Yes")
				++attr_val_yes;
			else if(attr_val_val.first->second=="No")
				++attr_val_no;
			++attr_val_val.first;
		}
		attr_val_num.push_back(entries);
		attr_val_num.push_back(attr_val_yes);
		attr_val_num.push_back(attr_val_no);
		attr_val_vec.push_back(attr_val_num);
	}
	typedef vector<vector<int> >::iterator vec_vec_iter;
	vec_vec_iter val_iter = attr_val_vec.begin();
	vector<double>Entropy_attr;
	for(;val_iter!=attr_val_vec.end();++val_iter)
	{
		vector<int>::iterator vec_iter = val_iter->begin();
		double en_result = Entropy(*(vec_iter+1),*(vec_iter+2));
		double result = static_cast<double>(*vec_iter)/static_cast<double>(S_size)*en_result;
		Entropy_attr.push_back(result);
	}
	vector<double>::iterator val_En = Entropy_attr.begin();
	double result = Entropy(s_yes_num,s_no_num);
	for(;val_En!=Entropy_attr.end();++val_En)
	{
		result -= *val_En;
	}
	return result;
}

std::string DiscionTree::Max_Gain(const Example_vec& rhs,std::vector<string>& Attribute)
{
	string max_string;
	double max_Gain = 0;
	vector<string>::iterator iter = Attribute.begin();
	int n = 0;    //���ڼ���
	for(int count=0;iter!=Attribute.end();++iter,++count)
	{
		double gain = Gain(rhs,*iter);
		if(gain>max_Gain)
		{
			max_Gain = gain;
			max_string = *iter;     //��¼��������
			n = count;				//��¼�ڼ����������
		}
	}
	iter = Attribute.begin()+n;
	Attribute.erase(iter);
	return max_string;
}
/*
void DiscionTree::printTree()
{
	string root = Max_Gain(example_vector);     //�ҵ���Ϣ�������ĵ�Ϊ��
	Tree<string> aTree;
	aTree.CreateRoot(root);					//����һ����rootΪ���Ľ��
}
*/
std::set<string> DiscionTree::attribute_value(Example_vec example,const std::string& attri_name) const
{
	Example_vec::const_iterator vec_iter = example.begin();
	set<string>attr_value;//���Ե�ֵ
	for(;vec_iter!=example.end();++vec_iter)
	{
		vector<pair<string,string> >::const_iterator iter = vec_iter->begin();
		for(;iter!=vec_iter->end();++iter)
		{
			if(iter->first==attri_name)          //����ֵ��Ҫ���ҵ�����ֵ���
			{
				attr_value.insert(iter->second);
			}
		}
	}
	return attr_value;
}

Tree<string>* DiscionTree::ID3(Example_vec Example,std::string Target_attribute,std::vector<string> Attribute)
{
	Tree<string> *aTree = new Tree<string>();    //����һ����
	pair<int,int>zf = zhengfan(Example,Target_attribute);
	if(zf.first==Example.size())//���Example��Ϊ������ô����label = +�ĵ������Root
	{
		aTree->CreateRoot("+");
		return aTree;
	}else if(zf.second==Example.size())//���Example��Ϊ������ô����label = -�ĵ������Root
	{
		aTree->CreateRoot("-");
		return aTree;
	}else if(Attribute.empty()){  //���AttributeΪ�գ���ô���ص������Root,label=Example�����ձ��Target_attribute;
		string str = zf.first>=zf.second ? "+":"-";
		aTree->CreateRoot(str);
		return aTree;
	}
	string root = Max_Gain(Example,Attribute);     //�ҵ���Ϣ�������ĵ�Ϊ��,�����������б���ɾ����������ֵ
	aTree->CreateRoot(root);
	set<string>value = attribute_value(Example,root);   //����ÿ�����Կ��ܵ�ֵ
	set<string>::const_iterator iter = value.begin();    
	for(;iter!=value.end();++iter){
		TreeNode<string>* child = new TreeNode<string>(*iter);       //����һ���ֽ��
		aTree->getRoot()->InsertFirst(child);              //����Ϊ�ӽ��
		Example_vec tmp_exp = select_exp(Example,root,*iter);//ȡ��examplesVi
		Tree<string>* temp_tree = new Tree<string>();
		temp_tree = ID3(tmp_exp,"PlayTennis",Attribute);   //����������
		child->setChild(temp_tree->getRoot());    //�Ѳ��������ĸ���Ϊ���ӵĺ���
	}
	return aTree;
}



pair<int,int> DiscionTree::zhengfan(const Example_vec& rhs,const string& str) const
{
	int positive = 0;
	int negative = 0;
	vector<vector<pair<string,string> > >::const_iterator iter = rhs.begin();
	for(;iter!=rhs.end();++iter){
		vector<pair<string,string> >::const_iterator iter1 = iter->begin();
		for(;iter1!=iter->end();++iter1){
			if(iter1->first==str)
				if(iter1->second=="Yes")
					++positive;
				else
					++negative;
		}
	}
	return make_pair(positive,negative);
}

DiscionTree::Example_vec DiscionTree::select_exp(Example_vec exp,const std::string& tags,const std::string& val)
{
	Example_vec tmp;            //���ڱ���Ҫ���ص�����
	Example_vec::iterator iter1 = exp.begin();    //���������ĵ�����
	for(;iter1!=exp.end();++iter1){
		vector<pair<string,string> >::iterator iter2 = iter1->begin();
		for(;iter2!=iter1->end();++iter2){
			if(iter2->first==tags&&iter2->second==val)      //�ҵ����Ժ�ֵ��������������
				tmp.push_back(*iter1);
		}
	}
	return tmp;
}