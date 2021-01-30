#include "Sequence.h"

// CONSTRUCTOR - Creates an empty sequence (numElts == 0) or
//  a sequence of numElts items indexed from 0 ... (numElts - 1).
Sequence::Sequence(size_type sz)
{
	SequenceNode* newNode;		// Intialize new node
	SequenceNode* current;		// A temp SquenceNode tracker
	if (sz > 0) {				// Only intialize if size is bigger than one
		newNode = new SequenceNode();
		head = newNode;
		tail = newNode;
		numElts = 1;
		for (int i = 1; i < sz; i++) {
			current = tail;
			newNode = new SequenceNode();
			current->next = newNode;
			newNode->prev = current;
			numElts++;
			tail = newNode;
		}
	}
}

// CONSTRUCTOR - Creates a (deep) copy of sequence seq. 
Sequence::Sequence(const Sequence& s)
{
	SequenceNode* current;
	SequenceNode* currentNext;
	if (s.head == nullptr) {
		head = tail = nullptr;
	}
	else
	{
		head = new SequenceNode(s.head->elt);
		numElts = 1;
		currentNext = s.head->next;
		current = head;
		while (currentNext != nullptr) {
			current->next = new SequenceNode(currentNext->elt);
			current->next->prev = current;
			numElts++;
			current = current->next;
			currentNext = currentNext->next;
		}
		tail = current;
	}
}

// DESTRUCTOR - Destroys all items in the sequence and
//  release the memory associated with the sequence
Sequence::~Sequence()
{
	SequenceNode* killMe;
	SequenceNode* killMeNext;
	killMe = head;
	while (numElts != 0) {
		killMeNext = killMe->next;
		delete killMe;
		killMe = killMeNext;
		numElts--;
	}
	numElts = 0;
}

// The current sequence is released and replaced by a (deep) copy of sequence (seq).
//		A reference to the copied sequence is returned.
Sequence& Sequence::operator=(const Sequence& s)
{
	this->~Sequence();
	SequenceNode* current;
	SequenceNode* currentNext;
	if (s.head == nullptr) {
		head = tail = nullptr;
	}
	else
	{
		head = new SequenceNode(s.head->elt);
		numElts = 1;
		currentNext = s.head->next;
		current = head;
		while (currentNext != nullptr) {
			current->next = new SequenceNode(currentNext->elt);
			current->next->prev = current;
			numElts++;
			current = current->next;
			currentNext = currentNext->next;
		}
		tail = current;
	}
	return (*this);
}

// Returns a reference to the item at index position in the sequence.
//  The position must satisfy ( position >= 0 && position <= last_index( ) ).
Sequence::value_type& Sequence::operator[](size_type position)
{
	SequenceNode* current;
	current = head;
	if (position < 0 || position >= numElts)
	{
		throw exception("Error: Invalid position");
	}
	for (int i = 0; i < position; i++) {
		current = current->next;
	}
	return current->elt;
}

// The value of item is append to the sequence.
void Sequence::push_back(const value_type& value)
{
	SequenceNode* newNode;
	SequenceNode* current;
	if (!empty()) {
		current = tail;
		newNode = new SequenceNode(value);
		current->next = newNode;
		newNode->prev = current;
		tail = newNode;
		numElts++;
	}
	else
	{
		newNode = new SequenceNode(value);
		head = newNode;
		tail = newNode;
		numElts = 1;
	}
}

// The item at the end of the sequence is deleted and size of the sequence is reduced by one.
//  The size of the sequence must be greater than zero.
void Sequence::pop_back()
{
	if (empty() || tail->prev == nullptr)
		throw exception("Error: List is empty");
	else {
		SequenceNode* current;
		SequenceNode* currentNext;

		current = tail->prev;
		current->next = nullptr;
		currentNext = tail;
		delete currentNext;
		tail = current;
		numElts--;
	}
}

// The value is inserted at position and the size of sequence is increased by one.
//  The position must satisfy ( position >= 0 && position <= last_index( ) ).
void Sequence::insert(size_type position, value_type value)
{
	SequenceNode* newNode;
	SequenceNode* current;
	SequenceNode* currentNext;

	if (position == 0) {
		current = head;
		newNode = new SequenceNode(value);
		newNode->next = current;
		current->prev = newNode;
		head = newNode;
		numElts++;
	}
	else if (position < 0 || position >= numElts)
	{
		throw exception("Error: Invalid position");
	}
	else {
		current = head;
		newNode = new SequenceNode(value);
		for (int i = 1; i < position; i++) {
			current = current->next;
		}
		currentNext = current->next;
		current->next = newNode;
		newNode->prev = current;
		newNode->next = currentNext;
		currentNext->prev = newNode;
		numElts++;
	}
}

// Returns a reference to the first item in the sequence.
//  The size of the sequence must be greater than zero.
const Sequence::value_type& Sequence::front() const
{
	if (!empty())
		return head->elt;
	else
		throw exception("Error: List is empty");
}

// Returns a reference to the last item in the sequence.
//  The size of the sequence must be greater than zero.
const Sequence::value_type& Sequence::back() const
{
	if (!empty())
		return tail->elt;
	else
		throw exception("Error: List is empty");
}

// A true return value indicates size of the sequence is zero.
bool Sequence::empty() const
{
	if (numElts == 0 || head == nullptr) {
		return true;
	}
	else
		return false;
}

// Returns the number of items in the sequence.
Sequence::size_type Sequence::size() const
{
	return numElts;
}

// All items in the sequence deleted and the memory associated with the sequence is released.
void Sequence::clear()
{
	SequenceNode* killMe;
	SequenceNode* killMeNext;
	killMe = head;
	while (killMe != nullptr) {
		killMeNext = killMe->next;
		delete killMe;
		killMe = killMeNext;
	}
	numElts = NULL;
}

// The positions / items in the sequence at ( position ... (position + count - 1) ) are deleted.
//  The position must satisfy ( position >= 0 && position + count - 1 <= last_index( ) ).
void Sequence::erase(size_type position, size_type count)
{
	SequenceNode* current;
	SequenceNode* current1;
	SequenceNode* previous;

	if (position < 0 || position + count > numElts) {
		throw exception("Error: List is empty");
	}
	else if (position == 0) {
		current = head;
		for (int i = 0; i < count; i++) {
			current1 = current->next;
			delete current;
			current = current1;
			numElts--;
		}
		head = current;
		head->prev = nullptr;
	}
	else if (position + count == numElts) {
		current = head;
		for (int i = 0; i < position; i++) {
			current = current->next;
		}
		tail = current->prev;
		for (int i = 0; i < count; i++) {
			current1 = current->next;
			delete current;
			current = current1;
			numElts--;
		}
		tail->next = nullptr;
	}
	else {
		current = head;
		for (int i = 0; i < position; i++) {
			current = current->next;
		}
		previous = current->prev;
		for (int i = 0; i < count; i++) {
			current1 = current->next;
			delete current;
			current = current1;
			numElts--;
		}
		previous->next = current;
		current->prev = previous;
	}
}

// Prints the elements of the Sequence to outputStream
void Sequence::print(ostream& outputStream) const {
	SequenceNode* current;
	if (!empty() ) {
		current = head;
		while (current) {
			outputStream << current->elt;
			if (current->next) {
				outputStream << ", ";
			}
			current = current->next;
		}
		outputStream << endl;
	}
	else
		throw exception("Error: List is empty");
}

//overloaded << operator used to print to the ostream
ostream& operator<<(ostream& os, Sequence& s)
{
	s.print(os);
	return os;
}