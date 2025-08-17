// CoursePlanner.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

// Define a struct for a course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
    Course* left;
    Course* right;

    Course(const string& number, const string& title, const vector<string>& prereqs)
        : courseNumber(number), courseTitle(title), prerequisites(prereqs), left(nullptr), right(nullptr) {}
};

// Define a class for the course planner
class CoursePlanner {
private:
    Course* root;

public:
    CoursePlanner() : root(nullptr) {}

    ~CoursePlanner() {
        clearTree(root);
    }

    // Destructor helper to avoid memory leaks
    void clearTree(Course* node) {
        if (node) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    void insertCourse(const string& number, const string& title, const vector<string>& prereqs) {
        Course* newCourse = new Course(number, title, prereqs);
        if (root == nullptr) {
            root = newCourse;
        } else {
            insertCourseHelper(root, newCourse);
        }
    }

    void insertCourseHelper(Course* current, Course* newCourse) {
        if (newCourse->courseNumber < current->courseNumber) {
            if (current->left == nullptr) {
                current->left = newCourse;
            } else {
                insertCourseHelper(current->left, newCourse);
            }
        } else {
            if (current->right == nullptr) {
                current->right = newCourse;
            } else {
                insertCourseHelper(current->right, newCourse);
            }
        }
    }

    void loadCourseData(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string item;
            vector<string> tokens;

            while (getline(ss, item, ',')) {
                item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end()); // Trim whitespace
                tokens.push_back(item);
            }

            if (tokens.size() >= 2) {
                string courseNumber = tokens[0];
                string courseTitle = tokens[1];
                vector<string> prereqs(tokens.begin() + 2, tokens.end());
                insertCourse(courseNumber, courseTitle, prereqs);
            } else {
                cout << "Invalid line: " << line << endl;
            }
        }

        inputFile.close();
        cout << "Course data successfully loaded from " << filename << "." << endl;
    }

    void printCourseList() {
        cout << "Here is the course list:\n";
        printCourseListHelper(root);
        cout << endl;
    }

    void printCourseListHelper(Course* current) {
        if (current != nullptr) {
            printCourseListHelper(current->left);
            cout << current->courseNumber << ", " << current->courseTitle << endl;
            printCourseListHelper(current->right);
        }
    }

    void printCourse(const string& courseNumber) {
        Course* course = findCourse(root, courseNumber);
        if (course != nullptr) {
            cout << course->courseNumber << ", " << course->courseTitle << endl;
            cout << "Prerequisites:\n";
            if (course->prerequisites.empty()) {
                cout << "  None" << endl;
            } else {
                for (const string& prereq : course->prerequisites) {
                    printCourseChain(prereq, 1); // depth = 1
                }
            }
        } else {
            cout << "Course not found.\n";
        }
    }

    void printCourseChain(const string& courseNumber, int depth) {
        Course* course = findCourse(root, courseNumber);
        if (course != nullptr) {
            cout << string(depth * 2, ' ') << "- " << course->courseNumber << ": " << course->courseTitle << endl;
            for (const string& prereq : course->prerequisites) {
                printCourseChain(prereq, depth + 1);
            }
        } else {
            cout << string(depth * 2, ' ') << "- " << courseNumber << " (not found)" << endl;
        }
    }

    Course* findCourse(Course* current, const string& courseNumber) {
        if (current == nullptr || current->courseNumber == courseNumber) {
            return current;
        }

        if (courseNumber < current->courseNumber) {
            return findCourse(current->left, courseNumber);
        } else {
            return findCourse(current->right, courseNumber);
        }
    }

    void displayMenu() {
        cout << "Welcome to the course planner.\n";

        int choice = -1;
        while (choice != 9) {
            cout << "\n1. Load Data Structure\n";
            cout << "2. Print Course List\n";
            cout << "3. Print Course\n";
            cout << "9. Exit\n";
            cout << "What would you like to do? ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush input buffer

            switch (choice) {
            case 1: {
                string filename;
                cout << "Enter the file name: ";
                getline(cin, filename);
                loadCourseData(filename);
                break;
            }
            case 2:
                printCourseList();
                break;
            case 3: {
                string courseNumber;
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);
                printCourse(courseNumber);
                break;
            }
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option.\n";
            }
        }
    }
};

// Main function
int main() {
    CoursePlanner planner;
    planner.displayMenu();
    return 0;
}
