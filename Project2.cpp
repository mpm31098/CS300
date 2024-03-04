# include <iostream>
# include <iomanip>
# include <string>
# include <vector>
# include <sstream>
# include <fstream>

using namespace std;

///--------------
/// Course Struct
///--------------
struct Course {
    string courseNum;
    string courseName;

    vector<string> PreReqs;

    //empty constructor
    Course() {};
    //constructor with 2 values
    Course(string aCourseNum, string aCourseName) {
        courseNum = aCourseNum;
        courseName = aCourseName;
    }
    //constructor with prereqs
    Course(string aCourseNum, string aCourseName, vector<string> prereqs) {
        courseNum = aCourseNum;
        courseName = aCourseName;
        vector<string> PreReqs = prereqs;
    }
};

///------------
/// Node Struct
///------------
struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course acourse) : Node() {
        this->course = acourse;
    }
};

///-----------
/// BST Class
///-----------
class BST {
private:
    Node* root;
    void addNode(Course acourse, Node* node);
    void inOrder(Node* node);
    void printCourse(Node* node, string courseNum);

public:
    BST();
    void InOrder();
    void PrintCourse(string courseNum);
    bool loadData();
    bool validatePreReqs(Course course);
    void insert(Course course);
};

//default constructor for bst
BST::BST() {
    root = nullptr;
}

//public in order (passes root to private in order)
void BST::InOrder() {
    this->inOrder(root);
}

//passes root node to print course
void BST::PrintCourse(string courseNum) {
    this->printCourse(root, courseNum);
}

///------------------
/// Insert a New Node
///------------------
void BST::insert(Course course) {
    //if root is empty
    if (root == nullptr) {
        root = new Node(course);
    }
    //tree has been started, call addnode, pass root
    else {
        this->addNode(course, root);
    }
}

///-----------------
/// Add Node to Tree
///-----------------
void BST::addNode(Course acourse, Node* node) {

    //if current node is larger than passed courseNum
    if (node->course.courseNum.compare(acourse.courseNum) > 0) {
        //if there is no left node
        if (node->left == nullptr) {
            node->left = new Node(acourse);
        }
        //recurse down left side
        else {
            this->addNode(acourse, node->left);
        }
    }
    //if current node is smaller than passed courseNum
    else {
        //if there is no right node
        if (node->right == nullptr) {
            node->right = new Node(acourse);
        }
        //recurse down right side
        else {
            this->addNode(acourse, node->right);
        }
    }
}

///--------------------------------
/// Print Courses In Order: private
///--------------------------------
void BST::inOrder(Node* node) {

    if (node != nullptr) {
        //recurse down left side
        inOrder(node->left);
        //print course information at current node
        cout << "Course Number: " << node->course.courseNum;
        cout << " Course Name: " << node->course.courseName;
        cout << " Prerequisite(s): ";
        //iterate through course's prereq vector to print
        for (int i = 0; i < node->course.PreReqs.size(); ++i) {
            cout << node->course.PreReqs[i] << " ";
        }
        if (node->course.PreReqs.size() == 0) {
            cout << "None";
        }
        cout << "\n";

        //recurse down right side
        inOrder(node->right);
    }
    return;
}

///---------------------------------
/// Print Individual Course: private
///---------------------------------
void BST::printCourse(Node* node, string courseNum) {

    while (node != nullptr) {
        //if match is found
        if (node->course.courseNum.compare(courseNum) == 0) {
            cout << "Course Number: " << node->course.courseNum;
            cout << "  Course Name: " << node->course.courseName;
            cout << "  Prerequisite(s): ";
            //iterate through prereq vector to print
            for (int i = 0; i < node->course.PreReqs.size(); ++i) {
                cout << node->course.PreReqs[i] << " ";
            }
            //if there are no prereqs
            if (node->course.PreReqs.size() == 0) {
                cout << "None";
            }
            cout << "\n";
            return;
        }
        //if current course number is smaller than the one passed in
        else if (node->course.courseNum.compare(courseNum) > 0 && node->left != nullptr) {
            node = node->left;
        }
        //if current course number is larger than the one passed in
        else if (node->course.courseNum.compare(courseNum) < 0 && node->right != nullptr) {
            node = node->right;
        }
        else {
            cout << "Course not found. Please enter course number \n";
            return;
        }
    }
    //error if tree is not loaded
    if (root == nullptr) {
        cout << "Error loading data. \n";
        return;
    }
}


///------------------------------------------------------
///Load Data (from file into bst)
///------------------------------------------------------
bool BST::loadData() {

    //use ifstream to open file
    ifstream file;
    file.open("courses.csv");

    if (file.is_open()) {
        //while end of file is not reached
        while (!file.eof()) {
            //create a vector to hold line items
            vector<string> courseLine;
            //to parse line into vector
            string line;

            //get line from file and assign to 'line'
            getline(file, line);
            //while line is not empty
            while (line.length() > 0) {
                unsigned int delim = line.find(',');
                //limits data size to 99 or less characters
                if (delim < 100) {
                    //add item to courseLine, then delete
                    courseLine.push_back(line.substr(0, delim));
                    line.erase(0, delim + 1);
                }
                //add item following last comma, then clear line
                else {
                    courseLine.push_back(line.substr(0, line.length()));
                    line = "";
                }
            }

            //local course object (to be filled and passed to insert)
            Course course;
            //assign course number and name

            course.courseNum = courseLine[0];
            course.courseName = courseLine[1];
            //push items 3+ into prereq vector
            for (unsigned int i = 2; i < courseLine.size(); i++) {
                course.PreReqs.push_back(courseLine[i]);
            }
            //insert the course into bst
            insert(course);
        }
        return true;
    }
    if (!file) {
        cout << "File could not load \n";
        return false;
    }
    file.close();
}

bool BST::validatePreReqs(Course course) {
    //create local node and set to root
    Node* current = root;
    if (current->course.PreReqs.size() > 0) {
        //iterate through prerequisite vector
        for (int i = 0; i < course.PreReqs.size(); i++) {
            while (current != nullptr) {
                //if match is found
                if (course.PreReqs[i] == current->course.courseNum) {
                    return true;
                }
                //if course number is smaller than prerequisite
                else if (course.PreReqs[i].compare(current->course.courseNum) > 0 && current->left != nullptr) {
                    current = current->left;
                }
                //if course number is larger than prerequisite
                else if (course.PreReqs[i].compare(current->course.courseNum) < 0 && current->right != nullptr) {
                    current = current->right;
                }
                //if corresponding course number is not found
                else {
                    cout << "Prerequisite not found. \n";
                    return false;
                }
            }
        }
    }
    else {
        //no prerequisites
        return true;
    }
}

int main()
{
    //create bst 
    BST* bst = new BST();
    //create a string variable to hold input (for case 3)
    string courseNum;
    //set choice to 0
    int choice = 0;

    cout << "Welcome to the course planner! \n\n";
    //Menu loop that will iterate until user enters 4
    while (choice != 4) {
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        //menu selections
        switch (choice) {
            //load data from file
        case 1:
            bst->loadData();
            break;
            //print sorted course information
        case 2:
            cout << "\nSample schedule: \n";
            cout << "---------------------------- \n";
            bst->InOrder();
            cout << "\n";
            break;
            //search for a specific course to display
        case 3:
            cout << "Please enter the course number you would like to search: ";
            cin >> courseNum;
            bst->PrintCourse(courseNum);
            cout << "\n";
            break;
            //say goodbye, the user is leaving now
        case 4:
            cout << "\nThank you for using the course planner!\n";
            break;
            //for invalid input
        default:
            cout << choice << " is not a valid option. Please enter a valid option.\n";
        }
    }
    return 0;
}