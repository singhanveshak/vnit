import java.util.Scanner;

class Student {
    private int id;
    private String name;
    private double grade;

    // Parameterized constructor
    public Student(int id, String name, double grade) {
        this.id = id;
        this.name = name;
        this.grade = grade;
    }

    // Getter methods
    public int getId() {
        return this.id;
    }

    public String getName() {
        return this.name;
    }

    public double getGrade() {
        return this.grade;
    }

    // Display student information
    public void displayInfo() {
        System.out.println("ID: " + id);
        System.out.println("Name: " + name);
        System.out.println("Grade: " + grade);
        System.out.println();
    }
}

class StudentManager {
    private int MAX_SIZE = 10;
    int index;
    private Student[] ArrayList;

    public StudentManager() {
        this.ArrayList = new Student[MAX_SIZE];
        this.index=-1;      //initialize with -1
    }
    // Add a new student
    public void addStudent(int id, String name, double grade) {
        index++;
        this.ArrayList[this.index] = new Student(id,name,grade);
    }

    // Display information about all students
    public void displayStudents() {
        for (int i=0;i<=this.index;i++) {
            this.ArrayList[i].displayInfo();
        }
    }

    // Calculate and display the average grade of all students
    public void displayAverageGrade() {
        double totalGrade = 0;

        for(int i=0;i<=this.index;i++){
            totalGrade+=this.ArrayList[i].getGrade();
        }
        double averageGrade = totalGrade / (this.index+1);
        System.out.println("Average grade of all students: " + averageGrade);
    }
}

public class p {
    public static void main(String[] args) {
        StudentManager studentManager = new StudentManager();
        Scanner scanner = new Scanner(System.in);

        //dynamic implementaion
        System.out.println("Enter number of students");
        int k = scanner.nextInt();
        // Add students
        for (int i = 0; i < k; i++) {
            System.out.println("Enter student ID:");
            int id = scanner.nextInt();
            System.out.println("Enter student name:");
            String name = scanner.next();
            System.out.println("Enter student grade:");
            double grade = scanner.nextDouble();

            studentManager.addStudent(id,name,grade);
        }

        System.out.println("\nStudent information:");
        studentManager.displayStudents();
        studentManager.displayAverageGrade();
        
        scanner.close();
    }
}
