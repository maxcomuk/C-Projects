#pragma once

#include "db/Database.h"

namespace dbUtils
{
	void ClearInputBuffer();
	bool SetupTables(Database& db);
	void registrationMenu();
	void inventoryMenu();
}

namespace invUtils
{
	bool get_product_category(std::string& itemCategory);
	void get_product_name(std::string& itemName);
	bool get_product_amount(int& itemAmount);
	bool get_product_price(double& itemPrice);
	void get_product_size(std::string& itemSize);
	void get_product_description(std::string& itemDesc);
}
