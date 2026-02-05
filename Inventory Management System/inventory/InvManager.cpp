#include <iostream>
#include "InvManager.h"

bool InventoryManager::setup_inventory_manager(Database& db, AuthManager& auth)
{
	int userInput;

	while (true)
	{
		system("cls");
		dbUtils::inventoryMenu();
		if (!(std::cin >> userInput))
		{
			std::cout << "\nInvalid Input: Please try again\n";
			std::cout << "\nPress Enter To Continue . . .";

			dbUtils::ClearInputBuffer();
			std::cin.get();

			continue;
		}

		if (userInput < 1 || userInput > 10)
		{
			std::cout << "\nInvalid Number!\n";
			std::cout << "\nPress Enter To Continue . . .";

			dbUtils::ClearInputBuffer();
			std::cin.get();

			continue;
		}

		if (userInput == 1)
		{
			InventoryManager::SelectItem("Hot Meal", db);
		}
		else if (userInput == 2)
		{
			InventoryManager::SelectItem("Cold Meal", db);
		}
		else if (userInput == 3)
		{
			InventoryManager::SelectItem("Drink", db);
		}
		else if (userInput == 4)
		{
			InventoryManager::SelectItem("Extra", db);
		}
		else if (userInput == 5)
		{
			InventoryManager::DisplayItems(db);
		}
		else if (userInput == 6)
		{
			InventoryManager::UpdateItem(db);
		}
		else if (userInput == 7)
		{
			InventoryManager::AddItem(db);
		}
		else if (userInput == 8)
		{
			InventoryManager::DeleteItem(db);
		}
		else if (userInput == 9)
		{
			InventoryManager::LogUserOut(auth);
			return true;
		}
		else if (userInput == 10)
		{
			std::cout << "\nExiting Program . . .\n";
			break;
		}
	}

	return false;
}

void InventoryManager::SelectItem(const char* itemCategory, Database& db)
{
	system("cls");
	InventoryItem storedItem{ itemCategory };

	std::cout << "Item Name: ";
	std::getline(std::cin >> std::ws, storedItem.productName);
	
	if (!(db.GetItem(storedItem)))
	{
		std::cout << "\nPress Enter To Continue . . .";
		std::cin.get();
		return;
	}

	std::cout << "\n**** Item Information ****\n";
	std::cout << "Name: " << storedItem.productName << '\n';
	std::cout << "Amount: " << storedItem.amount << '\n';
	std::cout << "Price: " << storedItem.price << '\n';
	std::cout << "Size: " << storedItem.size << '\n';
	std::cout << "Description: " << storedItem.description << '\n';
	std::cout << "Created At: " << storedItem.createdAt << '\n';

	std::cout << "\nPress Enter To Continue . . .";
	std::cin.get();
}

void InventoryManager::DisplayItems(Database& db)
{
	system("cls");

	std::vector<InventoryItem> storedItems = db.GetAllItems();
	if (storedItems.empty())
	{
		std::cout << "\nNo Items Found In Inventory!\n";
	}
	else {
		int count{ 0 };

		for (const InventoryItem& item : storedItems)
		{
			count++;

			std::cout << "**** Item " << count << " ****\n";
			std::cout << "Category: " << item.category << '\n';
			std::cout << "Name: " << item.productName << '\n';
			std::cout << "Amount: " << item.amount << "x\n";
			std::cout << "Price: $" << item.price << '\n';
			std::cout << "Size: " << item.size << "\n";
			std::cout << "Description: " << item.description << '\n';
			std::cout << "Created At: " << item.createdAt << '\n';
			std::cout << "---------------------------------------\n\n";
		}
	}

	std::cout << "\nPress Enter To Continue . . .";
	dbUtils::ClearInputBuffer();
	std::cin.get();
}

void InventoryManager::UpdateItem(Database& db)
{
	system("cls");
	InventoryItem original_object;
	InventoryItem object;

	std::cout << "Item Name: ";
	std::getline(std::cin >> std::ws, original_object.productName);
	
	if (!db.ValidateItem(original_object)) {
		std::cout << "\nItem Not Found\n";
		std::cout << "\nPress Enter To Continue . . .";

		dbUtils::ClearInputBuffer();
		std::cin.get();

		return;
	}
	else {
		std::cout << "Item Found: New Values Below\n\n";
	}

	while (true)
	{
		object.category.clear();
		object.productName.clear();
		object.description.clear();
		object.amount = 0;
		object.price = 0.0;

		if (!(invUtils::get_product_category(object.category)))
			continue;

		invUtils::get_product_name(object.productName);

		if (!(invUtils::get_product_amount(object.amount)))
			continue;

		if (!(invUtils::get_product_price(object.price)))
			continue;

		invUtils::get_product_size(object.size);
		invUtils::get_product_description(object.description);

		if (!object.category.empty() && !object.productName.empty() && object.amount > 0 && object.price > 0 && !object.size.empty() && !object.description.empty())
		{
			if (db.ModifyItem(original_object.productName, object))
			{
				db.ValidateItem(object);

				std::cout << "\n**** Successfully Updated Item ****\n";
				std::cout << "Category: " << original_object.category << " ---> " << object.category << '\n';
				std::cout << "Name: " << original_object.productName << " ---> " << object.productName << '\n';
				std::cout << "Amount: " << original_object.amount << "x ---> " << object.amount << "x\n";
				std::cout << "Price: $" << original_object.price << " ---> $" << object.price << '\n';
				std::cout << "Size: " << original_object.size << " ---> " << object.size << '\n';
				std::cout << "Description: " << original_object.description << " ---> " << object.description << '\n';
				std::cout << "Created At: " << original_object.createdAt << " ---> " << object.createdAt << '\n';
				std::cout << "-----------------------------------------------------------------\n";

				break;
			}
		}
	}

	std::cout << "\nPress Enter To Continue . . .";
	std::cin.get();
}

void InventoryManager::AddItem(Database& db)
{
	std::string itemCategory, itemName, itemSize, itemDesc;
	int amount;
	double price;
	
	while (true)
	{
		system("cls");

		itemCategory.clear();
		itemName.clear();
		itemSize.clear();
		itemDesc.clear();
		amount = 0;
		price = 0.0;

		if (!(invUtils::get_product_category(itemCategory)))
			continue;

		invUtils::get_product_name(itemName);

		if (!(invUtils::get_product_amount(amount)))
			continue;

		if (!(invUtils::get_product_price(price)))
			continue;

		invUtils::get_product_size(itemSize);
		invUtils::get_product_description(itemDesc);

		if (!itemCategory.empty() && !itemName.empty() && amount > 0 && price > 0 && !itemSize.empty() && !itemDesc.empty())
		{
			InventoryItem object{ itemCategory, itemName, amount, price, itemSize, itemDesc };
			db.InsertItem(object);
			break;
		}
	}
}

void InventoryManager::DeleteItem(Database& db)
{
	system("cls");
	std::string productName;

	std::cout << "Item Name: ";
	std::getline(std::cin >> std::ws, productName);

	if (db.RemoveItem(productName)) { std::cout << "\nItem Deleted Successfully\n"; }
	std::cout << "\nPress Enter To Continue . . .";
	std::cin.get();
}

void InventoryManager::LogUserOut(AuthManager& auth)
{
	std::string user = auth.get_current_user();
	auth.LogOut();

	std::cout << '\n' << user << " Successfully Logged Out\n";
	std::cout << "\nPress Enter To Continue . . .";

	dbUtils::ClearInputBuffer();
	std::cin.get();
}
