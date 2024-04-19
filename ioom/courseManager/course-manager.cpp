#include "course-manager.h"

//Definitions for Student class
int Student::getID(){return ID;}
string Student::getName(){return name;}


// Definitions for LectureCourse methods
void LectureCourse::displayDetails() {
    cout << "\n\n\tLecture Course Details:" << endl;
    cout << "\t\tCode: " << code << "\n\t\t Title: " << title << "\n\t\t Instructor: " << instructor << "\n\t\t Credits: " << credits << "\n\t\tAttendance: ";
    for (const auto& str : attendance) {
        cout << str << " ";
    }
}

char LectureCourse::calculateGrade() {
    int midsem = 0, endsem = 0;
    float avg = 0.0;
    cout << "\n\tEnter Midsem marks out of 100 (enter -1 if paper not happened): ";
    cin >> midsem;
    cout << "\n\tEnter Endsem marks out of 100 (enter -1 if paper not happened): ";
    cin >> endsem;

    if ((midsem > 0 && midsem <= 100) && (endsem > 0 && endsem <= 100)) {
        avg = (float)(midsem + endsem) / 2.0;
        if (avg >= 75) {
            cout << "\n\t\tGRADE A"<<endl;
            return 'A';
        } else {
            cout << "\n\t\tGRADE B"<<endl;
            return 'B';
        }
    } else {
        cout << "\n\t\tGRADE CANNOT BE CALCULATED WITHOUT BOTH PAPERS"<<endl;
        return 'C';
    }
}

// Definitions for LabCourse methods
void LabCourse::displayDetails() {
    cout << "\n\n\tLab Course Details:" << endl;
    cout << "\t\tCode: " << code << "\n\t\t Title: " << title << "\n\t\t Instructor: " << instructor << "\n\t\t Credits: " << credits << "\n\t\tAttendance: ";
    for (const auto& str : attendance) {
        cout << str << " ";
    }
}

char LabCourse::calculateGrade() {
    int midsem = 0, endsem = 0;
    float avg = 0.0;
    cout << "\n\tEnter Midsem lab marks out of 20 (enter -1 if paper not happened): ";
    cin >> midsem;
    cout << "\n\tEnter Endsem marks out of 20 (enter -1 if paper not happened): ";
    cin >> endsem;

    if ((midsem > 0 && midsem <= 20) && (endsem > 0 && endsem <= 20)) {
        avg = (float)(midsem + endsem) / 2.0;
        if (avg >= 14) {
            cout << "\n\t\tGRADE A"<<endl;
            return 'A';
        } else {
            cout << "\n\t\tGRADE B"<<endl;
            return 'B';
        }
    } else {
        cout << "\n\t\tGRADE CANNOT BE CALCULATED WITHOUT BOTH PAPERS"<<endl;
        return 'C';
    }
}

// Definitions for SeminarCourse methods
void SeminarCourse::displayDetails() {
    cout << "\n\n\tSeminar Course Details:" << endl;
    cout << "\t\tCode: " << code << "\n\t\t Title: " << title << "\n\t\t Instructor: " << instructor << "\n\t\t Credits: " << credits << "\n\t\tAttendance: ";
    for (const auto& str : attendance) {
        cout << str << " ";
    }
}

char SeminarCourse::calculateGrade() {
    float percent = 0.0;
    int present = 0;
    int total_days = 0;
    for (const auto& str : attendance) {
        if (str == "p") {
            present++;
        }
        total_days++;
    }
    if (total_days > 0) {
        percent = (float)present / total_days;
        if (percent >= 0.6) {
            cout << "\n\t\tGRADE A"<<endl;
            return 'A';
        } else {
            cout << "\n\t\tGRADE B"<<endl;
            return 'B';
        }
    } else {
        cout << "\n\t\tGRADE CANNOT BE CALCULATED (NO CLASS HAS HAPPENED)"<<endl;
        return 'C';
    }
}

// Definitions for CourseManager methods
void CourseManager::addCourse(int ID, Course* course) {
    for(auto stu= students.begin(); stu!=students.end(); ++stu){
        if((*stu).getID() == ID){
            (*stu).mycourses.push_back(course);
        }
    }
}

void CourseManager::addStudent(Student stu){
    students.push_back(stu);
}

void CourseManager::removeCourse(int ID, string code) {
    for(auto stu= students.begin(); stu!=students.end(); ++stu){
        if((*stu).getID() == ID){
            for (auto it = (*stu).mycourses.begin(); it != (*stu).mycourses.end(); ++it) {
                if ((*it)->getCode() == code) {
                    delete *it;
                    (*stu).mycourses.erase(it);
                    break;
                }   
            }
        }
    }
}

void CourseManager::display_my_Courses(int ID) {
     for(auto stu= students.begin(); stu!=students.end(); ++stu){
        if((*stu).getID() == ID){
                for (auto& course : (*stu).mycourses) {
                    course->displayDetails();
                }
        }
    }
}

CourseManager::~CourseManager() {
    for(auto stu= students.begin(); stu!=students.end(); ++stu){
            for (auto& course : (*stu).mycourses) {
                delete course;
            }
        }
    }

void CourseManager::markAttendance(int ID, string att, string code) {
    for(auto stu= students.begin(); stu!=students.end(); ++stu){
        if((*stu).getID() == ID){
            for (auto it = (*stu).mycourses.begin(); it != (*stu).mycourses.end(); ++it) {
                if ((*it)->getCode() == code) {
                if (att == "p" || att == "P") {
                    (*it)->markAttendance("p");
                }
                if (att == "a" || att == "A") {
                    (*it)->markAttendance("a");
                }
                return;
                }
            }
        cout << "\n\tWRONG CHOICE" << endl;
        }
    }
}

void CourseManager::calculateGrade(int ID, string code) {
    for(auto stu= students.begin(); stu!=students.end(); ++stu){
        if((*stu).getID() == ID){
            for (auto it = (*stu).mycourses.begin(); it != (*stu).mycourses.end(); ++it) {
                if ((*it)->getCode() == code) {
                    (*it)->calculateGrade();
                    return;
                }
            }
        cout << "\n\tWRONG CHOICE" << endl;
        }
    }
}
