#include <iostream>
#include "Payroll.h"

bool PayrollManager::setupPayroll(Database& db, AuthManager& auth) {
	bool running{ true };
	int decision;

	while (running) {
		system("cls");

		std::cout << "**** Payroll System Manager ****\n";
		std::cout << "1 = Create Pay Period\n";
		std::cout << "2 = Delete Pay Period\n";
		std::cout << "3 = View Pay Periods\n";
		std::cout << "4 = Process Payroll for Pay Period\n";
		std::cout << "5 = View Payroll Records\n";
		std::cout << "6 = View Employee Payroll History\n";
		std::cout << "7 = Enter Employee Hours\n";
		std::cout << "8 = View Time Entries\n";
		std::cout << "9 = Activate / Deactivate Employee\n";
		std::cout << "10 = Add Employee\n";
		std::cout << "11 = View Employees\n";
		std::cout << "12 = Logout\n";
		std::cout << "13 = Exit Program\n";
		std::cout << "Input: ";

		if (!(std::cin >> decision)) {
			std::cout << "\nInvalid Input!\n";

			Utils::ClearInputBuffer();
			Utils::Pause();

			continue;
		}

		Utils::ClearInputBuffer();

		switch (decision) {
			case 1:
				createPayPeriod(db);

				break;
			case 2:
				deletePayPeriod(db);

				break;
			case 3:
				viewPayPeriods(db, true);

				break;
			case 4:
				processPayroll(db);

				break;
			case 5:
				viewPayrollRecords(db);

				break;
			case 6:
				viewEmployeePayrollHistory(db);

				break;
			case 7:
				enterEmployeeHours(db);

				break;
			case 8:
				viewTimeEntries(db, true);
				
				break;
			case 9:
				toggleEmployeeStatus(db);

				break;
			case 10:
				createEmployee(db);

				break;
			case 11:
				viewEmployees(db, true);

				break;
			case 12:
				auth.logout();

				std::cout << "\nUser Successfully Logged Out\n";
				Utils::Pause();

				running = false;
				break;
			case 13:
				std::cout << "\nExiting Program\n";
				Utils::Pause();

				return false;
			default:
				std::cout << "\nInvalid Input: Number must be between (1 - 13)\n";
				Utils::Pause();
		}
	}

	return true;
}

void PayrollManager::createPayPeriod(Database& db) {
	system("cls");
	std::regex date_regex(R"(\d{2}-\d{2}-\d{4})");
	std::string startDate, endDate;

	std::cout << "**** Create Pay Period ****\n";
	std::cout << "Date Format: 00-00-0000 || day-month-year\n\n";

	std::cout << "Start Date: ";
	std::getline(std::cin >> std::ws, startDate);
	if (!std::regex_match(startDate, date_regex)) {
		std::cout << "\nInvalid Date Format! (00-00-0000 || 01-01-2026)\n";
		Utils::Pause();

		return;
	}

	std::cout << "End Date: ";
	std::getline(std::cin >> std::ws, endDate);
	if (!std::regex_match(endDate, date_regex)) {
		std::cout << "\nInvalid Date Format! (00-00-0000 || 01-01-2026)\n";
		Utils::Pause();

		return;
	}

	// Fromating Date (day-month-year --> year-month-day || 00-00-0000 --> 0000-00-00) //
	Utils::formatDate(startDate);
	Utils::formatDate(endDate);

	bool success = db.addPayPeriod(startDate, endDate);
	std::cout << '\n' << (success ? "Pay period successfully Created" : "Failed to create pay period!") << '\n';
	Utils::Pause();
}

void PayrollManager::deletePayPeriod(Database& db) {
	system("cls");
	int payPeriodID;
	char confirmDeletion;

	std::cout << "**** Delete Pay Period ****\n";
	std::cout << "Input Format: 1, 2, 3 (type = number)\n\n";
	viewPayPeriods(db, false);

	std::cout << "Enter Pay Period ID: ";
	if (!(std::cin >> payPeriodID)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << "\nAre you sure you want to delete pay period ID: " << payPeriodID << " (y/n)\n";
	std::cout << "Input: ";
	if (!(std::cin >> confirmDeletion)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	if (std::tolower(confirmDeletion) != 'y') {
		std::cout << "\nDeletion Cancelled\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	if (db.removePayPeriod(payPeriodID)) {
		std::cout << "\nSuccessfully Deleted Pay Period\n";
	}

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::viewPayPeriods(Database& db, bool waitForUserInput) {
	if (waitForUserInput) {
		system("cls");
		std::cout << "**** View Pay Periods ****\n\n";
	}

	std::cout << std::left << std::setw(5) << "ID" << std::setw(15) << "Start Date" << std::setw(15) << "End Date" << std::setw(10) << "Status" << '\n';
	std::cout << "---------------------------------------------\n";
														
	std::vector<PayPeriod> payPeriods = db.getPayPeriods();
	if (payPeriods.empty()) {
		std::cout << "No Pay Periods Found\n\n";
		if (waitForUserInput) { Utils::Pause(); }
		
		return;
	}

	for (const PayPeriod& period : payPeriods) {
		std::cout << std::left
			<< std::setw(5) << period.id
			<< std::setw(15) << period.startDate
			<< std::setw(15) << period.endDate
			<< std::setw(10) << (period.isProcessed() ? "Processed" : "Open") << '\n';
	}

	std::cout << '\n';
	if (waitForUserInput) { Utils::Pause(); }
}

void PayrollManager::processPayroll(Database& db) {
	system("cls");
	int payPeriodID;

	std::cout << "**** Process Payroll for Pay Period ****\n";
	std::cout << "Input Format: 1, 2, 3 (type = number)\n\n";
	viewPayPeriods(db, false);

	std::cout << "Enter Pay Period ID: ";
	if (!(std::cin >> payPeriodID)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << "\nStarting payroll processing for pay period: " << payPeriodID << '\n';
	bool success = db.insertPayrollForPayPeriod(payPeriodID);
	std::cout << '\n' << (success ? "Payroll successfully processed" : "Failed to process payroll") << '\n';
	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::viewPayrollRecords(Database& db) {
	system("cls");

	std::cout << "**** View Payroll Records ****\n\n";
	std::cout << std::left
		<< std::setw(12) << "Employee ID"
		<< std::setw(15) << "First Name"
		<< std::setw(15) << "Last Name"
		<< std::setw(25) << "Pay Period"
		<< std::setw(12) << "Gross Pay"
		<< std::setw(12) << "Net Pay"
		<< '\n';
	std::cout << "--------------------------------------------------------------------------------------\n";

	std::vector<Payroll> payrollRecords = db.getPayrollRecords();
	if (payrollRecords.empty()) {
		std::cout << "No Payroll Records Found\n\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	for (Payroll& record : payrollRecords) {
		// Fromating Date (day-month-year --> year-month-day || 00-00-0000 --> 0000-00-00) //
		std::string payPeriod = record.startDate + " - " + record.endDate;

		std::ostringstream grossPay, netPay;
		grossPay << "$" << std::fixed << std::setprecision(2) << record.grossPay;
		netPay << "$" << std::fixed << std::setprecision(2) << record.netPay;

		std::cout << std::left
			<< std::setw(12) << record.employeeID
			<< std::setw(15) << record.firstName
			<< std::setw(15) << record.lastName
			<< std::setw(25) << payPeriod 
			<< std::setw(12) << grossPay.str()
			<< std::setw(12) << netPay.str()
			<< '\n';
	}

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::viewEmployeePayrollHistory(Database& db) {
	system("cls");
	int employeeID;

	std::cout << "**** View Employee Payroll History ****\n\n";
	viewEmployees(db, false);

	std::cout << "Employee ID: ";
	if (!(std::cin >> employeeID)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << '\n' << std::left << std::setw(5) << "ID" << std::setw(15) << "Start Date"
		<< std::setw(15) << "End Date" << std::setw(25) << "Processed At"
		<< std::setw(15) << "Gross Pay" << std::setw(15) << "Net Pay" << '\n';
	std::cout << "------------------------------------------------------------------------------------------\n";

	std::vector<EmployeePayroll> employee_payrolls = db.getEmployeePayrollHistory(employeeID);
	for (EmployeePayroll& payroll : employee_payrolls) {
		std::ostringstream grossPay, netPay;
		grossPay << "$" << std::fixed << std::setprecision(2) << payroll.grossPay;
		netPay << "$" << std::fixed << std::setprecision(2) << payroll.netPay;

		std::cout << std::left
			<< std::setw(5) << payroll.payPeriodID
			<< std::setw(15) << payroll.startDate
			<< std::setw(15) << payroll.endDate
			<< std::setw(25) << payroll.processedAt
			<< std::setw(15) << grossPay.str()
			<< std::setw(15) << netPay.str() << '\n';
	}

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::viewTimeEntries(Database& db, bool waitForUserInput) {
	if (waitForUserInput) {
		system("cls");
		std::cout << "**** View Employee Hours ****\n\n";
	}

	std::cout << std::left << std::setw(5) << "ID" << std::setw(15) << "Date Worked" << std::setw(15) << "Hours Worked" << '\n';
	std::cout << "-----------------------------------\n";

	std::vector<TimeEntry> TimeEntries = db.getTimeEntries();
	if (TimeEntries.empty()) {
		std::cout << "No Time Entries Found\n\n";
		Utils::Pause();

		return;
	}

	for (TimeEntry& entry : TimeEntries) {
		std::cout << std::left
			<< std::setw(5) << entry.employeeID
			<< std::setw(15) << entry.dateWorked
			<< std::setw(15) << entry.hoursWorked << '\n';
	}

	std::cout << "\n";
	if (waitForUserInput) { Utils::Pause(); }
}

void PayrollManager::enterEmployeeHours(Database& db) {
	system("cls");
	TimeEntry timeEntry;
	std::regex date_regex(R"(\d{2}-\d{2}-\d{4})");

	std::cout << "**** Enter Employee Hours ****\n\n";
	viewEmployees(db, false);

	std::cout << "\nEmployee ID: ";
	if (!(std::cin >> timeEntry.employeeID)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << "Date Worked: ";
	std::getline(std::cin >> std::ws, timeEntry.dateWorked);
	if (!std::regex_match(timeEntry.dateWorked, date_regex)) {
		std::cout << "\nInvalid Date Format! (00-00-0000 || 01-01-2026)\n";
		Utils::Pause();

		return;
	}

	std::cout << "Hours Worked: ";
	if (!(std::cin >> timeEntry.hoursWorked)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	// Fromating Date (day-month-year --> year-month-day || 00-00-0000 --> 0000-00-00) //
	Utils::formatDate(timeEntry.dateWorked);

	bool success = db.addTimeEntry(timeEntry);
	if (success)
		std::cout << "\nTime Entry Successfully Added\n";

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::toggleEmployeeStatus(Database& db) {
	system("cls");
	int employeeID;
	char isActive;

	std::cout << "**** Activate / Deactivate Employee Status ****\n\n";
	viewEmployees(db, false);

	std::cout << "Employee ID: ";
	if (!(std::cin >> employeeID)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << "Activate (y/n): ";
	if (!(std::cin >> isActive)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	isActive = std::tolower(isActive);
	if (isActive != 'y' && isActive != 'n') {
		std::cout << "\ninvalid Character: Input Must Be 'y' or 'n'\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	bool success = db.setEmployeeStatus(employeeID, isActive == 'y');
	if (success)
		std::cout << "\nEmployee Status Successfully " << (isActive == 'y' ? "Activated" : "Deactivated") << '\n';

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::createEmployee(Database& db) {
	system("cls");
	Employee emp;

	std::cout << "**** Add An Employee ****\n\n";
	std::cout << "First Name: ";
	std::getline(std::cin >> std::ws, emp.firstName);

	std::cout << "Last Name: ";
	std::getline(std::cin >> std::ws, emp.lastName);

	std::cout << "Department: ";
	std::getline(std::cin >> std::ws, emp.department);

	std::cout << "Employment Type (eg. full-time or part-time): ";
	std::getline(std::cin >> std::ws, emp.employmentType);

	std::cout << "Hourly Pay Rate: ";
	if (!(std::cin >> emp.hourlyRate)) {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}

	std::cout << "\nEmployee Active? (y/n)\n";
	std::cout << "Input: ";
	char active;
	std::cin >> active;
	active = std::tolower(active);
	
	if (active != 'y' && active != 'n') {
		std::cout << "\nInvalid Input\n";

		Utils::ClearInputBuffer();
		Utils::Pause();

		return;
	}
	else {
		emp.isActive = active == 'y';
	}

	bool success = db.addEmployee(emp);
	if (success)
		std::cout << "\nEmployee Successfully Added\n";

	Utils::ClearInputBuffer();
	Utils::Pause();
}

void PayrollManager::viewEmployees(Database& db, bool waitForUserInput) {
	if (waitForUserInput) {
		system("cls");
		std::cout << "**** View Employees ****\n\n";
	}

	std::cout << std::left << std::setw(5) << "ID"
		<< std::setw(15) << "First Name"
		<< std::setw(15) << "Last Name"
		<< std::setw(15) << "Department"
		<< std::setw(15) << "Hire Date"
		<< std::setw(15) << "Hourly Rate"
		<< std::setw(15) << "Status" << '\n';
	std::cout << "-----------------------------------------------------------------------------------------------\n";

	std::vector<Employee> employees = db.getEmployees();
	if (employees.empty()) {
		std::cout << "No Employee Found\n\n";
		Utils::Pause();

		return;
	}

	for (Employee& emp : employees) {
		std::cout << std::left
			<< std::setw(5) << emp.id
			<< std::setw(15) << emp.firstName
			<< std::setw(15) << emp.lastName
			<< std::setw(15) << emp.department
			<< std::setw(15) << emp.hireDate
			<< std::setw(15) << emp.hourlyRate
			<< std::setw(15) << (emp.isActive ? "Working" : "Resting") << '\n';
	}

	std::cout << "\n";
	if (waitForUserInput) { Utils::Pause(); }
}
