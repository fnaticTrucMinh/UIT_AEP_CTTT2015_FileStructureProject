/* 
__FUNCTIONAL REQUIREMENT
	Read data from “Original_File.txt”.
	Build Huffman code and save it in "Huffman_Code.txt".
	Encode data of “Original_File.txt” and save it in “Huffman_Pack.txt”.
	Decode data of “Huffman_Pack.txt” and save it in “Huffman_Unpack.txt”
__MAIN FUNCTION :
	Encode
	Decode
**Signed by Nguyen Minh Truc.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std;
//Initializing variables include : a node and a vector of node.
ifstream inp;
ofstream out;

struct HMNode {
	int freq=1;
	char character='*';
	HMNode *left=NULL;
	HMNode *right=NULL;
};

vector<HMNode> nodeList;
HMNode *tree = NULL;
int charLength;
char character[255];
int freq[255];
string code[255];

void Sort();
string GetHuffmanCode(char k);
//step 1 : count frequency of character during process read from file 
void ReadFromFile()
{
	//open file Original_File.txt.
	inp.open("Original_File.txt");
	HMNode temp; bool flag;
	while (!inp.eof())
	{
		inp.get(temp.character);//get a single char
		flag = false;
		for (int i = 0; i < nodeList.size(); i++)
		{
			if (temp.character == '\0' )
			{
				flag = true;
				continue;
			}

			if (temp.character == nodeList[i].character)
			{
				flag = true;
				nodeList[i].freq += 1;
				continue;
			}
		}
		/*HMNode no;
		no.character = temp->character;
		no.freq = temp->freq;
		no.left = NULL;
		no.right = NULL;*/
		if (!flag)
			nodeList.push_back(temp);
	}
	Sort();
	charLength = nodeList.size();
	for (int i = 0; i < charLength; i++)
	{
		character[i] = nodeList[i].character;
		freq[i] = nodeList[i].freq;
	}
	inp.close();
}
//step 2 : sort 
void Sort()
{
	sort(nodeList.begin(), nodeList.end(), [](const HMNode& lhs, const HMNode& rhs)
	{
		return lhs.freq > rhs.freq;
	});
}

//step 3   : build huffman tree.
	// 3.1 : get total frequences of the 2 smallest node amd store by a new node.
	// 3.2 : replace current node by a new node.
	// 3.3 : resort the array.

HMNode *CreateNewNode(HMNode node)
{
	HMNode *newNode = new HMNode;
	newNode->freq = node.freq;
	newNode->character = node.character;
	newNode->left = node.left;
	newNode->right = node.right;
	return newNode;
}

HMNode constructHuffmanTree()
{
	//merge node until it become a tree
	while (nodeList.size() != 1)
	{
		/*HMNode *newNode = &nodeList[nodeList.size() - 1];
		HMNode *nNode = &nodeList[nodeList.size() - 2];
		HMNode node;*/
		
		/*AssignValue(newNode, &nodeList[nodeList.size() - 1]);
		AssignValue(nNode, &nodeList[nodeList.size() - 2]);

		node = CreateNewNode(newNode->freq + nNode->freq);
		node.left = newNode;
		node.right = nNode;

		nodeList[nodeList.size() - 2] = node;*/

		nodeList[nodeList.size() - 2].right = CreateNewNode(nodeList[nodeList.size() - 2]);
		nodeList[nodeList.size() - 2].left = CreateNewNode(nodeList[nodeList.size() - 1]);

		nodeList[nodeList.size() - 2].freq = nodeList[nodeList.size() - 2].freq + nodeList[nodeList.size() - 1].freq;
		nodeList[nodeList.size() - 2].character = '*';

		//delete the useless one
		nodeList.pop_back();

		//reorder the tree
		Sort();
	}
	return nodeList[0];
}

string printPostorder(struct HMNode *tree,char k)
{

	string cod = "";
	if (tree->left && tree->right)
	{
		if (tree->right && tree->right->character == k)
			return "1";
		if (tree->left && tree->left->character == k)
			return "0";

		cod = printPostorder(tree->right,k);
		if (cod != "")
			return "1" + cod;

		cod = printPostorder(tree->left,k);
		if (cod != "")
			return "0" + cod;
	}
	return cod;

}

//step 4 : assign huffman code and print to "Huffman_Code.txt"

void AssignHuffmanCode()
{
	out.open("Huffman_Code.txt");
	for (int i = 0; i < charLength; i++)
	{
		code[i] = GetHuffmanCode(character[i]);
		out << character[i] << " : " << freq[i] << " : " << code[i] << endl;
	}

	out.close();
}

//step 5 : Encode
void HMEncode()
{
	out.open("Huffman_Pack.txt");
	inp.open("Original_File.txt");
	char temp;
	while (!inp.eof())
	{
		inp.get(temp);
		if (temp == '\0')
			continue;
		out << GetHuffmanCode(temp);
	}
	inp.close();
	out.close();
}
//step 6 : Decode.
void HMDecode()
{
	out.open("Huffman_Unpack.txt");
	inp.open("Huffman_Pack.txt");
	string temp;
	while (!inp.eof())
	{
		getline(inp,temp);
	}
	string ans = "";
	struct HMNode* curr = tree;
	for (int i = 0; i<temp.size(); i++)
	{
		if (temp[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		// reached leaf node
		if (curr->left == NULL and curr->right == NULL)
		{
			ans += curr->character;
			curr = tree;
		}
	}
	// cout<<ans<<endl;
	out<< ans + '\0';
	inp.close();
	out.close();
}
string  GetHuffmanCode(char k)
{
	return printPostorder(tree, k);
}
int main()
{
	//Initial
	ReadFromFile();
	//Build Huffman Tree
	tree = &constructHuffmanTree();
	//Get HuffmanCode
	AssignHuffmanCode();
	//Encode 
	HMEncode();
	//Decode
	HMDecode();
	//Stop the console
	cout << "Done!!\n";
	system("pause");
	return 0;
}