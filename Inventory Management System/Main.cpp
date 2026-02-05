#include <iostream>
#include <limits>

#include "Utilities.h"
#include "db/Database.h"
#include "registration/Registration.h"
#include "inventory/InvManager.h"

int main()
{
	AuthManager auth;
	Database db;
	InventoryManager inv;

	if (!(db.OpenDatabase("Inventory Management System.db")))
		return 1;

	if (!(dbUtils::SetupTables(db)))
		return 1;

	std::cout << "**** Inventory System ****\n";

	while (true)
	{
		if (!(auth.get_login_status())) // Checking if the user is logged in
		{
			if (!(auth.setup_user_registration(db))) // false = user chose to exit program
				break;
		}

		if (!(inv.setup_inventory_manager(db, auth))) // false = user chose to exit program
			break;
	}

	return 0;
}
