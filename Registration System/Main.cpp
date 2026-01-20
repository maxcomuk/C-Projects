// Including libraries //
#include <iostream>
#include <string>
#include <limits>
#include "sqlite3.h"

// Clearing the input stream and any errors if std::cin failed //
void clearInputStream()
{
	if (std::cin.fail())
		std::cin.clear();

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// running a infinite while loop aswell as deciding what prompt to give the user depending if he is in the menu navigation or just started the program //
void registrationType(int& decision, bool menu_navigation)
{
	while (true)
	{
		if (menu_navigation)
		{
			std::cout << "1 = Login Into Another Account || 2 = Register Another Account || 3 = Get Account Details\n";

			if (std::cin >> decision && (decision == 1 || decision == 2 || decision == 3))
				break;
		}
		else
		{
			std::cout << "\n1 = Login Account || 2 = Register An Account\n";

			if (std::cin >> decision && (decision == 1 || decision == 2))
				break;
		}
		
		std::cout << "\nInvalid Input\n";
		clearInputStream();
	}
}

// Creating class that will contain the methods and data for the user //
class UserData
{
private:
	// We make these private so it cant be messed with or changed outside this class //
	std::string username;
	std::string password;
public:
	// Registering the user to the database that we take as a parameter //
	void registerAccount(sqlite3* DataBase)
	{
		sqlite3_stmt* stmt; // A handle that represents the SQL command we are preparing to run

		// Entering infinite while loop until the user has registerd an account //
		while (true)
		{
			std::cout << "Username: ";
			std::getline(std::cin, username);

			std::cout << "Password: ";
			std::getline(std::cin, password);

			// Running sql command to check if the accounts username arleady exists in the database //
			std::string checkSQL = "SELECT count(*) FROM USERS WHERE USERNAME = '" + username + "';";
			sqlite3_prepare_v2(DataBase, checkSQL.c_str(), -1, &stmt, NULL);
			sqlite3_step(stmt);

			// If the account arleady exists we get the return value of 1 otherwise it will return 0 //
			int count = sqlite3_column_int(stmt, 0);
			sqlite3_finalize(stmt);
			if (count > 0)
			{
				std::cout << "\nInvalid Username: Account arleady exists\n";
				continue;
			}

			// Running sql command that will create our account within the database //
			std::string saveSQL = "INSERT INTO USERS (USERNAME, PASSWORD) VALUES ('" + username + "', '" + password + "');";
			char* errorMsg;
			int saveResponse = sqlite3_exec(DataBase, saveSQL.c_str(), NULL, 0, &errorMsg);
			if (saveResponse != SQLITE_OK)
			{
				std::cerr << errorMsg; // Using std::cerr incase we get any errors while creating the account then using errorMsg to get the error log
				sqlite3_free(errorMsg); // Important we free memory up here
			}
			else
			{
				// Account sucessfull means we break out the loop and finish registering account //
				std::cout << "\nUser registerd successfully\n";
				break;
			}
		}
	}

	// Validating if the users account exists and that the details he has enterd are valid //
	void accountLogin(sqlite3* DataBase)
	{
		sqlite3_stmt* stmt; // A handle that represents the SQL command we are preparing to run

		// Entering infinite while loop until the user has logged in sucessfully //
		while (true)
		{
			std::cout << "Username: ";
			std::getline(std::cin, username);

			std::cout << "Password: ";
			std::getline(std::cin, password);

			// Running sql command that will check if the username exists and that the password is valid. Note we use ? to protect against basic sql attacks //
			const char* loginSQL = "SELECT count(*) FROM USERS WHERE USERNAME = ? AND PASSWORD = ?;";
			sqlite3_prepare_v2(DataBase, loginSQL, -1, &stmt, NULL);
			sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
			sqlite3_step(stmt);

			// If the account arleady exists we get the return value of 1 otherwise it will return 0 //
			int count = sqlite3_column_int(stmt, 0);
			sqlite3_finalize(stmt);
			if (count == 0)
			{
				std::cout << "\nInvalid Credentials Try Again!\n";
			}
			else
			{
				std::cout << "\nUser Logged In Sucessfully\n";
				break;
			}
		}
	}

	// Printing out the current logged in users account details //
	void getAccountDetails()
	{
		std::cout << "Username: " << username << '\n';
		std::cout << "Password: " << password << '\n';
		std::cout << "\nPress Enter To Continue\n";
		std::cin.get();
	}
};

// Program main entry point //
int main()
{
	// Opening the sql database file, if it doesnt find it then it will automatically create a new one with our specified name below //
	sqlite3* DataBase;
	int response = sqlite3_open("Registration-Database", &DataBase);
	if (response != SQLITE_OK)
	{
		std::cout << "Database Failure!\n";
		return 1;
	}

	// Running sql command that will define the table structure and create it if it doesn't already exist //
	const char* setupSQL = "CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT, USERNAME TEXT NOT NULL, PASSWORD TEXT NOT NULL);";
	char* sqlError;
	sqlite3_exec(DataBase, setupSQL, NULL, 0, &sqlError);

	UserData user; // Instantiating the active user session
	int decision; // Creating int named decision that will store the user's menu selection (1, 2, or 3)

	// Optional user greeting when starting the program //
	std::cout << "**** Welcome to the registration system ****\nPress enter to begin . . .\n";
	std::cin.get();

	/* Calling registrationType function and giving it the decision int that will be changed within this function, we also give it false
	   as this is not the menu navigation but the start of the program and the user cannot grab his details if he hasn't logged in yet */
	registrationType(decision, false);
	clearInputStream(); // Clearing Input Stream

	// Entering program infinite loop that will keep the program alive until the user closes it //
	while (true)
	{
		// Peforming conditional if statements to determain wether the user wants to login, register or grab his account details //
		if (decision == 1)
			user.accountLogin(DataBase);
		else if (decision == 2)
			user.registerAccount(DataBase);
		else
			user.getAccountDetails();

		// Optional but recommended to clear the console so its not populated with so many characters //
		system("cls");

		// Prompting user that he is in the navigation settings now and fully logged in //
		std::cout << "\t\t\t\t**** Menu Navigation Settings ****\n";
		registrationType(decision, true); // again calling the registrationType and giving it true this time as the user is in the navigation menu
		clearInputStream();

		// Loop will ireterate again with the new decision from the registrationType above on line 185 //
	}

	// Closing the database connection to release system resources and prevent file locking issues for future sessions //
	sqlite3_close(DataBase);

	// Optional but I like to use to specify end of program //
	return 0;
}
