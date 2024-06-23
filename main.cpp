#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Structure to represent an Item
struct Item {
    string name;
    string sku;
    string description;
    double price;
    int quantity;
};

vector<Item> inventory;  // Vector to store items in inventory

// Function to add a new item to the inventory
void addItem() {
    Item item;

    cout << "Enter item name: ";
    getline(cin >> ws, item.name);  // Use getline to read item name, ws ensures
                                    // leading whitespace is ignored

    cout << "Enter item SKU: ";
    getline(cin >> ws, item.sku);  // Use getline to read item SKU

    cout << "Enter item description: ";
    getline(cin >> ws,
            item.description);  // Use getline to read item description

    cout << "Enter item price: ";
    while (!(cin >> item.price)) {
        cout << "Invalid price. Please enter a numeric value." << endl;
        cin.clear();  // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(),
                   '\n');  // Ignore invalid input
    }

    cout << "Enter item quantity: ";
    while (!(cin >> item.quantity)) {
        cout << "Invalid quantity. Please enter a numeric value." << endl;
        cin.clear();  // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(),
                   '\n');  // Ignore invalid input
    }

    inventory.push_back(item);
    cout << "Item has been added successfully." << endl;

    cin.ignore(numeric_limits<streamsize>::max(),
               '\n');  // Clear remaining newline character in buffer
}

// Function to update an existing item in the inventory
void updateItem() {
    string sku;
    cout << "Enter SKU of item to update: ";
    cin >> sku;

    for (auto &item : inventory) {
        if (item.sku == sku) {
            cout << "Enter new item name: ";
            cin.ignore(numeric_limits<streamsize>::max(),
                       '\n');  // Clear input buffer
            getline(cin, item.name);

            cout << "Enter new item description: ";
            getline(cin, item.description);

            cout << "Enter new item price: ";
            cin >> item.price;

            cout << "Enter new item quantity: ";
            cin >> item.quantity;

            cout << "Item updated successfully." << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

// Function to remove an item from the inventory
void removeItem() {
    string sku;
    cout << "Enter SKU of item to remove: ";
    cin >> sku;

    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->sku == sku) {
            inventory.erase(it);
            cout << "Item removed successfully." << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

// Function to update quantity of an item in the inventory
void updateQuantity() {
    string sku;
    cout << "Enter SKU of item to update quantity: ";
    cin >> sku;

    for (auto &item : inventory) {
        if (item.sku == sku) {
            cout << "Enter new quantity: ";
            cin >> item.quantity;
            cout << "Quantity updated successfully." << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

// Function to generate and display inventory report
void generateReport() {
    if (inventory.empty()) {
        cout << "Inventory is empty." << endl;
        return;
    }

    double totalValue = 0;
    bool hasLowStockItems = false;

    cout << "Inventory Report:" << endl;
    cout << "=================" << endl;

    for (const auto &item : inventory) {
        totalValue += item.price * item.quantity;

        if (item.quantity < 10) {
            hasLowStockItems = true;
            cout << item.name << " - " << item.quantity << " items left"
                 << endl;
        }
    }

    cout << "Total stock value: $" << totalValue << endl;

    if (!hasLowStockItems) {
        cout << "No items with low stock." << endl;
    }
}

// Function to search for an item in the inventory by name
void searchItem() {
    string name;
    cout << "Enter item name to search: ";
    cin >> name;

    for (const auto &item : inventory) {
        if (item.name == name) {
            cout << "Item found:" << endl;
            cout << "Name: " << item.name << endl;
            cout << "SKU: " << item.sku << endl;
            cout << "Description: " << item.description << endl;
            cout << "Price: $" << item.price << endl;
            cout << "Quantity: " << item.quantity << " items" << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

// Function to filter items by a specified category (in this case, by
// description)
void filterItems() {
    string category;
    cout << "Enter category to filter (description): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer
    getline(cin, category);

    bool found = false;

    for (const auto &item : inventory) {
        if (item.description.find(category) != string::npos) {
            found = true;
            cout << "Name: " << item.name << endl;
            cout << "SKU: " << item.sku << endl;
            cout << "Description: " << item.description << endl;
            cout << "Price: $" << item.price << endl;
            cout << "Quantity: " << item.quantity << " items" << endl;
            cout << "--------------------------" << endl;
        }
    }

    if (!found) {
        cout << "No items found matching the category: " << category << endl;
    }
}

// Function to save inventory to a CSV file
void saveInventory() {
    ofstream file("inventory.csv");

    if (!file.is_open()) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    for (const auto &item : inventory) {
        file << item.name << "," << item.sku << "," << item.description << ","
             << item.price << "," << item.quantity << endl;
    }

    file.close();
    cout << "Inventory saved successfully." << endl;
}

// Function to load inventory from a CSV file
void loadInventory() {
    ifstream file("inventory.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Item item;

        getline(ss, token, ',');
        item.name = token;

        getline(ss, token, ',');
        item.sku = token;

        // Combine remaining tokens into description
        item.description = "";
        while (getline(ss, token, ',')) {
            if (!item.description.empty()) {
                item.description += ",";
            }
            item.description += token;
        }

        // Convert price from string to double
        getline(ss, token, ',');
        try {
            item.price = stod(token);
        } catch (const std::invalid_argument &e) {
            cerr << "Error converting price to double: " << token << endl;
            continue;  // Skip this item and proceed with the next one
        } catch (const std::out_of_range &e) {
            cerr << "Out of range error converting price to double: " << token
                 << endl;
            continue;  // Skip this item and proceed with the next one
        }

        // Convert quantity from string to int
        getline(ss, token, ',');
        try {
            item.quantity = stoi(token);
        } catch (const std::invalid_argument &e) {
            cerr << "Error converting quantity to int: " << token << endl;
            continue;  // Skip this item and proceed with the next one
        } catch (const std::out_of_range &e) {
            cerr << "Out of range error converting quantity to int: " << token
                 << endl;
            continue;  // Skip this item and proceed with the next one
        }

        inventory.push_back(item);
    }

    file.close();
    cout << "Inventory loaded successfully." << endl;
}

// Main function to run the inventory management system
int main() {
    loadInventory();  // Load inventory from file

    int choice;

    while (true) {
        cout << "\nInventory Management System" << endl;
        cout << "===========================" << endl;
        cout << "1. Add item" << endl;
        cout << "2. Update item" << endl;
        cout << "3. Remove item" << endl;
        cout << "4. Update quantity" << endl;
        cout << "5. Generate report" << endl;
        cout << "6. Search item" << endl;
        cout << "7. Filter items" << endl;
        cout << "8. Save and exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                do {
                    addItem();
                    cout << "Do you want to add another item? (1 = Yes, 0 = "
                            "No): ";
                    cin >> choice;
                } while (choice == 1);
                break;
            case 2:
                updateItem();
                break;
            case 3:
                removeItem();
                break;
            case 4:
                updateQuantity();
                break;
            case 5:
                generateReport();
                break;
            case 6:
                searchItem();
                break;
            case 7:
                filterItems();
                break;
            case 8:
                saveInventory();
                cout << "Exiting program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
