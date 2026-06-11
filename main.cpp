#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>     // for numeric_limits
#include <cctype>     // for std::isdigit
#include <iomanip>    // for std::setprecision, std::fixed
#include <sstream>    

using namespace std;

class AttendanceSystem {
private:
    // Encapsulated data: Cannot be modified from outside the class
    const string subjects[6] = {"OOP", "EE", "DBE", "MATH_III", "DLD", "DS"};

    // --- Helper Methods (Private) ---
    string extractRollNo(const string& regd_no) {
        size_t len = regd_no.length();
        return (len >= 3) ? regd_no.substr(len - 3) : regd_no;
    }

    int rollDisplay(const string& roll_str) {
        try {
            return stoi(roll_str); 
        } catch (...) {
            return 0; 
        }
    }

    bool adminLogin() {
        string user, pass;
        cout << "Enter admin username: ";
        cin >> user;
        cout << "Enter admin password: ";
        cin >> pass;
        return (user == "admin" && pass == "admin");
    }

    bool studentLogin(string &regd_no, string &roll, string &name) {
        cout << "Enter your Registration Number: ";
        cin >> regd_no;

        ifstream in("studentlist.txt");
        if (!in) return false;

        string r, temp_roll, temp_name;
        while (in >> r >> temp_roll) {
            in >> ws;
            getline(in, temp_name);
            if (regd_no == r) {
                roll = temp_roll;
                name = temp_name; 
                return true;
            }
        }
        return false;
    }

    // --- Core Admin Features (Private) ---
    void addStudent() {
        string regd_no, name;
        cout << "Enter Student Registration Number: ";
        cin >> regd_no;

        cout << "Enter Student Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        getline(cin, name);

        string roll = extractRollNo(regd_no);

        ifstream check("studentlist.txt");
        if (check) {
            string r, temp_roll, temp_name;
            bool exists = false;
            while (check >> r >> temp_roll) {
                check >> ws;                
                getline(check, temp_name);   
                if (r == regd_no) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                cout << "Student already exists!\n";
                return;
            }
        }
        
        ofstream out("studentlist.txt", ios::app);
        if (!out) {
            cout << "Error: cannot open studentlist.txt for writing.\n";
            return;
        }
        out << regd_no << " " << roll << " " << name << "\n";

        for (int i = 0; i < 6; i++) {
            ofstream subFile((regd_no + "_" + subjects[i] + ".txt").c_str(), ios::app);
        }
        cout << "Student added successfully! Regd No.: " << regd_no
             << ", Roll No.: " << rollDisplay(roll)
             << ", Name: " << name << "\n";
    }

    void markAttendance() {
        int subChoice;
        cout << "\nSelect Subject for Attendance:\n";
        for (int i = 0; i < 6; i++)
            cout << (i + 1) << ". " << subjects[i] << "\n";
        cout << "Enter choice (1-6): ";
        cin >> subChoice;

        if (subChoice < 1 || subChoice > 6) {
            cout << "Invalid choice!\n";
            return;
        }
        string selectedSubject = subjects[subChoice - 1];

        time_t now = time(0);
        tm* ltm = localtime(&now);
        int day = ltm->tm_mday, month = ltm->tm_mon + 1, year = ltm->tm_year + 1900;

        cout << "\nMarking attendance for " << selectedSubject
             << " on " << day << "/" << month << "/" << year << "\n";

        ifstream in("studentlist.txt");
        if (!in) {
            cout << "No students found. Add students first.\n";
            return;
        }

        string regd_no, roll, name;
        int status;
        while (in >> regd_no >> roll) {
            in >> ws;                
            getline(in, name);
            cout << "Roll No. " << rollDisplay(roll) << " (Regd: " << regd_no
                 << "), Name: " << name << " - Present(1)/Absent(0): ";

            while (true) {
                if (cin >> status && (status == 0 || status == 1)) break;
                cout << "Enter 1 for Present or 0 for Absent: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            ofstream subFile((regd_no + "_" + selectedSubject + ".txt").c_str(), ios::app);
            if (!subFile) {
                cout << "Error writing attendance file for " << regd_no << "\n";
                continue;
            }
            subFile << day << " " << month << " " << year << " " << status << "\n";
        }
        cout << "Attendance marked for " << selectedSubject << "!\n";
    }

    void viewAllStudentsAttendance() {
        cout << "\n=== Overall Attendance Report ===\n";
        ifstream in("studentlist.txt");
        if (!in) {
            cout << "No students found.\n";
            return;
        }

        string regd_no, roll, name;
        cout << fixed << setprecision(2);
        while (in >> regd_no >> roll) {
            in >> ws;
            getline(in, name);
            cout << "\nRoll No. " << rollDisplay(roll) << ", Name: " << name
                 << " (Regd: " << regd_no << "):\n";

            for (int i = 0; i < 6; i++) {
                ifstream subFile((regd_no + "_" + subjects[i] + ".txt").c_str());
                int total = 0, present = 0, d, m, y, stat;
                if (subFile) {
                    while (subFile >> d >> m >> y >> stat) {
                        total++;
                        if (stat == 1) present++;
                    }
                }
                double percent = (total > 0) ? (present * 100.0 / total) : 0.0;
                cout << "  " << subjects[i] << ": " << present << "/" << total
                     << " (" << percent << "%)\n";
            }
        }
    }

    void studentReport(const string& regd_no, const string& roll, const string& name) {
        cout << "\n=== Attendance Report for Roll No. " << rollDisplay(roll)
             << " (" << name << "), Regd: " << regd_no << " ===\n";
        int subChoice;
        cout << "\n1. View all subjects\n2. View specific subject\nChoice: ";
        cin >> subChoice;

        cout << fixed << setprecision(2);
        if (subChoice == 1) {
            for (int i = 0; i < 6; i++) {
                cout << "\n--- " << subjects[i] << " ---\n";
                ifstream subFile((regd_no + "_" + subjects[i] + ".txt").c_str());
                int total = 0, present = 0, d, m, y, stat;
                if (subFile) {
                    while (subFile >> d >> m >> y >> stat) {
                        cout << d << "/" << m << "/" << y << " - " << (stat ? "Present" : "Absent") << "\n";
                        total++;
                        if (stat == 1) present++;
                    }
                }
                double percent = (total > 0) ? (present * 100.0 / total) : 0.0;
                cout << "Total: " << total << ", Present: " << present
                     << " (" << percent << "%)\n";
            }
        } else if (subChoice == 2) {
            cout << "\nSelect Subject:\n";
            for (int i = 0; i < 6; i++)
                cout << (i + 1) << ". " << subjects[i] << "\n";
            cout << "Choice: ";
            cin >> subChoice;

            if (subChoice >= 1 && subChoice <= 6) {
                string selectedSubject = subjects[subChoice - 1];
                cout << "\n--- " << selectedSubject << " Attendance ---\n";
                ifstream subFile((regd_no + "_" + selectedSubject + ".txt").c_str());
                int total = 0, present = 0, d, m, y, stat;
                if (subFile) {
                    while (subFile >> d >> m >> y >> stat) {
                        cout << d << "/" << m << "/" << y << " - " << (stat ? "Present" : "Absent") << "\n";
                        total++;
                        if (stat == 1) present++;
                    }
                }
                double percent = (total > 0) ? (present * 100.0 / total) : 0.0;
                cout << "Total: " << total << ", Present: " << present
                     << " (" << percent << "%)\n";
            }
        }
    }

    // --- Core Menus (Private) ---
    void adminMenu() {
        int ch;
        do {
            cout << "\n=== Admin Menu ===\n";
            cout << "1. Add Student\n2. Mark Attendance\n3. View All Students Attendance\n0. Logout\nChoice: ";
            cin >> ch;
            switch (ch) {
                case 1: addStudent(); break;
                case 2: markAttendance(); break;
                case 3: viewAllStudentsAttendance(); break;
                case 0: cout << "Logging out...\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (ch != 0);
    }

    void studentMenu() {
        string regd_no, roll, name;
        if (studentLogin(regd_no, roll, name)) {
            studentReport(regd_no, roll, name);
        } else {
            cout << "Student not found!\n";
        }
    }

public:
    // This is the ONLY method exposed to the main function
    void run() {
        int choice;
        do {
            cout << "\n=== Smart Attendance Marker ===\n";
            cout << "Subjects: ";
            for (int i = 0; i < 6; i++) {
                cout << subjects[i] << (i < 5 ? ", " : "\n");
            }
            cout << "1. Admin Login\n2. Student Report\n0. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    if (adminLogin()) adminMenu();
                    else cout << "Admin login failed!\n";
                    break;
                case 2: studentMenu(); break;
                case 0: cout << "Goodbye!\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 0);
    }
};

int main() {
    // Instantiate the object and run the system
    AttendanceSystem app;
    app.run();
    return 0;
}
