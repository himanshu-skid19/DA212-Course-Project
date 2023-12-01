#include <iostream>
#include "string.h"
#include <unordered_map>
#include <regex>
#include <list>
#include <iomanip>
#include <openssl/evp.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace std;

string ADMIN_PASSKEY = "C0UR$E_PR0JECT";

struct student{
    string name;
    string branch;
    string rollno;
    int leaves;
    int *pending_leaves;
    string username;
    string role;


};

student currentStudent;

void setCurrentStudent(const student& s){
    currentStudent = s;
}

student getCurrentStudent(){
    return currentStudent;
}

std::unordered_map<string, int> BRANCHE_CODES = {
    {"BSBE",  0}, {"CE", 1}, {"CL", 2}, {"CSE", 3}, {"CST", 4}, {"DSAI", 5}, {"ECE", 6}, {"EEE", 7}, {"EP", 8},{"ME", 9}, {"MNC", 10}

};

std::unordered_map<int, string> BRANCHES = {
    {0, "BSBE"}, {1, "CE"}, {2, "CL"}, {3, "CSE"}, {4, "CST"}, {5, "DSAI"},
    {6, "ECE"}, {7, "EEE"}, {8, "EP"}, {9, "ME"}, {10, "MNC"}
};

std::unordered_map<string, string> user_database;

std::list<student> record;

vector<string> roles = {"STUDENT", "FACULTY", "ADMIN"};


int regex_check(string s){
    if (regex_match(s, regex("([Bb])(.*)"))){
        return 0;
    }
    else if (regex_match(s, regex("([Cc])(.*)"))){
        if (regex_match(s, regex("([Cc][iI])(.*)"))){
            return 1;

        }
        else if (regex_match(s, regex("([Cc][eE])$"))){
            return 1;

        }
        else if (regex_match(s, regex("([Cc][Hh])(.*)"))){
            if (regex_match(s, regex("(.*)([ ][Ee])(.*)"))){
            return 2;

            }
            else if (regex_match(s, regex("(.*)([ ][Ss])(.*)"))){
            return 4;

        }
        }
        else if (regex_match(s, regex("([Cc][lL])$"))){
            return 2;

        }
        else if (regex_match(s, regex("([Cc][sS][tT])$"))){
            return 4;


        }
        else if (regex_match(s, regex("([Cc][sS][Ee])$"))){
            return 3;


        }
        else if (regex_match(s, regex("([Cc][oO])(.*)$"))){
            return 3;


        }
    }
    else if (regex_match(s, regex("([Dd])(.*)"))){
        return 5;
    }

    else if (regex_match(s, regex("([Ee])(.*)"))){
        if (regex_match(s, regex("(.*)([ ][Cc])(.*)"))){
        return 6;
        }
        else if (regex_match(s, regex("(.*)([ ][eE])(.*)"))){
        return 7;
        }
        else if (regex_match(s, regex("([Ee][cC][eE])$"))){
        return 6;
        }
        else if (regex_match(s, regex("([Ee][Ee][eE])$"))){
        return 7;
        }
        else if (regex_match(s, regex("([Ee][pP])$"))){
        return 8;
        }
        else if (regex_match(s, regex("([Ee][Nn])(.*)"))){
        return 8;;
        }

    }
    else if (regex_match(s, regex("([Mm])(.*)"))){
        if (regex_match(s, regex("([Mm][eE])(.*)"))){
        return 9;
        }
        else if (regex_match(s, regex("([Mm][Ee])$"))){
        return 9;
        }
        else if (regex_match(s, regex("([Mm][aA])(.*)"))){
        return 10;
        }
        else if (regex_match(s, regex("([Mm][nN][cC])$"))){
        return 10;
        }
        else if (regex_match(s, regex("([Mm][aA][cC])$"))){
        return 10;
        }
    }

    return -1;
}



std::string sha256(const std::string& input) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname("sha256");

    if (!md) {
        std::cerr << "Error: SHA-256 not supported." << std::endl;
        return "";
    }

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    EVP_cleanup();

    return ss.str();
}


void disableEcho() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enableEcho() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

bool validate_rollno(string s, string branch){
    int b = regex_check(branch);
    string b_ = to_string(b);
    string pattern = "([1-9][0-9])([0-9])([0-9][0-9][0-9])$";
    regex regex_pattern(pattern);

    smatch match;
    if (regex_search(s, match, regex_pattern)){
        string patt = "(^.{2})([0-9])(.{3}$)";
        regex regex_patt(patt);
        smatch match2;
        if (regex_search(s, match2, regex_patt)){
            if (match2[2].str() == b_){
                return 1;
            }
        }
    }
    return 0;
}

void signup(){
    student stud;
    cout << "Enter your name: \n";
    string name;
    cin >> name;

    stud.name = name;

    string role;
    cout << "Are you a STUDENT/FACULTY/ADMIN?\n";
    cin >> role;

    if (role == "STUDENT"){
        int r = 1;
        stud.role = "STUDENT";
    while (r){
        cout << "Enter your branch \n";
        string s;
        cin >> s;
        if (regex_check(s) == -1){
            cout << "Invalid Branch\n";
            continue;
        }
        else{
            stud.branch = s;
            r=0;
        }
    }


    while (true){
            string rollno;
            cout << "Enter your rollno: \n";
            cin >> rollno;

            if (validate_rollno(rollno, stud.branch)){
                stud.rollno = rollno;
                break;

            }
            cout << "Invalid rollno\n";

        }

    string user;
    cout << "Enter a username: ";
    cin >> user;


    while (true){
        string pass1;
        string pass2;
        disableEcho();
        cout << "Enter a password: \n";
        cin >> pass1;
        cout << "Enter the password again: \n";
        cin >> pass2;
        if (pass1 == pass2){
            string hashed_pass = sha256(pass1);
            user_database[user] = hashed_pass;
            stud.username = user;
            enableEcho();
            break;
        }
        cout << "Passwords don't match. Try again\n";

    }
    stud.leaves = 30;

    record.push_back(stud);
    }

    else if (role == "FACULTY"){
        int r = 1;

        stud.role = "FACULTY";
    while (r){
        cout << "Enter your branch \n";
        string s;
        cin >> s;
        if (regex_check(s) == -1){
            cout << "Invalid Branch\n";
            continue;
        }
        else{
            stud.branch = s;
            r=0;
        }
    }

    string title;

    cout << "Are you the HOD/DOAA?\n";
    cin >> title;

    if (title == "HOD" || title == "DOAA"){
        string user;
        cout << "Enter a username: ";
        cin >> user;


        while (true){
            string pass1;
            string pass2;
            disableEcho();
            cout << "Enter a password: \n";
            cin >> pass1;
            cout << "Enter the password again: \n";
            cin >> pass2;
            if (pass1 == pass2){
                string hashed_pass = sha256(pass1);
                user_database[user] = hashed_pass;
                stud.username = user;
                enableEcho();
                break;
            }
            cout << "Passwords don't match. Try again\n";

        }

        stud.rollno = title;


        record.push_back(stud);
    }

    else{
         cout << "We are sorry but only the HOD and DOAA have authorization to approve Leave requests\n";
        return;
    }


    }

    else if(role == "ADMIN"){
        stud.role = "ADMIN";
        string passkey;
        cout << "What is the ADMIN_PASSKEY?\n";
        cout << "NOTE: For the purpose of demonstration the passkey is displayed as follows: C0UR$E_PR0JECT\n";
        cin >> passkey;

        if (passkey != "C0UR$E_PR0JECT"){
            cout << "We are sorry but you do not have authorization to sign up as an admin\n";
            return;
        }

        string user;
        cout << "Enter a username: ";
        cin >> user;


        while (true){
            string pass1;
            string pass2;
            disableEcho();
            cout << "Enter a password: \n";
            cin >> pass1;
            cout << "Enter the password again: \n";
            cin >> pass2;
            if (pass1 == pass2){
                string hashed_pass = sha256(pass1);
                user_database[user] = hashed_pass;
                stud.username = user;
                enableEcho();
                break;
            }
            cout << "Passwords don't match. Try again\n";

        }


    record.push_back(stud);

    }
    else {

        cout << "Invalid role. Try Again.\n";
        return;
    }



}

student getStudentByUsername(const string& username) {
    for (const auto& element : record) {
        if (element.username == username){
            setCurrentStudent(element);
            return element;
        }
    }
    return student();
}

student login(){
    while (true){
        string username;
        cout << "Enter your username: ";
        cin >> username;

        string enteredPass;
        cout << "Enter your password: \n";
        disableEcho();
        cin >> enteredPass;
        enableEcho();
        string hash_entered_pass = sha256(enteredPass);

        if (user_database.find(username) != user_database.end() && user_database[username] == hash_entered_pass){
            cout << "Correct\n";
            return getStudentByUsername(username);
        }

        cout << "Invalid User ID or Password\n";
    }
    return student();
}


void view_student_details(student stud){
    cout << "Name: " << stud.name <<endl;
    cout << "Roll no: " << stud.rollno <<endl;
    cout << "Branch: " << stud.branch <<endl;
    cout << "Number of leaves remaining: " << stud.leaves << endl;
    cout << "Role: " << stud.role << endl;


}

void view_faculty_details(student stud){
    cout << "Name: " << stud.name <<endl;
    cout << "Branch: " << stud.branch <<endl;
    cout << "title: " << stud.rollno << endl;
    cout << "Role: " << stud.role << endl;


}

void view_admin_details(student stud){
    cout << "Name: " << stud.name <<endl;
    cout << "Role: " << stud.role << endl;


}
void print_record(){
    for (const auto &element : record){
        cout << "Name: ";
        cout << element.name << endl;
        cout << "Branch: ";
        cout << element.branch << endl;
        cout << "Rollno: ";
        cout << element.rollno << endl;
    }
    cout << endl;
}

