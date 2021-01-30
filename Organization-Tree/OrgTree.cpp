#include "OrgTree.h"

//OrgTree constructor
//O(1)
OrgTree::OrgTree()	{
	count = 0;
	root = nullptr;
}

//Creates a root note for the tree, if a root exists, the entire tree becomes a subtree of the new root.
//2 arguments: title and name of the root
//O(1)
void OrgTree::addRoot(string title, string name)	{
	if (root != nullptr) {
		TreeNode* temp;
		temp = root;
		root = new OrgTree::TreeNode(title, name);
		root->leftMostChild = temp;
		temp->parent = root;
		count++;
	}

	else
	{
		root = new OrgTree::TreeNode(title, name);
		count++;
	}
}

//Returns the number of employees in the tree.
//O(1)
unsigned int OrgTree::getSize()	{
	return count;
}

//Returns the title of the employee in the root node of the OrgTree, or an empty string if the OrgTree is empty.
//O(1)
string OrgTree::getRoot()	{
	if (count != 0)
		return root->title;
	else
		return "";
}

//Searches for title in the OrgTree, if found, prints the subtree rooted at that node to the stream os.
//2 arguments: reference to ostream, title for the subtree
//O(1) for the function, O(n)[recursive] for findHelper & O(n)[recursive] for printHelper 
void OrgTree::printSubTree(ostream& os, string title)	{
	TreeNode* result;
	result = findHelper(root, title);
	if (result == nullptr)
		return;
	printHelper(os, result, "");
}

//Recursively print the subtree from passed node to the os stream, indenting each level to show tree structure
//3 arguments: reference to ostream, subroot to be printed, indent required
//O(n)[recursive]
void OrgTree::printHelper(ostream& os, TreeNode* subroot, string indent)	{
	os << indent << subroot->title << ":" << subroot->name << endl;

	TreeNode* temp = subroot->leftMostChild;
	while (temp != nullptr) {
		printHelper(os, temp, indent + "        ");
		temp = temp->rightSibling;
	}
}

//Finds title in a TreeNode; if found, returns true and places the name associated with the title into name.
//Otherwise returns false and name is unchanged.
//2 arguments: title and name reference 
//O(1) for the function, O(n)[recursive] for findHelper
bool OrgTree::find(string title, string& name)	{
	TreeNode* result;
	result = findHelper(root, title);
	if (result == nullptr)
		return false;
	name = result->name;
	return true;
}

//Recursively finds title in a TreeNode, returns TreeNode pointer if found and nullptr if not in the subtree
//2 arguments: current as the root of the subtree, title to be found
//O(n)[recursive], best case O(1)
OrgTree::TreeNode* OrgTree::findHelper(TreeNode* current, string title)	{
	TreeNode* result = nullptr;
	if (count == 0 || root == nullptr)
		return result;
	if (current->title == title) {
		return current;
	}
	if (result == nullptr && current->leftMostChild != nullptr) {
		result = findHelper(current->leftMostChild, title);
	}
	if (result == nullptr && current->rightSibling != nullptr) {
		result = findHelper(current->rightSibling, title);
	}
	return result;
}

//Reads an organization tree from a file saved by write(filename). Deletes old OrgTree object and reads one line at a time
//lines with ')' charcters mark the end of subtrees. Returns true if read is successful, false if it fails
//1 argument: filename of saved file
//O(1) for the function, O(n)[recuresive] for ~OrgTree & O(n)[recursive] for readHelper
bool OrgTree::read(string filename)	{
	ifstream inputFile;
	inputFile.open(filename);

	this->~OrgTree();
	string newTitle, newName, junk;

	getline(inputFile, newTitle, ',');
	getline(inputFile, newName);
	root = new TreeNode(newTitle, newName);
	count = 1;
	if (inputFile.peek() != ')')
	{
		root->leftMostChild = readHelper(inputFile, root);
		inputFile.close();
		return true;
	}
	inputFile.close();
	return false;
}

//Recursively reads each line, linking TreeNodes and creating the OrgTree
//2 arguments: inputFile reference, newParent to link all children to their parent
//O(n)[recursive]
OrgTree::TreeNode* OrgTree::readHelper(ifstream& inputFile, TreeNode* newParent)	{
 	string newTitle, newName, junk;
	if (inputFile.peek() == ')')
		return nullptr;
	getline(inputFile, newTitle, ',');
	getline(inputFile, newName);
	TreeNode* subroot;
	subroot = new TreeNode(newTitle, newName, newParent);
	count++;
	if (inputFile.peek() != ')')
		subroot->leftMostChild = readHelper(inputFile, subroot);
	getline(inputFile, junk); // Read the ‘)’ charreturn subroot;
	if (inputFile.peek() != ')')
		subroot->rightSibling = readHelper(inputFile, newParent);
	return subroot;
}

//Writes an organization tree to a file. writes each nodes title and name on a new line
//lines with ')' charcters mark the end of subtrees.
//1 argument: filename of save file
//O(1) for the function, O(n)[recursive] for writeHelper
void OrgTree::write(string filename)	{
	ofstream outputFile;
	outputFile.open(filename);
	outputFile.clear();
	writeHelper(root, outputFile);
	outputFile.close();
}

//Recursively writes each line, preserving the links with the use of ')' to indicate the end of a subtree
//2 arguments: current pointer to traverse the tree, outputFile reference
//O(n)[recursive]
void OrgTree::writeHelper(TreeNode* current, ofstream& outputFile)	{
	outputFile << current->title << ',' << " " << current->name << endl;

	TreeNode* temp = current->leftMostChild;
	while (temp != nullptr)	{
		writeHelper(temp, outputFile);
		temp = temp->rightSibling;
	}
	outputFile << ')' << endl;
}

//Hire an employee. Use findHelper to find the boss with the title, make all the needed links
//The employee is added to the tree as the last child of the node with title boss.
//3 arguments: title of the boss, the hired employee's newTitle and newName
//O(1) for the function, O(n)[recursive] for findHelper
void OrgTree::hire(string boss, string newTitle, string newName)	{
	TreeNode* theBoss;
	theBoss = findHelper(root, boss);
	if (theBoss->title != boss)
		return;
	TreeNode* current = theBoss->leftMostChild;
	if (current != nullptr) {
		while (current->rightSibling != nullptr)
			current = current->rightSibling;
		current->rightSibling = new OrgTree::TreeNode(newTitle, newName, theBoss);
		count++;
	}
	else {
		theBoss->leftMostChild = new OrgTree::TreeNode(newTitle, newName, theBoss);
		count++;
	}
}

//Fire an employee. Use fireHelper to find the former employee with the title, fix all the needed links
//Employees working under formerEmployee are added as the last children of the formerEmployee's boss.
//1 argument: formerTitle, used to find the node of the formerEmplyee
bool OrgTree::fire(string formerTitle)	{
	TreeNode* formerEmployee, *current;
	formerEmployee = findHelper(root, formerTitle);
	if (formerEmployee == root || formerEmployee == nullptr)
		return false;
	current = formerEmployee->parent->leftMostChild;
	if (current == formerEmployee) {
		if (formerEmployee->rightSibling == nullptr) {
			if (formerEmployee->leftMostChild != nullptr)
				formerEmployee->parent->leftMostChild = formerEmployee->leftMostChild;
			else
				formerEmployee->parent->leftMostChild = nullptr;
		}
		else {
			formerEmployee->parent->leftMostChild = formerEmployee->rightSibling;
			if (formerEmployee->leftMostChild != nullptr) {
				while (current->rightSibling != nullptr)
					current = current->rightSibling;
				current->rightSibling = formerEmployee->leftMostChild;
			}
		}
	}
	else if (formerEmployee->rightSibling != nullptr) {
		while (current->rightSibling != formerEmployee)
			current = current->rightSibling;
		current->rightSibling = formerEmployee->rightSibling;
		if (formerEmployee->leftMostChild != nullptr) {
			while (current->rightSibling != nullptr)
				current = current->rightSibling;
			current->rightSibling = formerEmployee->leftMostChild;
		}
	}
	else {
		while (current->rightSibling != formerEmployee)
			current = current->rightSibling;
		if (formerEmployee->leftMostChild != nullptr) {
			current->rightSibling = formerEmployee->leftMostChild;
		}
		else
			current->rightSibling = nullptr;
	}

	if (formerEmployee->leftMostChild != nullptr) {
		current = formerEmployee->leftMostChild;
		while (current->rightSibling != nullptr) {
			current->parent = formerEmployee->parent;
			current = current->rightSibling;
		}
	}
	delete formerEmployee;
	count--;
	return true;
}

//Overloaded operator = makes and independet copy of an OrgTree when it is passed as a parameter to a function using equalsHelper.
//1 argument: tree1, original tree to be copied
//O(1), for killTree & O(n)[recursive] for equalsHelper
OrgTree& OrgTree::operator=(const OrgTree& tree1)	{
	this->killTree(root);
	TreeNode* original;
	original = tree1.root;
	root = new TreeNode(original->title, original->name);
	count = 1;
	equalsHelper(root, original, root);
	return (*this);
}

//Recursively makes a copy of each node and makes all needed links
//3 arguments: copy, original and newParent pointers
//O(n)[recursive]
void OrgTree::equalsHelper(TreeNode* copy, TreeNode* original, TreeNode* newParent)	{
	if (original->leftMostChild != nullptr) {
		copy->leftMostChild = new TreeNode(original->leftMostChild->title, original->leftMostChild->name, newParent);
		count++;
		equalsHelper(copy->leftMostChild, original->leftMostChild, copy->leftMostChild);
	}
	if (original->rightSibling != nullptr) {
		copy->rightSibling = new TreeNode(original->rightSibling->title, original->rightSibling->name, newParent);
		count++;
		equalsHelper(copy->rightSibling, original->rightSibling, newParent);
	}
}

//Deletes subtree and all it's children with killTreeHelper and decreases the count
//1 argument: subroot
//O(1) for the function, O(n)[recuresive] for killTreeHelper
void OrgTree::killTree(TreeNode* subroot) {
	if (subroot != nullptr) {
		killTreeHelper(subroot->leftMostChild);
		delete subroot;
		count--;
	}
}

//Recursively traverses the rest of the tree and deletes all the nodes in the subtree
//1 argument: subroot
//O(n)[recuresive]
void OrgTree::killTreeHelper(TreeNode* subroot) {
	if (subroot != nullptr) {
		killTreeHelper(subroot->leftMostChild);
		killTreeHelper(subroot->rightSibling);
		delete subroot;
		count--;
	}
}