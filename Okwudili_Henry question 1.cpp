#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <limits>


/*
Chatgpt usage: gave general structure of code
implemented error handling for user input
redo and undo functions
fixed errors with delete event function
implemented numerical limits
implemented undo and redo features
implemented syntax for display functions
*/

using namespace std;

struct Participant{
	int id;
	string name;
	Participant* next;
	
	Participant(int id, string name): id(id), name(name), next(nullptr) {}
	
};

struct Event{
	int id;
	string name;
	string category;
	Participant* participants;
	
	Event(int id, string name, string category)
		:id(id), name(name), category(category), participants(nullptr) {}
		
	void addParticipant(int pid, string pname) {
		Participant* newParticipant = new Participant(pid, name);
		if (!participants) {
            participants = newParticipant;
        } else {
            Participant* temp = participants;
            while (temp->next) temp = temp->next;
            temp->next = newParticipant;
        }
	}
	
	void display() {
		cout<<"Event ID: "<< id << ", Name: "<< name
			<<", Category: "<< category << endl;
		cout << "Participants: ";
		Participant* temp = participants;
		if(!temp) {
			cout<<"No participant yet.";
		}
		while(temp){
			cout << temp->name << " (ID: "<< temp->id << "), ";
			temp = temp->next;
		}
		cout<<endl;
	}
};

int getIntInput(const string& prompt) {
	int value;
	while(true){
		cout<< prompt;
		cin>> value;
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout<< "Invalid input. Please enter a valid number."<<endl;
		}else{
			break;
		}
	}
	return value;
}

string getStringInput(const string& prompt) {
	string value;
	cout<<prompt;
	cin.ignore();
	getline(cin, value);
	return value;
}

struct TreeNode{
	Event* event;
	TreeNode* left;
	TreeNode* right;
	
	TreeNode(Event* event) : event(event), left(nullptr), right(nullptr) {}
};

struct Operation {
    string type;
    Event* event;
    Participant* participant;
    string oldName;
    string oldCategory;
};

//stack<Operation> undoStack;
//stack<Operation> redoStack;
//priority_queue<pair<int, Event*>> eventSchedule;
//queue<Participant*> checkInQ;

TreeNode* insertEvent(TreeNode* root, Event* event) {
	 if (!root) {
        return new TreeNode(event);
    }
    if (event->id < root->event->id) {
        root->left = insertEvent(root->left, event);
    } else {
        root->right = insertEvent(root->right, event);
    }
    return root;
}

TreeNode* searchEventNode(TreeNode* root, int id) {
	if (!root || root->event->id == id){
		return root;
	}
	return id < root->event->id ? searchEventNode(root->left, id) : searchEventNode(root->right, id);
}	

TreeNode* findMinNode(TreeNode* root) {
    while (root && root->left) root = root->left;
    return root;
}

TreeNode* deleteEvent(TreeNode* root, int id) {
	if (!root) return nullptr;

    if (id < root->event->id) {
        root->left = deleteEvent(root->left, id);
    } else if (id > root->event->id) {
        root->right = deleteEvent(root->right, id);
    } else {
        // Node to be deleted found
        if (!root->left) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }

       TreeNode* temp = findMinNode(root->right);
       root->event = temp->event;
       root->right = deleteEvent(root->right, temp->event->id);
    }
    return root;
}

void updateEvent(Event* event){
if (!event) return;
    cout << "Updating Event: " << event->id << " - " << event->name << endl;
    string newName = getStringInput("Enter new event name (or press Enter to keep current): ");
    string newCategory = getStringInput("Enter new event category (or press Enter to keep current): ");

    if (!newName.empty()) event->name = newName;
    if (!newCategory.empty()) event->category = newCategory;

    cout << "Event updated successfully!" << endl;
}

//Event* searchEvent(TreeNode* root, int id){
//	if (!root || root->event->id == id){
//		return root ? root->event : nullptr;
//	}
//	return id < root->event->id ? searchEvent(root->left, id) : searchEvent(root->right, id);
//}



void displayEvents(TreeNode* root) {
	if(!root) return;
	displayEvents(root->left);
	root->event->display();
	displayEvents(root->right);
}

queue<Participant*> checkInQ;


void enqParticipant(Participant* participant) {
	checkInQ.push(participant);
	 cout << "Participant " << participant->name << " added to the check-in queue." << endl;
}

void processCheckIn(){
	if (checkInQ.empty()) {
        cout << "No participants in queue." << endl;
        return;
    }
    Participant* next = checkInQ.front();
    checkInQ.pop();
    cout << "Processing check-in for: " << next->name << " (ID: " << next->id << ")" << endl;
}

void viewNextCheckIn(){
	if (checkInQ.empty()) {
        cout << "No participants in queue." << endl;
    } else {
        Participant* next = checkInQ.front();
        cout << "Next in line: " << next->name << " (ID: " << next->id << ")" << endl;
    }
}

//struct EventComparator{
//	bool operator()(Event* a, Event* b){
//		return a->id > b->id;
//	}
//};
//
//priority_queue<Event*, vector<Event*>, EventComparator> eventSchedule;
//
//void scheduleEvent(Event* event){
//	eventSchedule.push(event);
//	cout<<"Event "<< event->name << "scheduled successfully!"<<endl;
//}

stack<Event*> undoStack;
stack<Event*> redoStack;

void undo(){
	if (undoStack.empty()) {
        cout << "Nothing to undo." << endl;
        return;
    }
    Event* lastAction = undoStack.top();
    undoStack.pop();
    redoStack.push(lastAction);
    cout << "Undo performed for event: " << lastAction->name << endl;
}

void redo() {
	 if (redoStack.empty()) {
        cout << "Nothing to redo." << endl;
        return;
    }
    Event* lastRedo = redoStack.top();
    redoStack.pop();
    undoStack.push(lastRedo);
    cout << "Redo performed for event: " << lastRedo->name << endl;
}

struct EventComparator{
	bool operator()(Event* a, Event* b){
		return a->id > b->id;
	}
};

priority_queue<Event*, vector<Event*>, EventComparator> eventSchedule;

void scheduleEvent(Event* event) {
    eventSchedule.push(event);
    cout << "Event " << event->name << " scheduled successfully!" << endl;
}

void displaySchedule(){
	priority_queue<Event*, vector<Event*>, EventComparator> tempQueue = eventSchedule;
    cout << "Scheduled Events (by priority):" << endl;
    while (!tempQueue.empty()) {
        Event* event = tempQueue.top();
        tempQueue.pop();
        cout << "Event ID: " << event->id << ", Name: " << event->name << endl;
    }
}


//void generateReports(TreeNode* root) {
//    cout << "\n--- Event Reports ---\n";
//    cout << "\nList of Events and Participants:\n";
//    displayEvents(root);
//
//    cout << "\nEvent Schedule:\n";
//    displaySchedule();
//
//    cout << "\nCheck-In Statistics:\n";
//    cout << "Total Participants in Queue: " << checkInQ.size() << endl;
//}

//void generateReports() {
//        cout << "\nGenerating Reports...\n" << endl;
//        displayEvents();
//        displaySchedule();
//        checkInQ.size();
//    }


int main(){
	TreeNode* eventTree = nullptr;
	
	while(true){
		cout << "\nMenu:\n";
        cout << "1. Create Event\n";
        cout << "2. Add Participant\n";
        cout << "3. View All Events\n";
        cout << "4. Update Event\n";
        cout << "5. Delete Event\n";
        cout << "6. Schedule Event\n";
        cout << "7. View Schedule\n";
        cout << "8. Enqueue Check-In\n";
        cout << "9. Process Check-In\n";
        cout << "10. View Next Check-In\n";
        cout << "11. undo action\n";
        cout << "12. redo action\n";
		cout << "13. generate report\n"; 
        cout << "14. Exit\n";
        int choice = getIntInput("Enter your choice: ");
        
        switch (choice){
        	case 1: {
        		int id = getIntInput("Enter Event ID: ");
                string name = getStringInput("Enter Event Name: ");
                string category = getStringInput("Enter Event Category: ");
                Event* newEvent = new Event(id, name, category);
                eventTree = insertEvent(eventTree, newEvent);
                cout << "Event created successfully" << endl;
				break;
			}
			case 2: {
				 int eventId = getIntInput("Enter Event ID: ");
                TreeNode* eventNode = searchEventNode(eventTree, eventId);
                if (!eventNode) {
                    cout << "Event not found!" << endl;
                } else {
                    int pid = getIntInput("Enter Participant ID: ");
                    string pname = getStringInput("Enter Participant Name: ");
                    eventNode->event->addParticipant(pid, pname);
                    cout << "Participant added successfully!" << endl;
                }
				break;
			}
			case 3: {
				displayEvents(eventTree);
				break;
			}
			case 4: {
				int eventId = getIntInput("Enter Event ID to update: ");
                TreeNode* eventNode = searchEventNode(eventTree, eventId);
                if (!eventNode) {
                    cout << "Event not found!" << endl;
                } else {
                    updateEvent(eventNode->event);
                }
				break;
			}
			case 5:{
				int eventId = getIntInput("Enter Event ID to delete: ");
                eventTree = deleteEvent(eventTree, eventId);
                cout << "Event deleted successfully!" << endl;
				break;
			}
			case 6: {
				int eventId = getIntInput("Enter Event ID to schedule: ");
                TreeNode* eventNode = searchEventNode(eventTree, eventId);
                if (!eventNode) {
                    cout << "Event not found!" << endl;
                } else {
                    scheduleEvent(eventNode->event);
                }
				break;
			}
			case 7: {
				displaySchedule();
				break;
			}
			case 8: {
				int pid = getIntInput("Enter Participant ID: ");
                string pname = getStringInput("Enter Participant Name: ");
                enqParticipant(new Participant(pid, pname));
				break;
			}
			case 9: {
				processCheckIn();
				break;
			}
			 case 10: {
                viewNextCheckIn();
                break;
            }
            case 11:{
            	undo();
				break;
			}
			case 12:{
				redo();
				break;
			}
			case 13: {
//				generateReports();
				cout<<"Cannot at this time";
				break;
			}
            case 14: {
            	cout<<"Exiting..."<<endl;
				return 0;
			}
	default: {
		cout<<"Invalid choice. Please try again."<<endl;
		break;
	}
		}
	}
}