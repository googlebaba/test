#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
vector<string> transform(queue<string>& information);
int main()
{
	vector<string>lines;
	int n;
	//cout<<"Enter how many lines you want to transform"<<endl;
	cin>>n;
	cin.clear();
	string line;
	queue<string>information;//存储要传输的文字
	//cout<<"Enter lines you want to transform"<<endl;
	getline(cin,line);
	while(n!=0&&getline(cin,line))
	{
		information.push(line);
		--n;
	}
	lines = transform(information);
	vector<string>::iterator iter = lines.begin();
	while(iter!=lines.end())
		cout<<*iter++<<endl;
	return 0;
}

vector<string> transform(queue<string>& information)
{
	vector<string>lines;
	while(!information.empty())
	{
		string line = information.front();
		information.pop();
		for(string::size_type ix = 0;ix!=line.size();++ix)
		{
			if(line[ix]=='z')
				line[ix] = 'a';
			else if(line[ix]=='Z')
				line[ix] = 'A';
			else if(line[ix]=='!'||line[ix]==' ')
				continue;
			else line[ix] = static_cast<char>(line[ix]+1);
		}
		lines.push_back(line);
	}
	return lines;
}