// Header guard
#pragma once

// Including necessary libraries
#include <iostream>
#include <limits>
#include <string>
#include "sqlite3.h"

// Clears the input stream and any leftover whitespace
void ClearInputBuffer()
{
	if (std::cin.fail())
		std::cin.clear();

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Initializes the database and creates the USERS, STUDENTS, and COURSES tables if they do not already exist
void setupDatabase(sqlite3*& db)
{
	// Attempting to open database (if it doesnt exist it will automatically create one)
	int response = sqlite3_open("Studen_Database_Manager.db", &db);
	if (response != SQLITE_OK)
	{
		std::cerr << "\nFailed to open the database. Reason: " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
		exit(response);
	}

	// SQL script that will create a table named users if the table doesnt arleady exist (this will be used to store our users data such as username and passwords)
	const char* usersSQL = "CREATE TABLE IF NOT EXISTS USERS(ID INTEGER PRIMARY KEY AUTOINCREMENT, USERNAME TEXT NOT NULL UNIQUE, PASSWORD TEXT NOT NULL);";
	char* errMsg = nullptr;
	response = sqlite3_exec(db, usersSQL, nullptr, nullptr, &errMsg);
	if (response != SQLITE_OK)
	{
		std::cerr << "USERS SQL ERROR: " << errMsg << '\n';
		sqlite3_free(errMsg);
		sqlite3_close(db);
		exit(1);
	}

	// SQL script that will create a table named students if the table doesnt arleady exist (this will be used to store data for the students)
	const char* studentsSQL = "CREATE TABLE IF NOT EXISTS STUDENTS (ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT NOT NULL UNIQUE, AGE INTEGER, PLACE_OF_BIRTH TEXT, RELIGION TEXT);";
	response = sqlite3_exec(db, studentsSQL, nullptr, nullptr, &errMsg);
	if (response != SQLITE_OK)
	{
		std::cerr << "STUDENTS SQL ERROR: " << errMsg << '\n';
		sqlite3_free(errMsg);
		sqlite3_close(db);
		exit(1);
	}

	// SQL script that will create a table named courses if the table doesnt arleady exist (this will be used to store out data for all the courses)
	const char* coursesSQL = "CREATE TABLE IF NOT EXISTS COURSES (ID INTEGER PRIMARY KEY AUTOINCREMENT, COURSE TEXT NOT NULL UNIQUE, PASSGRADE INTEGER);";
	response = sqlite3_exec(db, coursesSQL, nullptr, nullptr, &errMsg);
	if (response != SQLITE_OK)
	{
		std::cerr << "COURSES SQL ERROR: " << errMsg << '\n';
		sqlite3_free(errMsg);
		sqlite3_close(db);
		exit(1);
	}
}

// Showing the menu navigation
void showMenu()
{
	std::cout << "\n1 = Login\n";
	std::cout << "2 = Register\n";
	std::cout << "3 = Exit\n";
	std::cout << "Input: ";
}

// Showing the database navigation
void showDatabaseMenu()
{
	system("cls");
	std::cout << "1 = Search Student\n";
	std::cout << "2 = Add Student\n";
	std::cout << "3 = Remove Student\n";
	std::cout << "4 = Display Students\n";
	std::cout << "5 = Display Courses\n";
	std::cout << "6 = Add Course\n";
	std::cout << "7 = Remove Course\n";
	std::cout << "8 = Search Course\n";
	std::cout << "9 = Log Out\n";
	std::cout << "10 = Exit\n";
	std::cout << "\nInput: ";
}
