#include <iostream>
#include <vector>
#include <string>
#include <limits>  
using namespace std;

// ----------------- Student Class -----------------
class student
{
private:
    string name;
    int age;
    int rollno;
    double avg;
    vector<int> marks;

public:
    student(string n, int a, int r) : name(n), age(a), rollno(r), avg(0) {}

    int getRollNo() const { return rollno; }

    void inputMarks(int numSubjects)
    {
        marks.clear();
        cout << "Enter " << numSubjects << " subject marks for " << name << ":\n";
        for (int i = 0; i < numSubjects; i++)
        {
            int mark;
            cout << "Subject " << (i + 1) << ": ";
            cin >> mark;
            marks.push_back(mark);
        }
        calculateAverage();
    }

    void calculateAverage()
    {
        int sum = 0;
        for (int mark : marks)
            sum += mark;
        avg = marks.empty() ? 0 : static_cast<double>(sum) / marks.size();
    }

    void displayInfo() const
    {
        cout << "Name: " << name << ", Age: " << age
             << ", Roll No: " << rollno << ", Average Marks: " << avg << "\n";
    }
};

// ----------------- Student Management Class -----------------
class studentmanagement
{
private:
    vector<student> students;

public:
    void addstudent()
    {
        string n;
        int a, r;

        cout << "Enter name of student: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush input
        getline(cin, n);
        // if (n.size() > 10)
        // {
        //     cout << "Name too long! Only first 10 characters will be used.\n";
        //     n = n.substr(0, 10);
        // }

        cout << "Enter age and roll no: ";
        cin >> a >> r;

        // Check for duplicate roll number
        for (auto &s : students)
        {
            if (s.getRollNo() == r)
            {
                cout << " Student with Roll No " << r << " already exists!\n";
                return;
            }
        }

        student s(n, a, r);

        int numSubjects;
        cout << "Enter number of subjects: ";
        cin >> numSubjects;
        while (numSubjects <= 0)
        {
            cout << "Number of subjects must be positive. Enter again: ";
            cin >> numSubjects;
        }

        s.inputMarks(numSubjects);

        students.push_back(s); // Add student to vector
        cout << " Student added successfully!\n";
    }

    void deletestudent(int r)
    {
        for (auto it = students.begin(); it != students.end(); ++it)
        {
            if (it->getRollNo() == r)
            {
                students.erase(it);
                cout << " Student with Roll No " << r << " deleted!\n";
                return; // exit after deleting
            }
        }
        cout << " Student with Roll No " << r << " not found!\n";
    }

    void displayall()
    {
        if (students.empty())
        {
            cout << "No students to display\n";
            return;
        }
        cout << "\n--- Student List ---\n";
        for (const auto &s : students)
        {
            s.displayInfo();
        }
    }
    void updatestudentdetails(int r)
    {
        for (auto &s : students)
        {
            if (s.getRollNo() == r)
            {
                string n;
                int a;

                cout << "Enter new name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush input
                getline(cin, n);

                cout << "Enter new age: ";
                cin >> a;

                s = student(n, a, r); // Recreate student with new details

                int numSubjects;
                cout << "Enter number of subjects: ";
                cin >> numSubjects;
                while (numSubjects <= 0)
                {
                    cout << "Number of subjects must be positive. Enter again: ";
                    cin >> numSubjects;
                }

                s.inputMarks(numSubjects);

                cout << " Student details updated successfully!\n";
                return;
            }
        }
        cout << " Student with Roll No " << r << " not found!\n";
    }
};

// ----------------- Main Function -----------------
int main()
{
    studentmanagement sm;
    int choice;

    while (true)
    {
        cout << "\n1. Add Student\n2. Delete Student\n3. Display All Students\n4. Update student details \n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            sm.addstudent();
            break;
        case 2:
        {
            int r;
            cout << "Enter roll no to delete: ";
            cin >> r;
            sm.deletestudent(r);
            break;
        }
        case 3:
            sm.displayall();
            break;
        case 4:
        {
            int r;
            cout << "Enter roll no to update details: ";
            cin >> r;
            sm.updatestudentdetails(r);
            break;
        }
        case 5:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
