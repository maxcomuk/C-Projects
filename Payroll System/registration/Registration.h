#pragma once

#include <string>
#include "../Utilities.h"

class AuthManager
{
private:
	std::string admin_username;
	std::string admin_password;

	bool userLoggedIn{ false };
public:
	AuthManager(const std::string& adminUsername, const std::string& adminPassword);
	AuthManager();

	bool setupRegistration();
	bool authenticateUser();
	bool getLoggedInStatus();
	void logout();
};
