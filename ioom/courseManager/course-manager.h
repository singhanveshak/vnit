#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Student{
    private:
        string name;
        int ID;
    public:
        Student(int id, string name) : ID(ID), name(name) {}
        vector <Course*> mycourses;
        string getName();
        int getID();
};

// Base class
class Course {
protected:
    string code;
    string title;
    string instructor;
    int credits;
    vector<string> attendance;

public:
    Course(string c, string t, string i, int cr) : code(c), title(t), instructor(i), credits(cr) {}
    virtual void displayDetails() = 0;
    virtual char calculateGrade() = 0;
    string getCode() { return code; }
    void markAttendance(string att) { attendance.push_back(att); }
};

class LectureCourse : public Course {
public:
    LectureCourse(string c, string t, string i, int cr) : Course(c, t, i, cr) {}
    void displayDetails() override;
    char calculateGrade() override;
};

class LabCourse : public Course {
public:
    LabCourse(string c, string t, string i, int cr) : Course(c, t, i, cr) {}
    void displayDetails() override;
    char calculateGrade() override;
};

class SeminarCourse : public Course {
public:
    SeminarCourse(string c, string t, string i, int cr) : Course(c, t, i, cr) {}
    void displayDetails() override;
    char calculateGrade() override;
};

class CourseManager {
private:
    vector <Student> students;
    vector <Course> courses;

public:
    void addStudent(Student stu);
    void addCourse(int ID, Course* course);
    void removeCourse(int ID, string code);
    void display_my_Courses(int ID);
    ~CourseManager();
    void markAttendance(int ID, string code, string att);
    void calculateGrade(int ID, string code);
};
