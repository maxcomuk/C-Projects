#pragma once

#include <string>
#include <vector>

#include "../db/Database.h"
#include "../registration/Registration.h"
#include "../Utilities.h"

class InventoryManager
{
private:

public:
	InventoryManager() {};
	~InventoryManager() {};

	bool setup_inventory_manager(Database& db, AuthManager& auth);
	
	void SelectItem(const char* itemCategory, Database& db);
	void DisplayItems(Database& db);
	void UpdateItem(Database& db);
	void AddItem(Database& db);
	void DeleteItem(Database& db);
	void LogUserOut(AuthManager& auth);
};
