#ifndef LEAVE_SYS_H
#define LEAVE_SYS_H
#include <string>
#include <queue>
#include <vector>
#include "authentication.h"

using namespace std;



struct LeaveForm {
    int formID;
    string leaveType;
    string startdate;
    string enddate;
    string status;
    string reason;
    int num_days;
};

struct Node{
    string stage;
    queue<LeaveForm> q;
    bool canReject;
    std::vector<Node*> children;
};


extern std::unordered_map<string, std::vector<int>> leaveRecord;

extern queue<LeaveForm> pending_leaves;

int leave_sub(int leaves, Node* root);
Node *build_tree();
LeaveForm generateRandomLeaveForm();
string processDate();
string process_at_some_stage(Node* currentNode);
void simulate_workflow_for_singleform(Node *node, string name);
void simulate_single_step(Node *root, string name);
LeaveForm searchFormID(Node *root, int formID);
void track_request(string name, Node *root);
void view_leave_record(string name, Node *root);
void generate_data(int n, Node *root);
string processLeaveRequest(Node* currentNode, int formid);
void view_leave_record_in_queue(Node *currentNode, string stage);
void view_leave_record_on_top(Node *currentNode, string stage);
void approve_application(Node *root, LeaveForm form);
void reject_application(Node *root, LeaveForm form);
void simulate_workflow(Node *node, string name);




#endif
