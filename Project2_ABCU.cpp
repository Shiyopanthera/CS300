//============================================================================
// Name        : Project2_ABCU.cpp
// Author      : Dani Allen
// Version     : 1.0
// Description : Project Two - Code for the Computer Science department at ABCU
//============================================================================

#include <algorithm>
#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream> 

using namespace std;



//============================================================================
// Loading Data Structure: Develop working code to load data from the file into the data structure.
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

//Structure to hold course objects
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};


class HashTable {

private:
	// Define structures to hold courses
	struct Node {
		Course course;
		unsigned int key;
		Node* next;

		// default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		// initialize with a bid
		Node(Course aCourse) : Node() {
			course = aCourse;
		}

		// initialize with a bid and a key
		Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned int tableSize = DEFAULT_SIZE;

	unsigned int hash(string key);

public:
	HashTable();
	void insert(Course course);
	void printAlphanumericList();
	Course Search(string courseNumber);
};


//Default constructor
HashTable::HashTable() {
	nodes.resize(tableSize);
}

//Calculate hash value for CourseNumber key
unsigned int HashTable::hash(string key) {
	unsigned int hash = 0;
	for (char c : key) {
		hash = (hash * 31) + c; //ASCII value
	}

	return hash % tableSize;
}

//Insert Course
void HashTable::insert(Course course) {
	unsigned int key = hash(course.courseNumber);
	Node* newNode = new Node(course, key);
	if (!nodes[key].course.courseNumber.empty()) {
		Node* current = &nodes[key];
		while (current->next) {
			current = current->next;
		}

		current->next = newNode;
	}
	else {
		nodes[key] = *newNode;
	}
}



//============================================================================
// Input: Design code to correctly read the course data file
//============================================================================


void loadCourses(HashTable& courses) {
	//Open file for reading
	ifstream file("CourseInformation.txt");

	//File not found
	if (!file) {
		cout << "File not found." << endl;
		return; 
	}

	//Read lines and add course object to Hash Table
	string line;
	while (getline(file, line)) { 

		//Skip empty lines
		if (line.empty()) {
			cout << "Format error: Empty line found." << endl;
			continue;
		}

		//Initialize variable for course data
		string courseNumber;
		string courseName;
		vector<string> prerequisites; 

		//Split the line by commas
		stringstream ss(line); 
		getline(ss, courseNumber, ','); 
		getline(ss, courseName, ','); 

		//Check if courseNumber and courseName are not empty
		if (courseNumber.empty() || courseName.empty()) {
			cout << "Format error: Course number and course name are required." << endl;
			return;
		}

		//Parse and add prerequisites
		string prerequisite;
		while (getline(ss, prerequisite, ',')) {
			if (!prerequisite.empty()) {
				prerequisites.push_back(prerequisite);
			}
		}

		//Create a Course object and insert it into the HashTable
		Course course; 
		course.courseNumber = courseNumber; 
		course.courseName = courseName;
		course.prerequisites = prerequisites;

		courses.insert(course); 
	}

	cout << "Data loaded successfully." << endl;
}




//============================================================================
// Course List: Develop working code to sort and print out a list of the courses in the Computer Science program in alphanumeric order
//============================================================================

void HashTable::printAlphanumericList() {
	vector<Course> sortedCourses;

	// Collect all courses into a vector for sorting
	for (unsigned int key = 0; key < tableSize; key++) {
		Node* node = &nodes[key];
		while (node) {
			if (!node->course.courseNumber.empty()) {
				sortedCourses.push_back(node->course);
			}
			node = node->next;
		}
	}

	//Sort courses by courseNumber
	sort(sortedCourses.begin(), sortedCourses.end(), [](Course& a, Course& b) {
		return a.courseNumber < b.courseNumber;
		});
	

	//Print the sorted list
	for (Course& course : sortedCourses) {
		cout << course.courseNumber << " | " << course.courseName << endl; 
	}

}


//============================================================================
// Course Information: Develop working code to print course information. 
//============================================================================

Course HashTable::Search(string courseNumber) {
	unsigned int key = hash(courseNumber);
	Node* node = &nodes[key];
	while (node) {
		if (node->course.courseNumber == courseNumber) {
			//Course found
			cout << "Course Number: " << node->course.courseNumber << endl;
			cout << "Course Name: " << node->course.courseName << endl;
			cout << "Prerequisites: " << endl;
			for (string& prerequisite : node->course.prerequisites) {
				cout << prerequisite << endl; 
			}
			return node->course;
		}
		node = node->next;
	}

	//Course not found
	cout << "Course not found." << endl; 
	Course notFound; 
	return notFound; 
}



int main()
{
	HashTable courses;
	

	//============================================================================
	// Menu: Design code to create a menu that prompts a user for menu options
	//============================================================================
	int choice = 0;
	while (choice != 4) {
		cout << "Menu: " << endl;
		cout << "1. Load Data" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course and Prerequisites" << endl;
		cout << "4. Exit" << endl;
		cin >> choice;

		switch (choice) {
		case 1: {
			// Load Data Structure
			loadCourses(courses); 
			break;
		}

		case 2: {
			//Print Course List
			courses.printAlphanumericList();
			break;
		}

		case 3: {
			//Search for Course and Print
			string courseNumber;
			cout << "Enter course number: ";
			cin >> courseNumber;
			Course course = courses.Search(courseNumber);
			break;
		}

		case 4: {
			//Exit
			cout << "Goodbye." << endl;
			break;
		}

		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}

}
