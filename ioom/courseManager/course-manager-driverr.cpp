#include "course-manager.h"
using namespace std;

int main() {
    // add courses to my account
    CourseManager manager;
    manager.addStudent(new Student(1,"Aisha"));
    manager.addStudent(new Student(2,"JJ"));
    manager.addCourse(1,new Course("AST101","Intro to astro","mr. hiran", 4));

    // Displaying all courses
    cout<<"\n---------------------------------------\nADDING ALL COURSE ..."<<endl;
    manager.display_my_Courses(1);

    //remove a courses
    cout<<"\n----------------------------------------\nREMOVING COURSES ..."<<endl;
    manager.removeCourse(1,"CHM101");
    manager.display_my_Courses(1);

    //marking attendance
    cout<<"\n----------------------------------------\nMARK ATTENDANCE ..."<<endl;
    string code;
    string att;
    do{
        cout<<"Enter course code for attendance (enter 0 when done): ";
        cin>>code;
        cout<<"Enter attendace p/a: ";
        cin>>att;
        manager.markAttendance(1,code,att);
    }while(code!="0");

    //marking grades
    cout<<"\n----------------------------------------\nCALCULATE GRADES ..."<<endl;
    code="1";
    do{
        cout<<"Enter course code for grade (enter 0 when done): ";
        cin>>code;
        manager.calculateGrade(code);
    }while(code!="0");

    cout<<"---------------------------------------\nFINALLY ..."<<endl;
    manager.displayAllCourses();

    return 0;
}
