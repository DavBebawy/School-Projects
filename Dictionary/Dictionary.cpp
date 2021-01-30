#include "Dictionary.h"

//Inserts a new word into the trie. Checks if a duplicate word exists, returns false
//Return true when a word is successfully inserted
//O(n)
bool Trie::insert(string word) {
	TrieNode* currentNode = root;
	int index;
	int wordSize = word.length();
	for (int i = 0; i < wordSize; i++) { //Splits the words into characters
		if (65 <= word[i] >= 90)
			word[i] += 32;
		index = word[i] - 'a';
		if (currentNode->children[index] == nullptr) { //If a node does not exists, creates a new one
			currentNode->children[index] = new TrieNode();
			nodeCount++;
		}
		currentNode = currentNode->children[index]; //If nodes exists, go to that node
	}
	if (currentNode->children[26] != nullptr) //Checks if words already exists, returns false
		return false;
	currentNode->children[26] = new TrieNode(); //Creates node at [26] to indicate new word, increments word and node counters and returns true
	nodeCount++;
	wordCount++;
	return true;
}

//Returns the number of words in the trie, incremented as words are inserted
//O(1)
int Trie::count() {
	return wordCount;
}

//Return the total number of nodes in the trie, incremented as nodes are created
//O(1)
int Trie::getSize() {
	return nodeCount;
}

//Finds a word in the trie. If the word is found, returns true
//Otherwise, it returns false
//O(1) for the function, O(n)[recursive] for findHelper
bool Trie::find(string word) {
	TrieNode* currentNode = findHelper(word);
	if (currentNode == nullptr || currentNode->children[26] == nullptr)
		return false;
	return true;
}

//Recursively traverses the trie to find word. If it is not found, it returns nullptr
//If found, it returns a reference to the TrieNode
//O(n)[recursive]
Trie::TrieNode* Trie::findHelper(string word) {
	TrieNode* currentNode = root;
	int index;
	int wordSize = word.length();
	for (int i = 0; i < wordSize; i++) {
		if (65 <= word[i] >= 90)
			word[i] += 32;
		index = word[i] - 'a';
		if (currentNode->children[index] == nullptr)
			return nullptr;

		currentNode = currentNode->children[index];
	}
	return currentNode;
}

//Returns the number of words in the dictionary that begin with the given input string.
//If no matching words are found, it returns zero.
//O(n) for the function, O(n)[recursive] for findHelper and completeCountHelper
int Trie::completeCount(string word) {
	int count = 0;
	TrieNode* currentNode = findHelper(word);
	if (currentNode == nullptr)
		return 0;
	for (int i = 0; i < NUMCHILDREN; i++) {
		if (currentNode->children[i] != nullptr)
			count += completeCountHelper(currentNode->children[i]);
	}
	return count;
}

//Recursively traverses the trie to count complete words. Only returns 1 if a leaf node is found
//O(n)[recursive]
int Trie::completeCountHelper(TrieNode* current) {
	bool leaf = true;
	int count = 0;
	for (int i = 0; i < NUMCHILDREN; i++) {
		if (current->children[i] != nullptr) {
			leaf = false;
			count += completeCountHelper(current->children[i]);
		}
	}
	if (leaf)
		return 1;
	else
		return count;
}

//Returns a vector of string with all the words in the dictionary that begin with the given input string.
//O(1) for the function, O(n)[recursive] for findHelper and completeHelper
vector<string> Trie::complete(string word) {
	vector<string> allWords;
	TrieNode* currentNode = findHelper(word);
	if (currentNode == nullptr)
		return allWords;
	completeHelper(word, currentNode, allWords);
	return allWords;
}

//Recursively traverses the trie to add words to the vector.
//O(n)[recursive]
void Trie::completeHelper(string soFar, TrieNode* currentNode, vector<string>& results) {
	for (int i = 0; i < NUMCHILDREN - 1; i++) {
		if (currentNode->children[i] != nullptr) {
			char x = 'a' + i;
			completeHelper(soFar + x, currentNode->children[i], results);
		}
	}
	if (currentNode->children[26] != nullptr)
		results.push_back(soFar);
}

//Reads an organization tree from a file saved by write(filename)
//Deletes old OrgTree object and reads one line at a time
//lines with ')' characters mark the end of subtrees.
//Returns true if read is successful, false if it fails
//O(n) for the function, O(n) for insert
void Trie::read(string filename) {
	ifstream inputFile;
	inputFile.open(filename);
	this->~Trie();

	wordCount = 0;
	root = new TrieNode();
	nodeCount = 1;

	string nextWord;

	while (inputFile.eof() == false) {
		getline(inputFile, nextWord);
		this->insert(nextWord);
	}
	inputFile.close();
}

//Prints words in vector
void Trie::print(vector <string> const& a) {
	int x = a.size();
	for (int i = 0; i < x; i++)
		cout << a.at(i) << endl;
}

//Asks the user for input
void Trie::askForInput() {
	bool search = true;
	string searchWord;
	while (search) {
		cout << "Please enter a word prefix (or press enter to exit): ";
		getline(cin, searchWord);
		if (searchWord.empty())
			search = false;
		else {
			int count;
			count = completeCount(searchWord);
			cout << "There are " << count << " completions for the prefix '" << searchWord << "'. ";
			if (count >= 1) {
				string showCompletions;
				cout << "Show completions? ";
				getline(cin, showCompletions);
				while (!(showCompletions == "No" || showCompletions == "N" || showCompletions == "no" || showCompletions == "n")) {
					if (showCompletions == "Yes" || showCompletions == "Y" || showCompletions == "yes" || showCompletions == "y") {
						cout << "Completions" << endl;
						cout << "-----------" << endl;
						print(complete(searchWord));
						break;
					}
					else {
						cout << "Please enter Y or N. Show completions? ";
						getline(cin, showCompletions);
					}
				}
			}
			else
				cout << endl;
			cout << endl;
		}
	}
}

//DESTRUCTOR uses deleteHelper to delete the trie 
Trie::~Trie() {
	for (int i = 0; i < NUMCHILDREN; i++) {
		deleteHelper(root->children[i]);
	}
	deleteHelper(root);
}

//Helper function recursively deletes each node
void Trie::deleteHelper(TrieNode* subroot) {
	if (subroot != nullptr) {
		for (int i = 0; i < NUMCHILDREN; i++) {
			deleteHelper(subroot->children[i]);
		}
		nodeCount--;
		delete subroot;
	}
}

//main function, creates a trie, reads the file, ask the user for input
int main() {
	Trie trie1;
	trie1.read("wordlist.txt");
	trie1.askForInput();
	trie1.~Trie();
}