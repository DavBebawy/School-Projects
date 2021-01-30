#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <exception>

using namespace std;

class OrgTree
{
private:
	class TreeNode {
	public:
		string title;
		string name;
		TreeNode* leftMostChild;
		TreeNode* rightSibling;
		TreeNode* parent;
		
		//TreeNode constructor
		TreeNode() {
			title = "";
			name = "";
			leftMostChild = nullptr;
			rightSibling = nullptr;
			parent = nullptr;
		}

		//Overloaded TreeNode constructor, with the title and name
		TreeNode(string newTitle, string newName) {
			title = newTitle;
			name = newName;
			leftMostChild = nullptr;
			rightSibling = nullptr;
			parent = nullptr;
		}

		//Overloaded TreeNode constructor, with the title and name and pointer to the parent
		TreeNode(string newTitle, string newName, TreeNode* newParent) {
			title = newTitle;
			name = newName;
			leftMostChild = nullptr;
			rightSibling = nullptr;
			parent = newParent;
		}
	};

public:
	int count;
	TreeNode* root;

	//OrgTree constructor
	//O(1)
	OrgTree();

	//Creates a root note for the tree, if a root exists, the entire tree becomes a subtree of the new root.
	//2 arguments: title and name of the root
	//O(1)
	void addRoot(string title, string name);
	
	//Returns the number of employees in the tree.
	//O(1)
	unsigned int getSize();

	//Returns the title of the employee in the root node of the OrgTree, or an empty string if the OrgTree is empty.
	//O(1)
	string getRoot();

	//Searches for title in the OrgTree, if found, prints the subtree rooted at that node to the stream os.
	//2 arguments: reference to ostream, title for the subtree
	//O(1) for the function, O(n)[recursive] for findHelper & O(n)[recursive] for printHelper 
	void printSubTree(ostream& os, string title);

	//Recursively print the subtree from passed node to the os stream, indenting each level to show tree structure
	//3 arguments: reference to ostream, subroot to be printed, indent required
	//O(n)[recursive]
	void printHelper(ostream& os, TreeNode* subroot, string indent);

	//Finds title in a TreeNode; if found, returns true and places the name associated with the title into name.
	//Otherwise returns false and name is unchanged.
	//2 arguments: title and name reference 
	//O(1) for the function, O(n)[recursive] for findHelper
	bool find(string title, string& name);

	//Recursively finds title in a TreeNode, returns TreeNode pointer if found and nullptr if not in the subtree
	//2 arguments: current as the root of the subtree, title to be found
	//O(n)[recursive], best case O(1)
	TreeNode* findHelper(TreeNode* i, string title);

	//Reads an organization tree from a file saved by write(filename). Deletes old OrgTree object and reads one line at a time
	//lines with ')' charcters mark the end of subtrees. Returns true if read is successful, false if it fails
	//1 argument: filename of saved file
	//O(1) for the function, O(n)[recuresive] for ~OrgTree & O(n)[recursive] for readHelper
	bool read(string filename);

	//Recursively reads each line, linking TreeNodes and creating the OrgTree
	//2 arguments: inputFile reference, newParent to link all children to their parent
	//O(n)[recursive]
	OrgTree::TreeNode* readHelper(ifstream& inputFile, TreeNode* newParent);

	//Writes an organization tree to a file. writes each nodes title and name on a new line
	//lines with ')' charcters mark the end of subtrees.
	//1 argument: filename of save file
	//O(1) for the function, O(n)[recursive] for writeHelper
	void write(string filename);

	//Recursively writes each line, preserving the links with the use of ')' to indicate the end of a subtree
	//2 arguments: current pointer to traverse the tree, outputFile reference
	//O(n)[recursive]
	void writeHelper(TreeNode* current, ofstream& inputFile);

	//Hire an employee. Use findHelper to find the boss with the title, make all the needed links
	//The employee is added to the tree as the last child of the node with title boss.
	//3 arguments: title of the boss, the hired employee's newTitle and newName
	//O(1) for the function, O(n)[recursive] for findHelper
	void hire(string boss, string newTitle, string newName);

	//Fire an employee. Use fireHelper to find the former employee with the title, fix all the needed links
	//Employees working under formerEmployee are added as the last children of the formerEmployee's boss.
	//1 argument: formerTitle, used to find the node of the formerEmplyee
	bool fire(string formerTitle);

	//Overloaded operator = makes and independet copy of an OrgTree when it is passed as a parameter to a function using equalsHelper.
	//1 argument: tree1, original tree to be copied
	//O(1), for killTree & O(n)[recursive] for equalsHelper
	OrgTree& operator=(const OrgTree& tree1);

	//Recursively makes a copy of each node and makes all needed links
	//3 arguments: copy, original and newParent pointers
	//O(n)[recursive]
	void equalsHelper(TreeNode* copy, TreeNode* original, TreeNode* newParent);

	//Deletes subtree and all it's children with killTreeHelper and decreases the count
	//1 argument: subroot
	//O(1) for the function, O(n)[recuresive] for killTreeHelper
	void killTree(TreeNode* subroot);

	//Recursively traverses the rest of the tree and deletes all the nodes in the subtree
	//1 argument: subroot
	//O(n)[recuresive]
	void killTreeHelper(TreeNode* subroot);
};