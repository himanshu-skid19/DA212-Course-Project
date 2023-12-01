#include <iostream>
#include "authentication.h"
#include "user_dashboard.h"
#include "leave_sys.h"

using namespace std;

void user_dashboard(student stud){
    string name = stud.name;
    cout << "Weclome " << name << endl;
    Node *root = build_tree();
    generate_data(10, root);
    int formid;
    string stage;
    string role = stud.role;
    int r;
    LeaveForm form;
    if (role == "STUDENT"){
        int input = 0;
        while (input != 5){
            cout << "Enter 1 to view your profile details\n";
            cout << "Enter 2 to submit a leave application\n";
            cout << "Enter 3 to see your pending leave Requests\n";
            cout << "Enter 4 to track a Leave Request\n";
            cout << "Enter 5 to log out\n";

            cin >> input;

            switch (input){
                case 1:
                    view_student_details(stud);
                    break;
                case 2:
                    formid = leave_sub(stud.leaves, root);
                    leaveRecord[name].push_back(formid);
                    cout << "Leave Application submitted with Form ID: "<< formid <<endl;
                    break;
                case 3:
                    view_leave_record(stud.name, root);
                    break;
                case 4:
                    track_request(stud.name, root);
                    break;
                case 5:
                    return;
                default:
                    cout << "Invalid option. Please try again.\n";

            }
        }
    }

    else if (role == "FACULTY"){
        int input = 0;
        while (input != 4){
            cout << "Enter 1 to view your profile details\n";
            cout << "Enter 2 to view your pending leave applications\n";
            cout << "Enter 3 to log out\n";

            cin >> input;

            switch (input){
                case 1:
                    view_faculty_details(stud);
                    break;
                case 2:
                    if (stud.rollno == "HOD"){
                        stage = "HOD";
                    }
                    else if (stud.rollno == "DOAA"){
                        stage = "DOAA";
                    }
                    else{
                        break;
                    }
                    view_leave_record_in_queue(root, stage);
                    break;
                case 3:
                    return;
                default:
                    cout << "Invalid option. Please try again.\n";


            }
        }
    }
    else if (role == "ADMIN"){
        int input = 0;
        string name;
        while (input != 6){
            cout << "Enter 1 to view your profile details\n";
            cout << "Enter 2 to view the student record\n";
            cout << "Enter 3 to view the leave records for a particular student\n";
            cout << "Enter 4 to simulate the workflow for a single Leave request\n";
            cout << "Enter 5 to simulate the going of a Leave Application to the Next stage\n";
            cout << "Enter 6 to log out\n";

            cin >> input;

            switch (input){
                case 1:
                    view_admin_details(stud);
                    break;
                case 2:
                    print_record();
                    break;
                case 3:
                    cout << "Whose Leave Record would you like to view?\n";
                    cin >> name;
                    view_leave_record(name, root);
                    break;
                case 4:
                    cout << "Whose Leave Record would you like to see go to the end of the workflow?\n";
                    cin >> name;
                    simulate_workflow_for_singleform(root, name);
                    break;
                case 5:
                    cout << "Whose Leave Record would you like to go to the next stage?\n";
                    cin >> name;
                    simulate_single_step(root, name);
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid option. Please try again.\n";

            }
        }
    }
}


void main_menu(void) {
    int input = 0;

    cout << "Welcome to the Online Leave Management System for IITG students!!\n";

    while (input != 3) {
        cout << "Enter 1 to Signup\n";
        cout << "Enter 2 to Login\n";
        cout << "Enter 3 to exit\n";

        cout << "What do you want to do? \n";
        cin >> input;

        switch (input) {
            case 1:
                signup();
                break;

            case 2: {
                student stud = login();

                if (!stud.name.empty()){
                    user_dashboard(stud);
                }
                break;
            }

            case 3:
                break;
            default:
                cout << " Invalid input. Try again.\n";
        }
    }

}


