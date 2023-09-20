#pragma once
#include<string>
#include <iostream>
#include <vector>

class Node
{
public:
	Node();
	~Node();
	Node(unsigned char symbol, int freq);
	Node(Node* left, Node* right);
	Node* right();
	Node* left();
	int frequency();
	std::string symbols();

protected:

	std::string m_symbols;
	int m_frequency = 0;
	Node* _left = nullptr;
	Node* _right = nullptr;
};

class HuffmanTree :public Node
{
public:
	HuffmanTree();
	~HuffmanTree();
	void clear();
	void clear(Node* nd);
	void build(std::string strIn);
	double encode(std::string strIn, std::string& strOut);
	bool decode(std::string strIn, std::string& strOut);
	void printTree();
	void printTree(Node* nd);
	int getHeight() const;
	int getHeight(Node* nd) const;

private:
	Node* _root;

};
