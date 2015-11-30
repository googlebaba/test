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

vector<string> get_attribute(ifstream& in) //获取样本的属性值
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
	vector<string>attribute_name;   //保存属性列的名字
	vector<map<string,vector<pair<string,string> > > > example;  //保存所有的样例
	string line1;
	getline(in,line1);//读取文本的第一条，并保存为属性值
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
		map<string,vector<pair<string,string> > > example_attri;//每一条样例
		vector<pair<string,string> > attribute;//保存每个样例除去关键字的属性值
		vector<string>::size_type count_word = 0;//当前为第几个属性值
		string word;
		istringstream stream(line.c_str());
		while(stream>>word)
		{		
			if(0==count_word)
			{
				keyword = word;
			}else
			{
				attribute.push_back(make_pair(*(att_iter+count_word),word));//保存每一行除去关键字的属性值
			}
			++count_word;
		}
		example_attri.insert(make_pair(keyword,attribute));//保存一条完整的样本
		example.push_back(example_attri);
	}
	return example;
}
*/
vector<vector<pair<string,string> > > YangBen(ifstream& in,vector<string> attribute)
{
	vector<vector<pair<string,string> > > example;  //保存所有的样例
	string line;
	while(getline(in,line))
	{
		vector<string>::iterator att_iter = attribute.begin();    //属性表的开头
		vector<pair<string,string> > attribute_value;//保存每个样例的属性值
		string word;
		vector<string>::size_type count_word = 0;//当前为第几个属性值
		istringstream stream(line.c_str());
		while(stream>>word)
		{		
			attribute_value.push_back(make_pair(*(att_iter+count_word),word));//保存每一行除去关键字的属性值
			++count_word;
		}
		example.push_back(attribute_value);
	}
	return example;
}
template<class T>
void printTree(TreeNode<T>* root,int blk,ofstream& outfile)  //画树函数,第一个参数为树的根结点，第二个参数为缩进次数
{
	using std::queue;
	queue<TreeNode<string>*>aQueue;
	TreeNode<string>* pointer = root;
	for(int i =0;i!=blk;++i)cout<<"\t";    //缩进
	cout<<"|-"<<root->value()<<endl;
	for(int i =0;i!=blk;++i)outfile<<"\t";    //缩进
	outfile<<"|-"<<root->value()<<endl;
	pointer = pointer->leftMostChild(); //指向左孩子
	while(pointer)
	{
		aQueue.push(pointer);    //孩子压入栈
		pointer = pointer->RightSibling();    //指向兄弟
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
	vector<string>Attribute_name;//读取属性值
	Attribute_name = get_attribute(infile);
	open_file(infile,file2);
	vector<vector<pair<string,string> > > example = YangBen(infile,Attribute_name);//读取样例
	DiscionTree aTree(example,Attribute_name);
	//Tree<std::string>* Dtree = new Tree<string>();
	Tree<string>* Dtree = aTree.ID3(example,"PlayTennis",Attribute_name);
	ofstream outfile;
	outfile.open("tree.txt");
	printTree(Dtree->getRoot(),0,outfile);   //画出所得到的树
	delete Dtree;
//	double Gain = Tree.Gain(example,"Wind");
	return 0;
}

/*----------------输出结果----------------------*/
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
请按任意键继续. . .
*/

