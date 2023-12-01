#include <iostream>
#include <iomanip>
#include <sstream>
#include "authentication.h"
#include "leave_sys.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

int counter = 0;

std::unordered_map<string, std::vector<int>> leaveRecord;
queue<LeaveForm> pending_leaves;

string processDate(){
    int year, month, day;

    std::cout << "Enter date (YYYY MM DD): ";

    std::cin >> year >> month >> day;

    if (year < 2023 || month < 1 || month > 12 || day < 1 || day > 31) {
        std::cerr << "Invalid date entered." << std::endl;
        return "";
    }
    std::stringstream dateStream;
    dateStream << std::setw(4) << std::setfill('0') << year << '-'
               << std::setw(2) << std::setfill('0') << month << '-'
               << std::setw(2) << std::setfill('0') << day;

    std::string dateString = dateStream.str();


    std::cout << "Entered date: " << dateString << std::endl;
    return dateString;
}

Node *build_tree(){

    Node *root = new Node{"Submission", queue<LeaveForm>(), false};
    root->children.push_back(new Node{"HOD", queue<LeaveForm>(), true});
    root->children[0]->children.push_back(new Node{"Rejected by HOD", queue<LeaveForm>(), false});
    root->children[0]->children[0]->children.push_back(new Node{"Rejected and Closed", queue<LeaveForm>(), true});
    root->children[0]->children.push_back(new Node{"Accepted by HOD", queue<LeaveForm>(), false});
    root->children[0]->children[1]->children.push_back(new Node{"DOAA", queue<LeaveForm>(), true});

    root->children[0]->children[1]->children[0]->children.push_back(new Node{"Rejected by DOAA", queue<LeaveForm>(), false});

    root->children[0]->children[1]->children[0]->children.push_back(new Node{"Accepted by DOAA", queue<LeaveForm>(), false});
    root->children[0]->children[1]->children[0]->children[0]->children.push_back(new Node{"Rejected and Closed", queue<LeaveForm>(), false});
    root->children[0]->children[1]->children[0]->children[1]->children.push_back(new Node{"Accepted and Closed", queue<LeaveForm>(), false});



    return root;
}

string process_at_some_stage(Node* currentNode) {
        LeaveForm currentRequest = currentNode->q.front();
        currentNode->q.pop();

        std::cout << "Processing Form ID: " << currentRequest.formID
                  << " at Stage: " << currentRequest.status << std::endl;


        if (currentNode->canReject && rand() % 100 < 20) {
            std::cout << "Leave Request Rejected at Stage: " << currentRequest.status << std::endl;
            currentRequest.status = "Rejected and Closed";
            currentNode->children[0]->q.push(currentRequest);
            return currentRequest.status;
        }


        if (currentRequest.status == "Submitted") {
            currentRequest.status = "Pending Approval from HOD";
            currentNode->children[0]->q.push(currentRequest);
            return currentRequest.status;

        } else if (currentRequest.status == "Pending Approval from HOD") {
            currentRequest.status = "Processing";
            currentNode->children[1]->q.push(currentRequest);
            return currentRequest.status;
        } else if (currentRequest.status == "Processing"){
            currentRequest.status = "Pending Approval from DOAA";
            currentNode->children[0]->q.push(currentRequest);
            return currentRequest.status;
        } else if (currentRequest.status == "Pending Approval from DOAA"){
            currentRequest.status = "Accepted and closed";
            currentNode->children[1]->q.push(currentRequest);
            return currentRequest.status;
        }
        else{
            return "";
        }

}

string processLeaveRequest(Node* currentNode, int formid) {
    int c = 0;

    while (!currentNode->q.empty()){
        LeaveForm currentRequest = currentNode->q.front();
        currentNode->q.pop();

        std::cout << "Processing Form ID: " << currentRequest.formID
                  << " at Stage: " << currentRequest.status << std::endl;


        c++;

        if (currentNode->canReject && rand() % 100 < 20) {
            std::cout << "Leave Request Rejected at Stage: " << currentRequest.status << std::endl;
            currentNode->children[0]->q.push(currentRequest);

        }


        if (currentRequest.status == "Submitted") {
            currentRequest.status = "Pending Approval from HOD";
            currentNode->children[0]->q.push(currentRequest);


        } else if (currentRequest.status == "Pending Approval from HOD") {
            currentRequest.status = "Processing";
            currentNode->children[1]->q.push(currentRequest);

        } else if (currentRequest.status == "Processing"){
            currentRequest.status = "Pending Approval from DOAA";
            currentNode->children[0]->q.push(currentRequest);

        } else if (currentRequest.status == "Pending Approval from DOAA"){
            currentRequest.status = "Accepted and Closed";
            currentNode->children[1]->q.push(currentRequest);

        }
        else{
            continue;
        }

        if (currentRequest.formID == formid) {
            cout << "It took "<< c/5+1 << " days to process your leaveRequest at the current stage\n";
            return currentRequest.status;
        }
    }

    std::cout << "Form does not exist.\n";
    return "";
}


LeaveForm searchFormID(Node *root, int formID){

    queue<LeaveForm> q;
    q = root->q;
    while (!q.empty()){
        if (q.front().formID == formID){
            return q.front();
        }
        else{
            q.pop();
        }
    }

    for (auto& child : root->children) {
        LeaveForm foundForm = searchFormID(child, formID);
        if (foundForm.formID != -1) {
            return foundForm;
        }
    }

    return LeaveForm{-1, "", "", "", "","",  -1};
}

Node *searchFormNode(Node *root, int formID){
    queue<LeaveForm> q = root->q;
    while (!q.empty()){
        if (q.front().formID == formID){
            return root;
        }
        else{
            q.pop();
        }
    }

    for (auto& child : root->children) {
        Node* foundNode = searchFormNode(child, formID);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}

int leave_sub(int leaves, Node *root){
    LeaveForm form;
    form.formID = 1000+counter;
    counter++;
    string type;
    cout <<"Enter leave type: (personal/medical/vacation)\n";
    cin >> type;
    form.leaveType = type;

    int days;
    cout << "For how many days are you planning to take a leave for? \n";
    cin >> days;

    if (days > leaves){
        cout << "Sorry but the number of leaves you have specified is greater than the number of leaves you have remaining.";
        return -1;
    }
    form.num_days = days;


    string startdate;
    startdate = processDate();
    if (startdate == ""){
        return -1;
    }
    form.startdate = startdate;

    string enddate;
    enddate = processDate();
    if (enddate == ""){
        return -1;
    }
    form.enddate = enddate;

    string reason;
    cout << "Give details for your reason take this leave: \n";
    cin >> reason;

    form.reason = reason;

    form.status = "Submitted";

    root->q.push(form);

    return form.formID;
}

LeaveForm generateRandomLeaveForm(){
    LeaveForm form;
    form.formID = 1000 + counter;
    counter++;

    string leaveTypes[] = {"personal", "medical", "vacation"};
    form.leaveType = leaveTypes[rand() % 3];

    form.num_days = rand() % 30 + 1;

    int month = rand() % 12 + 1;
    int day = rand() % 28 + 1;
    form.startdate = string("2023-") + (month < 10 ? "0" : "") + to_string(month) + "-" + (day < 10 ? "0" : "") + to_string(day);

    month = rand() % 12 + 1;
    day = rand() % 28 + 1;
    form.enddate = string("2023-") + (month < 10 ? "0" : "") + to_string(month) + "-" + (day < 10 ? "0" : "") + to_string(day);

    string reasons[] = {"Family vacation", "Medical check-up", "Personal reasons"};
    form.reason = reasons[rand()%3];

    form.status = "Submitted";

    return form;

}

void view_leave_record_in_queue(Node *currentNode, string stage){

    if (currentNode) {
        for (Node* child : currentNode->children){
            view_leave_record_in_queue(child, stage);
        }

        if (currentNode->stage == stage) {
            queue<LeaveForm> q1 = currentNode->q;
            int x = rand()%10+1;
            for (int i = 0; i < x; i++){
                LeaveForm form= generateRandomLeaveForm();
                q1.push(form);
            }
            while (!q1.empty()){
                LeaveForm form = q1.front();
                cout << "Form ID: " << form.formID << "\tLeave Type: " << form.leaveType
                 << "\tStatus: " << form.status << endl;
                cout << "Start Date: " << form.startdate << "\tEnd Date: " << form.enddate << endl;
                cout << "Reason: " << form.reason << "\tNumber of Days: " << form.num_days << endl;
                cout << "---------------------------------------------" << endl;
                cout << endl;
                q1.pop();
            }
        }
    }

}

void approve_application(Node *root, LeaveForm form){
    Node *currentNode = searchFormNode(root, form.formID);

    cout << "Approved Form ID: " << form.formID << endl;
    currentNode->children[0]->q.push(form);
}


void reject_application(Node *root, LeaveForm form){
    Node *currentNode = searchFormNode(root, form.formID);
    cout << "Rejected Form ID: " << form.formID << endl;
    currentNode->children[1]->q.push(form);
}


void view_leave_record_on_top(Node *currentNode, string stage) {
    if (currentNode) {
        if (currentNode->stage == stage) {
            if (!currentNode->q.empty()) {
                queue<LeaveForm> q1 = currentNode->q;
                int x = rand() % 3 + 1;
                for (int i = 0; i < x; i++) {
                    LeaveForm form = generateRandomLeaveForm();
                    q1.push(form);
                }
                LeaveForm form = currentNode->q.front();
                cout << "Form ID: " << form.formID << "\tLeave Type: " << form.leaveType
                     << "\tStatus: " << form.status << endl;
                cout << "Start Date: " << form.startdate << "\tEnd Date: " << form.enddate << endl;
                cout << "Reason: " << form.reason << "\tNumber of Days: " << form.num_days << endl;
                cout << "---------------------------------------------" << endl;
                cout << endl;
                return;
            }
        }

        for (Node *child : currentNode->children) {
            view_leave_record_on_top(child, stage);
        }
    }
}



void generate_data(int n, Node *root){
    std::srand(std::time(0));

    const char* randomNames[] = {
        "Alice", "Bob", "Charlie", "David", "Eva",
        "Frank", "Grace", "Hank", "Ivy", "Jack",
        "Kelly", "Liam", "Mia", "Noah", "Olivia",
        "Peter", "Quinn", "Rachel", "Sam", "Tyler",
        "Ursula", "Victor", "Wendy", "Xander", "Yara",
        "Zane", "Abby", "Ben", "Cara", "Dylan",
        "Elle", "Finn", "Gina", "Hugo", "Isla",
        "Jake", "Kira", "Leo", "Mila", "Nate",
        "Olive", "Pablo", "Quincy", "Riley", "Sara",
        "Tom", "Uma", "Vince", "Will", "Xena"
    };

    std::unordered_map<int, string> BRANCHES = {
    {0, "BSBE"}, {1, "CE"}, {2, "CL"}, {3, "CSE"}, {4, "CST"}, {5, "DSAI"},
    {6, "ECE"}, {7, "EEE"}, {8, "EP"}, {9, "ME"}, {10, "MNC"}
    };

    const int arraySize = sizeof(randomNames) / sizeof(randomNames[0]);
    for (int i = 0; i < n; i++){
        student stud;

        int index = std::rand()%arraySize;
        stud.name = randomNames[index];
        int bindex = std::rand()%10;
        stud.branch = BRANCHES[bindex];

        int roll = std::rand()%100;
        string rollno = "22";
        string branchcode = to_string(bindex);
        string last = to_string(roll);
        rollno.append(branchcode);
        rollno.append("0");
        rollno.append(last);
        stud.rollno = rollno;

        int leaves = std::rand()%30;
        stud.leaves = leaves;
        record.push_back(stud);

        int x = rand() % 3 + 1;
        for (int j = 0; j<x; j++){
            LeaveForm form = generateRandomLeaveForm();
            root->q.push(form);
            leaveRecord[stud.name].push_back(form.formID);
        }

    }


}

void simulate_single_step(Node *root, string name){
    cout << "You have leave requests with the following form id(s) pending\n";
    for (const auto &record :leaveRecord[name]){
        cout << record << " ";
    }
    cout << endl;

    cout << "Enter the form id you want to see simulated: ";
    int formid;
    cin >> formid;

    LeaveForm form = searchFormID(root, formid);
    string status = form.status;
    Node *node = searchFormNode(root, formid);
        if (node == nullptr){
            cout << "Form ID not found.\n";
            return;
        }
    if (status != "Rejected and Closed" && status != "Accepted and Closed"){
        status = processLeaveRequest(node, formid);
        cout << status << endl;
    }

}

void simulate_workflow_for_singleform(Node *node, string name){


    cout << "You have leave requests with the following form id(s) pending\n";
    for (const auto &record :leaveRecord[name]){
        cout << record << " ";
    }
    cout << endl;

    cout << "Enter the form id you want to see simulated: ";
    int formid;
    cin >> formid;

    string status = "";
    while (status != "Rejected and Closed" && status != "Accepted and Closed"){
        status = processLeaveRequest(node, formid);
        cout << status << endl;
         if (status != "Rejected and Closed" && status != "Accepted and Closed") {
            node = searchFormNode(node, formid);
            if (node == nullptr) {
                cout << "Form ID not found." << endl;
                break;
            }
        }
    }
}

void simulate_workflow(Node *node, string name){


    int formid = leaveRecord[name][0];

    string status = "";
    while (status != "Rejected and Closed" && status != "Accepted and Closed"){
        status = processLeaveRequest(node, formid);
        cout << status << endl;
         if (status != "Rejected and Closed" && status != "Accepted and Closed") {
            node = searchFormNode(node, formid);
            if (node == nullptr) {
                cout << "Form ID not found." << endl;
                break;
            }
        }
    }
}


void track_request(string name, Node *root){
    cout << "You have leave requests with the following form id(s) pending\n";
    for (const auto &record :leaveRecord[name]){
        cout << record << " ";
    }
    cout << endl;

    cout << "Enter the form id you want to track: ";
    int formid;
    cin >> formid;

    LeaveForm form = searchFormID(root, formid);
    cout << "STATUS: " << form.status << endl;

}

void view_leave_record(string name, Node *root){
    if (leaveRecord.find(name) != leaveRecord.end()){
        cout << "Leave Record for " << name << ":" << endl;
        for (const auto& formID : leaveRecord[name]) {
            LeaveForm form = searchFormID(root, formID);
            cout << "Form ID: " << form.formID << "\tLeave Type: " << form.leaveType
                 << "\tStatus: " << form.status << endl;
            cout << "Start Date: " << form.startdate << "\tEnd Date: " << form.enddate << endl;
            cout << "Reason: " << form.reason << "\tNumber of Days: " << form.num_days << endl;
            cout << "---------------------------------------------" << endl;
        }
    }
    else {
        cout << "No leave record found for " << name << "."<<endl;
    }

}


