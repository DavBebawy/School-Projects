#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#define NUMCHILDREN 27

using namespace std;

class Trie {
private:
	class TrieNode {
	public:
		TrieNode* children[NUMCHILDREN];

		//TrieNode constructor, creating an array with 27 nullptrs
		TrieNode() {
			for (int i = 0; i < NUMCHILDREN; i++) {
				children[i] = nullptr;
			}
		}
	};
	int wordCount;
	int nodeCount;
	TrieNode* root;	
public:

	//Trie constructor, creating a root node and setting the word and node count to 0
	Trie() {
		wordCount = 0;
		root = new TrieNode();
		nodeCount = 1;
	};

	bool insert(string word);
	int count();
	int getSize();
	bool find(string word);
	Trie::TrieNode* findHelper(string word);
	int completeCount(string word);
	int completeCountHelper(TrieNode* current);
	vector<string> complete(string word);
	void completeHelper(string soFar, TrieNode* current, vector<string>& results);
	void read(string filename);
	void print(std::vector <string> const& a);
	void askForInput();
	~Trie();
	void deleteHelper(TrieNode* subroot);
};