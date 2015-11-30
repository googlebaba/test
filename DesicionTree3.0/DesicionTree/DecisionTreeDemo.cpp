/*
* FILE: DecisionTreeDemo.cpp
* Date: 2015-11-30
* Author: shaohua Fan
* Modifier: shaohua Fan    E-mail:835705572@qq.com
*
* DISCLAIMER: No liability is assumed by the author for any use made
* of this program.
*
* DISTRIBUTION: Any use may be made of this program, as long as the
* clear acknowledgment is made to the author in code and runtime
* executables
*/
#include "DecisionTree.h"
using namespace std;

ifstream& open_file(ifstream& in,std::string filename)
{
	in.close();
	in.clear();
	in.open(filename.c_str());
	return in;
}

vector<string> get_attribute(ifstream& in) //��ȡ����������ֵ
{
	vector<string>attribute;
	string line;
	getline(in,line);
	string word;
	stringstream stream(line.c_str());
	while(stream>>word){
		attribute.push_back(word);
	}
	return attribute;
}
/*
vector<map<string,vector<pair<string,string> > > > YangBen(ifstream& in)
{
	vector<string>attribute_name;   //���������е�����
	vector<map<string,vector<pair<string,string> > > > example;  //�������е�����
	string line1;
	getline(in,line1);//��ȡ�ı��ĵ�һ����������Ϊ����ֵ
	string word;
	stringstream stream(line1.c_str());
	while(stream>>word)
	{
		attribute_name.push_back(word);
	}
	string line;
	
	while(getline(in,line))
	{
		vector<string>::iterator att_iter = attribute_name.begin();
		string keyword;
		map<string,vector<pair<string,string> > > example_attri;//ÿһ������
		vector<pair<string,string> > attribute;//����ÿ��������ȥ�ؼ��ֵ�����ֵ
		vector<string>::size_type count_word = 0;//��ǰΪ�ڼ�������ֵ
		string word;
		istringstream stream(line.c_str());
		while(stream>>word)
		{		
			if(0==count_word)
			{
				keyword = word;
			}else
			{
				attribute.push_back(make_pair(*(att_iter+count_word),word));//����ÿһ�г�ȥ�ؼ��ֵ�����ֵ
			}
			++count_word;
		}
		example_attri.insert(make_pair(keyword,attribute));//����һ������������
		example.push_back(example_attri);
	}
	return example;
}
*/
vector<vector<pair<string,string> > > YangBen(ifstream& in,vector<string> attribute)
{
	vector<vector<pair<string,string> > > example;  //�������е�����
	string line;
	while(getline(in,line))
	{
		vector<string>::iterator att_iter = attribute.begin();    //���Ա�Ŀ�ͷ
		vector<pair<string,string> > attribute_value;//����ÿ������������ֵ
		string word;
		vector<string>::size_type count_word = 0;//��ǰΪ�ڼ�������ֵ
		istringstream stream(line.c_str());
		while(stream>>word)
		{		
			attribute_value.push_back(make_pair(*(att_iter+count_word),word));//����ÿһ�г�ȥ�ؼ��ֵ�����ֵ
			++count_word;
		}
		example.push_back(attribute_value);
	}
	return example;
}
template<class T>
void printTree(TreeNode<T>* root,int blk,ofstream& outfile)  //��������,��һ������Ϊ���ĸ���㣬�ڶ�������Ϊ��������
{
	using std::queue;
	queue<TreeNode<string>*>aQueue;
	TreeNode<string>* pointer = root;
	for(int i =0;i!=blk;++i)cout<<"\t";    //����
	cout<<"|-"<<root->value()<<endl;
	for(int i =0;i!=blk;++i)outfile<<"\t";    //����
	outfile<<"|-"<<root->value()<<endl;
	pointer = pointer->leftMostChild(); //ָ������
	while(pointer)
	{
		aQueue.push(pointer);    //����ѹ��ջ
		pointer = pointer->RightSibling();    //ָ���ֵ�
	}
	while(!aQueue.empty()){
		pointer = aQueue.front();
		aQueue.pop();
		printTree(pointer,blk+1,outfile);
	}
	return;
}
int main()
{
	string file1("Attribute.txt");
	string file2("Example.txt");
	ifstream infile;
	open_file(infile,file1);
	if(!infile)
	{
		cerr<<"error:unable to open input file:"
			<<file1<<endl;
		return -1;
	}
	vector<string>Attribute_name;//��ȡ����ֵ
	Attribute_name = get_attribute(infile);
	open_file(infile,file2);
	vector<vector<pair<string,string> > > example = YangBen(infile,Attribute_name);//��ȡ����
	DiscionTree aTree(example,Attribute_name);
	//Tree<std::string>* Dtree = new Tree<string>();
	Tree<string>* Dtree = aTree.ID3(example,"PlayTennis",Attribute_name);
	ofstream outfile;
	outfile.open("tree.txt");
	printTree(Dtree->getRoot(),0,outfile);   //�������õ�����
	delete Dtree;
//	double Gain = Tree.Gain(example,"Wind");
	return 0;
}

/*----------------������----------------------*/
/*
|-Outlook
        |-Sunny
                |-Humidity
                        |-Normal
                                |-+
                        |-High
                                |--
        |-Rain
                |-Wind
                        |-Weak
                                |-+
                        |-Strong
                                |--
        |-Overcast
                |-+
�밴���������. . .
*/

