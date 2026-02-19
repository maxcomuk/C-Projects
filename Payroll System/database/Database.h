#pragma once

#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "sqlite3.h"
#include "../Utilities.h"
#include "../models/PayPeriod.h"
#include "../models/Payroll.h"
#include "../models/EmployeePayroll.h"
#include "../models/Employee.h"
#include "../models/TimeEntry.h"

class Database
{
private:
	sqlite3* db;
public:
	Database();
	~Database();

	bool openDatabase(const std::string& fileName);
	bool SetupTables();
	
	bool addPayPeriod(const std::string& startDate, const std::string& endDate);
	bool removePayPeriod(const int& payPeriodID);
	bool insertPayrollForPayPeriod(const int& payPeriodID);
	bool addTimeEntry(const TimeEntry& timeEntry);
	bool setEmployeeStatus(const int& employeeID, const bool& isActive);
	bool addEmployee(const Employee& emp);

	std::vector<PayPeriod> getPayPeriods();
	std::vector<Payroll> getPayrollRecords();
	std::vector<EmployeePayroll> getEmployeePayrollHistory(const int& employeeID);
	std::vector<TimeEntry> getTimeEntries();
	std::vector<Employee> getEmployees();
};
