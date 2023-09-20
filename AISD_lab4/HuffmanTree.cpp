#include "HuffmanTree.h"
#include <algorithm>

Node::Node() :
	m_frequency(0), _left(nullptr),
	_right(nullptr), m_symbols("")
{}

Node::Node(unsigned char symbol, int frequency)
{
	m_symbols = symbol;
	m_frequency = frequency;
	_left = nullptr;
	_right = nullptr;
}

Node::Node(Node* left, Node* right)
{
	m_symbols = left->m_symbols + right->m_symbols;
	m_frequency = left->m_frequency + right->m_frequency;
	_left = left;
	_right = right;
}

Node::~Node()
{

}


Node* Node::left()
{
	return _left;
}

Node* Node::right()
{
	return _right;
}

int Node::frequency()
{
	return m_frequency;
}

std::string Node::symbols()
{
	return m_symbols;
}


HuffmanTree::HuffmanTree()
{
	_root = nullptr;
}

HuffmanTree::~HuffmanTree()
{
	clear();
}

void HuffmanTree::clear()
{
	return clear(_root);
}

void HuffmanTree::clear(Node* nd)
{
	if (nd)
	{
		clear(nd->right());
		clear(nd->left());
		delete nd;
	}
}

void HuffmanTree::build(std::string strIn)
{
	clear(_root);
	int TAB[256];
	for (int i = 0; i < 256; i++)
		TAB[i] = 0;

	for (int i = 0; i < strIn.size(); i++)
		TAB[strIn[i]]++;


	std::vector<Node*> nodes;

	for (int i = 0; i < 256; i++) 
		if (TAB[i] != 0) 
		{
			Node* temp = new Node((unsigned char)i, TAB[i]);
			nodes.push_back(temp);
			//std::cout << temp->symbols() << ' ' << temp->frequency() << '\n';
		}


	std::sort(nodes.begin(), nodes.end(),
		[](Node* a, Node* b)
		{	
			return a->frequency() < b->frequency();
		}
	);

	/*for (int i = 0; i < nodes.size(); i++)
	{
		std::cout << nodes[i]->symbols() << ' ' << nodes[i]->frequency() << '\n';
	}*/

	while (nodes.size() >= 2)
	{
		Node* temp = new Node(nodes[0], nodes[1]);
		nodes.erase(nodes.begin());
		nodes.erase(nodes.begin());
		nodes.push_back(temp);

		std::sort(nodes.begin(), nodes.end(),
			[](Node* a, Node* b)
			{
				return a->frequency() < b->frequency();
			}
		);

	}
	


	
	_root = nodes[0];
	//std::cout << _root->symbols() << ' ' << _root->frequency() << '\n';


}


double HuffmanTree::encode(std::string strIn, std::string& strOut)
{
	build(strIn);

	strOut = "";

	int resultCountOfBits = 0;


	for (int i = 0; i < strIn.size(); i++)
	{
		Node* temp = _root;
		while (temp->left() && temp->right())
		{
			if (temp->left()->symbols().find(strIn[i]) != std::string::npos)
			{
				temp = temp->left();
				strOut += '0';
			}
			else
			{
				temp = temp->right();
				strOut += '1';
			}
			resultCountOfBits++;

		}
	}

	

	/*if (_root->left() == nullptr && _root->right() == nullptr)
		std::cout << "Only one symbol: " << strIn;*/

	if (_root->left() == nullptr && _root->right() == nullptr)
	{
		for (int i = 0; i < strIn.size(); i++)
			strOut += '0';

		resultCountOfBits = strIn.size();
	}

	//std::cout << "result: " << resultCountOfBits << "input: " << strIn.size() << " strOut: " << strOut << '\n';


	double a = (double)resultCountOfBits / (double)(strIn.size() * 8);
	//std::cout << a;
	return  a;


}

bool HuffmanTree::decode(std::string strIn, std::string& strOut)
{
	if (!_root)
		return false;


	Node* temp = _root;
	strOut = "";
	for (int i = 0; i < strIn.size(); i++)
	{
		if (strIn[i] == '0' && temp->left())
			temp = temp->left();
		else
			if (strIn[i] == '1' && temp->right())
				temp = temp->right();
			else
				if(temp->left() || temp->right())
					return false;

		
		
		if (!temp->left() && !temp->right())
		{
			//std::cout << "temp frequecy = " << temp->frequency() << " temp symbols = " << temp->symbols() << '\n';
			strOut += temp->symbols();
			temp = _root;
		}
	}

	//std::cout << "strOut: " << strOut;
	return true;

}


void HuffmanTree::printTree()
{
	return printTree(_root);
}


void HuffmanTree::printTree(Node* nd)
{
	if (nd == nullptr)
	{
		std::cerr << "Tree is empty";
		return;
	}

	int height = getHeight(nd);

	for (int level = 0; level < height; level++)
	{
		//std::cout << level << '\n';
		std::vector<Node*> currentLevel;
		currentLevel.push_back(_root);

		for (int i = 0; i < level; i++)
		{
			std::vector<Node*> nextLevel;
			nextLevel.reserve(currentLevel.size() * 2);

			for (Node* Node : currentLevel)
			{
				if (Node)
				{
					nextLevel.push_back(Node->left());
					nextLevel.push_back(Node->right());
				}
				else
				{
					nextLevel.push_back(nullptr);
					nextLevel.push_back(nullptr);
				}
			}

			currentLevel.swap(nextLevel);
		}

		for (int i = 0, tabs = 6; i < currentLevel.size(); i++, tabs--)
		{
			if (currentLevel[i])
				std::cout << "("<<  currentLevel[i]->symbols() << ", " << currentLevel[i]->frequency() << ") ";
			else
				std::cout << "-" << ' ';
		}
		std::cout << '\n';
	}

}

int HuffmanTree::getHeight() const
{
	return getHeight(_root);
}

int HuffmanTree::getHeight(Node* nd) const
{
	if (nd == nullptr)
		return 0;

	if ((nd->left() == nullptr) && (nd->right() == nullptr))
		return 1;

	int leftHeight = 0, rightHeight = 0;

	if (nd->left())
		leftHeight = getHeight(nd->left());

	if (nd->right())
		rightHeight = getHeight(nd->right());

	int height;

	if (rightHeight > leftHeight)
		height = rightHeight;
	else
		height = leftHeight;

	return height + 1;
}

