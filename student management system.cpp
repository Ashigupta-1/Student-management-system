#include <iostream>  // For standard input and output
#include <fstream>   // For file handling
#include <string>    // For string operations

using namespace std; // To avoid typing "std::" before standard library functions

// Student class
class Student {
public:
    int id;
    string name;
    int age;
    string gender;
    float grade;

    void input() {
        cout << "Enter Student ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cout << "Enter Grade: ";
        cin >> grade;
    }

    void display() {
        cout << "ID: " << id << ", Name: " << name
             << ", Age: " << age << ", Gender: " << gender
             << ", Grade: " << grade << endl;
    }
};

// Add a student to file
void addStudent() {
    Student student;
    student.input();

    ofstream outFile("students.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *>(&student), sizeof(student));
    outFile.close();

    cout << "Student record added successfully.\n";
}

// Display all students from file
void displayStudents() {
    Student student;
    ifstream inFile("students.dat", ios::binary);
    if (!inFile) {
        cout << "Error opening file!\n";
        return;
    }

    cout << "\nAll Students:\n";
    while (inFile.read(reinterpret_cast<char *>(&student), sizeof(student))) {
        student.display();
    }
    inFile.close();
}

// Search student by ID
void searchStudent(int id) {
    Student student;
    ifstream inFile("students.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char *>(&student), sizeof(student))) {
        if (student.id == id) {
            student.display();
            found = true;
            break;
        }
    }

    inFile.close();
    if (!found) {
        cout << "Student with ID " << id << " not found.\n";
    }
}

// Modify a student record
void modifyStudent(int id) {
    Student student;
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (file.read(reinterpret_cast<char *>(&student), sizeof(student))) {
        if (student.id == id) {
            cout << "Current record:\n";
            student.display();
            cout << "\nEnter new details:\n";
            student.input();

            int pos = -1 * static_cast<int>(sizeof(student));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char *>(&student), sizeof(student));
            cout << "Record updated successfully.\n";
            found = true;
            break;
        }
    }

    file.close();
    if (!found) {
        cout << "Student with ID " << id << " not found.\n";
    }
}

// Delete a student record
void deleteStudent(int id) {
    Student student;
    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&student), sizeof(student))) {
        if (student.id != id) {
            outFile.write(reinterpret_cast<char *>(&student), sizeof(student));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        cout << "Record deleted successfully.\n";
    } else {
        cout << "Student with ID " << id << " not found.\n";
    }
}

// Main menu
int main() {
    int choice, id;

    do {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by ID\n";
        cout << "4. Modify Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                cout << "Enter Student ID to search: ";
                cin >> id;
                searchStudent(id);
                break;
            case 4:
                cout << "Enter Student ID to modify: ";
                cin >> id;
                modifyStudent(id);
                break;
            case 5:
                cout << "Enter Student ID to delete: ";
                cin >> id;
                deleteStudent(id);
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}