#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <string>
#include <list>
#include <unordered_map>

using namespace std;



struct student{
    string name;
    string branch;
    string rollno;
    int leaves;
    int *pending_leaves;
    string username;
    string role;


};


extern std::list<student> record;

extern std::unordered_map<string, string> user_database;


int regex_check(string s);
std::string sha256(const std::string& input);
void disableEcho();
void enableEcho();
bool validate_rollno(string s, string branch);
void signup();
student login();
void print_record();
void setCurrentStudent(const student& s);
student getCurrentStudent();
student getStudentByUsername(const string& username);
void view_student_details(student stud);
void view_faculty_details(student stud);
void view_admin_details(student stud);


#endif
