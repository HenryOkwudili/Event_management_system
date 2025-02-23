#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <limits>
#include <fstream>

/*
Chatgpt usage: 
Provided general structure of code
Implemented reusable function that ensures correct input for numeric data types
implemented display syntax
generate report function
*/

using namespace std;

template <typename T>
T validInput(const string& prompt) {
	T value;
//	if(printPrompt){
//		cout<<prompt;
//	}
	
	while (true) {
		cout << prompt;
		cin >> value;
		
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout<<"Invalid Input, try again.\n";
		}else{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
	}
}


class Parcel{
public:
	int id;
	string recipient, address;
	int priority;
	
	Parcel(int id, string recipient, string address, int priority)
		: id(id), recipient(recipient), address(address), priority(priority) {}
};

struct Node {
	Parcel* parcel;
	Node* next;
	
	Node(Parcel* p) : parcel(p), next(nullptr) {}
};

class LinkedList {
public:
	Node* head;
	
	LinkedList() : head(nullptr) {}
	
	void addParcel(Parcel* parcel) {
		Node* newNode = new Node(parcel);
		newNode->next = head;
		head = newNode;
	}
	
	void displayParcels() {
		Node* temp = head;
		while (temp) {
			cout<<"ID: "<<temp->parcel->id
				<<", Recipient: "<<temp->parcel->recipient
				<<", Priority: "<<temp->parcel->priority << endl;
			temp = temp->next;
		}
	}
};

class ComparePriority{
public:
	bool operator()(Parcel* p1, Parcel* p2) {
		return p1->priority > p2->priority;
	}
};

priority_queue<Parcel*, vector<Parcel*>, ComparePriority> priorityQueue;

void addToPriorityQueue(Parcel* parcel) {
	priorityQueue.push(parcel);
}

void displayPriorityQueue(){
	auto tempQueue = priorityQueue;
	while (!tempQueue.empty()) {
		Parcel* p = tempQueue.top();
		cout<<"ID: " << p->id
			<<", Recipient: "<<p->recipient
			<<", priority: "<<p->priority <<endl;
		tempQueue.pop();
	}
}

queue<Parcel*> loadingQueue;
void addToLoadingQueue(Parcel* parcel){
	loadingQueue.push(parcel);
}

void loadParcels() {
	int loadedCount = 0;
	// will limit loadedCount to 10 assuming a truck can't carry more than 10 parcels
	while (!loadingQueue.empty() && loadedCount < 10) {
		Parcel* p = loadingQueue.front();
		cout<<"Loading Parcel ID: "<< p->id <<endl;
		loadingQueue.pop();
		++loadedCount;
	}
	if (loadedCount == 0 ) cout << "No parcels to load.\n";
}

stack<Parcel*> undoStack;
stack<Parcel*> redoStack;

void undoLastAction() {
	if (!undoStack.empty()) {
		Parcel* p = undoStack.top();
		redoStack.push(p);
		undoStack.pop();
		cout<< "Undo last action for parcel of ID: "<< p->id<<endl;
	}else{
		cout<<"No actions to undo.\n";
	}
}

void redoLastAction() {
	if (!redoStack.empty()){
		Parcel* p = redoStack.top();
		undoStack.push(p);
		redoStack.pop();
		cout<< "Redp last action for the parcel of ID: "<< p->id<<endl;
	}else{
		cout<<"No actions to redo.\n";
	}
}

struct TreeNode{
	Parcel* parcel;
	TreeNode* left;
	TreeNode* right;
	
	TreeNode(Parcel* p) : parcel(p), left(nullptr), right(nullptr) {}
};

class Bst{
public:
	TreeNode* root;
	
	Bst() : root(nullptr) {}
	
	void insert(TreeNode*& node, Parcel* parcel) {
		if (!node) {
			node = new TreeNode(parcel);
		} else if (parcel->id < node->parcel->id) {
			insert(node->left, parcel);
		}else{
			insert(node->right, parcel);
		}
	}
	
	void searchByID(TreeNode* node, int id) {
		if (!node) return;
		if (node->parcel->id == id){
			cout<<"Found Parcel ID: "<<id
				<<", Recipient: "<< node->parcel->recipient <<endl;
		}else if (id < node->parcel->id){
			searchByID(node->left, id);
		}else{
			searchByID(node->right, id);
		}
	}
};

//void generateReports(LinkedList& deliveredParcels) {
//	cout<<"Delivered Parcels:" << endl;
//	deliveredParcels.displayParcels();
//	
//	cout<<"\nPending Parcels by Priority:"<<endl;
//	displayPriorityQueue();
//	
//	cout<<"\nDelivery routes used: Route 1"<<endl;
//}


//This uses .csv, Faulty
void generateReports(LinkedList& deliveredParcels){
	ofstream report("ParcelReport.csv");
	if(!report){
		cerr<<"Error: Unable to create file\n";
		return;
	}
	
	report << "ID, Recipient, Priority\n";
	
	Node* temp = deliveredParcels.head;
	while (temp){
		report << temp->parcel->id<<","
		       << temp->parcel->recipient<<","
		       << temp->parcel->priority<<"\n";
		temp = temp->next;
	}
	report.close();
	cout<<"ParcelReport.csv has been generated.\n";
}

Parcel* ParcelInput(){
	int id = validInput<int>("Enter Parcel ID (integer): ");
	
//	cout<<"Enter Parcel ID: ";
//	cin>>id;
//	cin.ignore();
	
	cout<<"Enter recipient's name: ";
	string recipient;
	getline(cin, recipient);
	
	cout<<"Enter address: ";
	string address;
	getline(cin, address);
	
	int priority;
	while(true){
		priority = validInput<int>("Choose priority. 1, 2, or 3, for High, Medium and low respecively: ");
		if (priority>=1 && priority <=3)break;
		cout<< "Please enter a value between 1 and 3.\n";
	}
	
	return new Parcel(id, recipient, address, priority);
}

// Error handling implementation for user input. Faulty
//int getValidatedInput(const string& prompt){
//	int value;
//	while (true) {
//		cout << prompt;
//		cin >> value;
//		if (!cin.fail()) break;
//		cin.clear();
//		cin.ignore(numeric_limits<streamsize>::max(), '\n');
//		cout<<"Invalid input. Please enter a number.\n";
//	}
//	return value;
//}



int main(){
	LinkedList deliveredParcels;
	Bst parcelTree;
	bool running = true;
	
	while (running) {
		cout << "\nParcel Delivery System Menu:\n";
        cout << "1. Register a new parcel\n";
        cout << "2. Add parcel to priority queue\n";
        cout << "3. Add parcel to loading queue\n";
        cout << "4. Load parcels onto delivery truck\n";
        cout << "5. Mark parcel as delivered\n";
        cout << "6. Search for a parcel's ID\n";
        cout << "7. Undo last action\n";
        cout << "8. Redo last action\n";
        cout << "9. Generate reports\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        
        int choice = validInput<int>("Enter your choice: ");
//        cin>>choice;
        
        switch (choice) {
        	case 1: {
        		Parcel* parcel = ParcelInput();
        		parcelTree.insert(parcelTree.root, parcel);
        		undoStack.push(parcel);
        		cout<< "Parcel registered successfully.\n";
				break;
			}
			
			case 2:{
				Parcel* parcel = ParcelInput();
                addToPriorityQueue(parcel);
                cout << "Parcel added to priority queue.\n"; 
				break;
			}
			
			case 3:{
				Parcel* parcel = ParcelInput();
                addToLoadingQueue(parcel);
                cout << "Parcel added to loading queue.\n";
				break;
			}
			
			case 4:{
				loadParcels();
				break;
			}
			
			case 5:{
				Parcel* parcel = ParcelInput();
                deliveredParcels.addParcel(parcel);
                cout << "Parcel marked as delivered.\n";
				break;
			}
			
			case 6:{
				int id = validInput<int>("Enter parcel ID to search: ");
                parcelTree.searchByID(parcelTree.root, id);
				break;
			}
			
			case 7:{
				undoLastAction();
				break;
			}
			
			case 8:{
				redoLastAction();
				break;
			}
			
			case 9:{
				generateReports(deliveredParcels);
				break;
			}
			
			case 0:{
				running = false;
				cout<<"Exiting...\n";
				break;
			}
			default:{
				cout<<"Invalid input. Please enter a number between 0 and 9.\n";
				break;
			}
		}
	}
	
	return 0;
}