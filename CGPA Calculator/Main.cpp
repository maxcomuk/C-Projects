// Including necessary libraries //
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>

/* Resetting input stream error flags and discards all leftover characters
  in the current line to ensure a clean stream for later input */
void discardLine() {
	if (std::cin.fail())
		std::cin.clear();

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Creating a data store struct so that we can store each individual grade, credit hours and course name //
struct coursesData {
	std::string courseName;
	int grade;
	int creditHours;
};

// Getting the amount of courses from the userinput if input is invalid we loop until the input is a valid number //
void getCourseAmount(int& totalCourses) {
	while (true) {
		std::cout << "Enter the amount of courses taken: ";
		if (std::cin >> totalCourses)
			break;
		else
			discardLine();
	}
}

/* Creating a loop that will get data on the specified course then setting its valu to the userinput.
   we also create a integer named type so we can reuse this function for both credit hours and grade */
void setCourseStats(coursesData& course, int type) {
	while (true) {
		if (type == 1) {
			std::cout << "Credit Hours: ";
			if (std::cin >> course.creditHours)
				break;
			else
				discardLine();
			continue;
		}
		else if (type == 2) {
			std::cout << "Grade (0 - 4): ";
			if (std::cin >> course.grade)
				break;
			else
				discardLine();
			continue;
		}
		else {
			// If for whatever reason we fail, we leave a comment and exit the loop doing nothing //
			std::cout << "Failed to set course stats" << '\n';
			break;
		}
	}
}

// Looping through the struct_list array that we have resserved and getting user input on each subject (eg course name, credit hours, grade) //
void getCourseData(std::vector <coursesData>& struct_list, int& totalCourses) {
	// This is not required however i use it just incase the input/output stream has leftover whitespaces //
	discardLine();

	for (int i = 0; i < totalCourses; i++) {
		coursesData user;	

		std::cout << "\nEnter course name: ";
		std::getline(std::cin, user.courseName);

		// Providing the function our coursesData user and the type that it will set (1 = credit hours, 2 = grade) //
		setCourseStats(user, 1);
		setCourseStats(user, 2);

		// Clearing the input/output stream once more to prepare for the next iretiration if any //
		discardLine();

		// Adding the struct we have just created to the struct_list //
	 
		/* Cool little optimization trick i learnt is that if you dont use std::move(user) it will copy
		   the struct to add it to the struct_list therefore leading to a peformance issue, thanks to std::move
		   we can add the exact copy to struct_list we created without creating a new copy*/
		struct_list.push_back(std::move(user));
	}
}

// Calculating the users GPA using the formula //
// ///////////////////////
// grade * credit hours //
// -------------------- //
//     credit hours     //
//////////////////////////
double calculateGPA(const std::vector <coursesData>& struct_list) {
	double totalGradePoints = 0.0;
	int totalCreditHours = 0;

	for (const coursesData& course : struct_list) {
		totalGradePoints += course.grade * course.creditHours;
		totalCreditHours += course.creditHours;
	}

	if (totalCreditHours == 0)
		return 0.0;
	else
		return totalGradePoints / totalCreditHours;
}

// Main entry where all the logic is handled //
int main()
{
	// Creating a integer that will track the amount of courses the user has enterd //
	int totalCourses;

	// Prompting user to press enter to start //
	std::cout << "CGPA Calculater. Press enter to start" << '\n';
	std::cin.get();

	// Sending our original copy of the total courses then setting it to the users input inside this function //
	getCourseAmount(totalCourses);

	// Creating a struct array //
	std::vector <coursesData> stuct_list;

	// Using reserve here so that the array creates the exact copy of bytes it needs for all the courses //
	stuct_list.reserve(totalCourses);

	// Sending our struct list and total courses amount to this function to set all the values depending on the users input //
	getCourseData(stuct_list, totalCourses);


	// Calculating the GPA then storing it in a double so we can prompt the user with his GPA result //
	double result = calculateGPA(stuct_list);

	// Using std::fixed and std::setprecision(2) so that we give an exact 0.00 gpa result to the user //
	std::cout << "Your total is: " << std::fixed << std::setprecision(2) << result << " GPA" << '\n';

	// This is optional //
	return 0;
}
