// 300projectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// define a struct for a course
struct Course
{
    // data member
    string courseNumber;
    string courseTitle;
    string prerequisites;
    Course* left;
    Course* right;

    // constructor
    Course(const string& number, const string& title, const string& prereqs)
        : courseNumber(number), courseTitle(title), prerequisites(prereqs), left(nullptr), right(nullptr) {}
};

// define a class for a course planner
class CoursePlanner
{
    // data member
private:
    // root of the binary search tree
    Course* root;

public:
    // default constructor
    CoursePlanner() : root(nullptr) {}

    // Function to insert a course into the binary search tree
    void insertCourse(const string& number, const string& title, const string& prereqs)
    {
        // create a new course
        Course* newCourse = new Course(number, title, prereqs);

        // insert the new course into the binary search tree
        if (root == nullptr)
        {
            // set the root of the binary search tree
            root = newCourse;
        }
        else
        {
            // otherwise, recursively insert the new course into the binary search tree
            insertCourseHelper(root, newCourse);
        }
    }

    // Helper function to recursively insert a course into the binary search tree
    void insertCourseHelper(Course* current, Course* newCourse)
    {
        // insert the new course into the binary search tree
        if (newCourse->courseNumber < current->courseNumber)
        {
            // insert the new course into the left subtree
            if (current->left == nullptr)
            {
                // set the left child of the current node
                current->left = newCourse;
            }
            else
            {
                // otherwise, recursively insert the new course into the left subtree
                insertCourseHelper(current->left, newCourse);
            }
        }
        else
        {
            // insert the new course into the right subtree
            if (current->right == nullptr)
            {
                // set the right child of the current node
                current->right = newCourse;
            }
            else
            {
                // otherwise, recursively insert the new course into the right subtree
                insertCourseHelper(current->right, newCourse);
            }
        }
    }

    // Function to load course data from a file into the binary search tree
    void loadCourseData(const string& filename)
    {
        // open the file
        ifstream inputFile(filename);

        // check if the file is opened successfully
        if (!inputFile)
        {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        // declare variables to store the course information
        string line;

        // read the file line by line
        while (getline(inputFile, line))
        {
            // parse the line to get the course information
            string courseNumber, courseTitle, prerequisites;

            // find the positions of the commas
            size_t commaPos1 = line.find(',');
            size_t commaPos2 = line.find(',', commaPos1 + 1);

            // check if both commas are found
            if (commaPos1 != string::npos && commaPos2 != string::npos)
            {
                // parse the line to get the course information
                courseNumber = line.substr(0, commaPos1);

                // remove the leading and trailing spaces
                courseTitle = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                prerequisites = line.substr(commaPos2 + 1);
            }
            // if only one comma is found
            else if (commaPos1 != string::npos)
            {
                // parse the line to get the course information
                courseNumber = line.substr(0, commaPos1);

                // remove the leading and trailing spaces
                courseTitle = line.substr(commaPos1 + 1);
                prerequisites = "No prerequisite";
            }
            else
            {
                // otherwise, print an error message
                cout << "Invalid line in file: " << line << endl;
                continue; // Skip to the next line
            }

            // insert the course into the binary search tree
            insertCourse(courseNumber, courseTitle, prerequisites);
        }

        // close the file
        inputFile.close();
    }

    // Function to print the course list in alphanumeric order
    void printCourseList()
    {
        // check if the course list is empty
        cout << "Here is the course list:" << endl;
        printCourseListHelper(root); // print the course list in alphanumeric order
        cout << endl;
    }

    // Helper function to recursively print the course list in alphanumeric order
    void printCourseListHelper(Course* current)
    {
        // print the course list in alphanumeric order if the current node is not null
        if (current != nullptr)
        {
            // recursively print the left subtree
            printCourseListHelper(current->left);
            cout << current->courseNumber << ", " << current->courseTitle << endl;
            // recursively print the right subtree
            printCourseListHelper(current->right);
        }
    }

    // Function to print the course information for a given course number
    void printCourse(const string& courseNumber)
    {
        // find the course in the binary search tree
        Course* course = findCourse(root, courseNumber);

        // if the course is found, print the course information
        if (course != nullptr)
        {
            cout << course->courseNumber << ", " << course->courseTitle << endl;
            cout << "Prerequisites: " << course->prerequisites << endl;
        }
        // otherwise, print an error message
        else
        {
            cout << "Course not found." << endl;
        }
    }

    // Function to find a course in the binary search tree
    Course* findCourse(Course* current, const string& courseNumber)
    {
        // find the course in the binary search tree
        if (current == nullptr || current->courseNumber == courseNumber)
        {
            return current;
        }

        // recursively find the course in the left or right subtree
        if (courseNumber < current->courseNumber)
        {
            return findCourse(current->left, courseNumber);
        }
        else
        {
            // otherwise, recursively find the course in the right subtree
            return findCourse(current->right, courseNumber);
        }
    }

    // Function to display the menu and handle user input
    void displayMenu()
    {
        // display the menu
        cout << "Welcome to the course planner." << endl;

        // handle user input
        int choice = -1;

        // loop until the user chooses to exit
        while (choice != 9)
        {
            cout << endl;
            cout << "1. Load Data Structure." << endl;
            cout << "2. Print Course List." << endl;
            cout << "3. Print Course." << endl;
            cout << "9. Exit" << endl;
            cout << endl;
            cout << "What would you like to do? ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;

            // apply the swithc case user's choice
            switch (choice)
            {
            case 1:
            {
                // load course data from a file
                string filename;
                cout << "Enter the file name: ";
                getline(cin, filename);
                loadCourseData(filename);
                break;
            }
            case 2:
                // print the course list
                printCourseList();
                break;
            case 3:
            {
                // declare a variable to store the course number
                string courseNumber;
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber); // get the course number from the user
                printCourse(courseNumber); // find and print the course information
                break;
            }
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
            }
        }
    }
};

// main function
int main()
{
    // create a course planner
    CoursePlanner coursePlanner;
    // display the menu
    coursePlanner.displayMenu();
    return 0;
}