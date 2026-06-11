# Smart Attendance Management System

## Overview
This is a robust, console-based application engineered to efficiently track, manage, and update student attendance records. Built entirely in C++, the system strictly adheres to Object-Oriented Programming (OOP) principles to ensure secure data encapsulation and modular design.

## Technical Architecture
* **Language:** C++
* **Core Concepts Demonstrated:** Object-Oriented Programming (Classes, Access Modifiers, Encapsulation), Standard Template Library (STL) strings, and robust error handling (`try/catch`).
* **Data Persistence:** Utilizes C++ File I/O streams (`<fstream>`) to generate, read, and append persistent text files, ensuring long-term data integrity for master student lists and individual subject attendance logs.

## System Features
* **Admin Module:** Secured login enabling administrators to add new students, initialize records, and mark daily attendance.
* **Student Module:** Read-only access allowing students to view their overall attendance percentages and subject-specific logs.
* **Dynamic Time-Stamping:** Automatically fetches the host machine's local date/time via the `<ctime>` library for accurate attendance logging.
* **Input Validation:** Built-in buffer clearing and type-checking to prevent infinite loops and crashes from invalid user inputs.

## Execution
To compile and run this application locally, you can use any standard C++ compiler (such as GCC) via your terminal or an integrated terminal in VS Code:

```bash
# Compile the source code
g++ main.cpp -o attendance_system

# Execute the compiled program
./attendance_system
