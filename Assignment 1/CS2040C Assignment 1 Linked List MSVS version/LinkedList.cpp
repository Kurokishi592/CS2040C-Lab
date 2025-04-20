#include "LinkedList.h"

// copy the following code to coursemology without the above line of "#include "LinkedList.h""

// Returns the value at head
int List::head() {
	if (empty()) {
		throw std::out_of_range("List is empty");		//exception for empty list
	}
	return m_head->element;
}

// Checks whether the container/list is empty
bool List::empty() const {
	return m_head == nullptr;
}

// Returns the number of elements
size_t List::size() const {
	return m_size;
}

// Inserts an element/new node to the head
void List::push_head(int element) {
	Node* aNewNode = new Node(element);					//create new node
	aNewNode->next = m_head;							//new node points to node 1
	m_head = aNewNode;									//head points to new node
	m_size++;											//update size
}

// Removes the head element
int List::pop_head() {
	if (empty()) {
		throw std::out_of_range("List is empty");		//exception for empty list
	}
	int value_in_Node = m_head->element;				//need to return the head's value
	Node* temp = m_head;								//to not lose the current head
	m_head = m_head->next;								//to link to next node
	delete temp;										//free the memory
	m_size--;											//update size
	return value_in_Node;
}

// Checks whether the container/linked list contains the specified element
bool List::contains(int element) const {
	Node* currentNode = m_head;							//start from head
	while (currentNode != nullptr) {
		if (currentNode->element == element) {
			return true;
		}
		currentNode = currentNode->next;
	}
	return false;
}

// Returns a std::string equivalent of the container
std::string List::to_string() const {
	if (empty()) {										//check for exception of empty list
		return "{}";
	}

	Node* currentNode = m_head;
	std::string answer = "{";
	
	while (currentNode != nullptr) {
		answer += std::to_string(currentNode->element); //convert from int to string and append
		if (currentNode->next != nullptr) {				//check to ensure it is not the last node
			answer += ", ";								//requires space and comma
		}
		currentNode = currentNode->next;
	}
	answer += "}";
	return answer;
}
