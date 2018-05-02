/*
	DEFRAGMENT then WRITEFILE

*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;
void Init();
void ReadFile();
void WriteFile();
void AddRecord(string data);
void DeleteRecord(string data);
void Defragment();

struct FLNode
{
	int RRNs=0;//relative record numbers
 	string data;
	//FLNode *next;
};

string comma[10];
vector<FLNode> lists;
bool IsEmpty = true;
ifstream input;
ofstream output;

FLNode NewNode(string data,int rrns)
{
	FLNode node;
	node.data = data;
	node.RRNs = rrns;
	return node;
}

void InitComma()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < i; j++)
			comma[i] = comma[i]+ '.';
	}
}

void ReadFile()
{
	string temp;
	ifstream inp;
	inp.open("FileWrite.txt");
	while (!inp.eof())
	{
		getline(inp, temp);
		cout << temp << endl;
	}
	inp.close();
}

void WriteFile()
{
	output.open("FileWrite.txt");
	for (int i = 0; i < lists.size(); i++)
	{
		if (lists[i].RRNs != 0)
			output << lists[i].RRNs << lists[i].data << comma[10 - lists[i].data.length()];
		else
			output << lists[i].data << comma[10 - lists[i].data.length()];
	}
	output.close();
}

int GetRRNs(string s)
{
	string temp;
	int res=0;
	for (int i = 1;; i++)
	{
		if (!isalpha(s[i]))
			temp += s[i];
		else
			break;
	}
	stringstream geek(temp);
	geek >> res;
	return res;
}

void Defragment()
{
	if (lists[0].RRNs == -1)
		return;
	int i = 0;
	while (1<2)
	{
		if (lists[i].data[0] == '*')
		{
			lists.erase(lists.begin() + i);
			i -= 1;
		}
		if (i >= lists.size()-1) break;
		i++;
	}
	lists[0].RRNs = -1;
}

void AddRecord(string data)
{
	if (IsEmpty)
	{
		lists.push_back(NewNode(data, -1));
		IsEmpty = false;
	}
	else
	{
		if (lists[0].RRNs == -1)
			lists.push_back(NewNode(data, 0));
		else
		{
			string dat = lists[lists[0].RRNs].data;
			lists[lists[0].RRNs] = NewNode(data, 0);
			lists[0].RRNs = GetRRNs(dat);
		}
	}
}

void del(string& data,int rrns)
{
	data.erase(0, 2);
	data.insert(0, '*'+std::to_string(rrns));
}

void DeleteRecord(string data)
{
	for (int i = 0; i < lists.size(); i++)
	{
		if (lists[i].data == data)
		{
			//Convert into deleted Record with RRNS of the first Node
			del(data,lists[0].RRNs);
			lists[i].data = data;
			//assign RRNS of the current to the first Node
			lists[0].RRNs = i;
			break;
		}
	}
}

void Init()
{	
	string temp;
	input.open("Case1.txt");
	//input.open("Case2.txt");
	while (!input.eof())
	{
		getline(input, temp);
		switch (temp[0]) {
		case '+' : temp = temp.erase(0, 1);
				  AddRecord(temp);
				  break;
		case '-' : temp = temp.erase(0, 1);
				  DeleteRecord(temp);
				  break;
		case 'R': ReadFile();
				  break;
		case 'W': Defragment();
				  WriteFile(); 
				  break;
		case 'D': Defragment(); 
				  break;
		}
	}
	input.close();
}

int main()
{
	InitComma();
	Init();
	system("pause");
	return 0;
}