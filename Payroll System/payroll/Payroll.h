#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctype.h>

#include "../Utilities.h"
#include "../models/Employee.h"
#include "../models/Payroll.h"
#include "../models/EmployeePayroll.h"
#include "../models/TimeEntry.h"

#include "../database/Database.h"
#include "../registration/Registration.h"

class Database;

class PayrollManager
{
public:
	bool setupPayroll(Database& db, AuthManager& auth);
	void createPayPeriod(Database& db);
	void deletePayPeriod(Database& db);
	void viewPayPeriods(Database& db, bool waitForUserInput);
	void processPayroll(Database& db);
	void viewPayrollRecords(Database& db);
	void viewEmployeePayrollHistory(Database& db);
	void viewTimeEntries(Database& db, bool waitForUserInput);
	void enterEmployeeHours(Database& db);
	void toggleEmployeeStatus(Database& db);
	void createEmployee(Database& db);
	void viewEmployees(Database& db, bool waitForUserInput);
};
