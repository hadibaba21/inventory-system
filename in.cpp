#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <algorithm>

using namespace std;

// Inventory Item Structure
struct Item {
    int id;
    string name;
    string category;
    int quantity;
    double price;
    
    Item() : id(0), quantity(0), price(0.0) {}
    Item(int i, string n, string c, int q, double p)
        : id(i), name(n), category(c), quantity(q), price(p) {}
};

class InventoryManagementSystem {
private:
    vector<Item> inventory;
    int nextId;
    string filename;
    
public:
    InventoryManagementSystem(string fileName = "inventory.txt") 
        : filename(fileName), nextId(1) {
        loadInventory();
    }
    
    // Generate unique ID
    int generateId() {
        return nextId++;
    }
    
    // Save inventory to file
    void saveInventory() {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file for saving!" << endl;
            return;
        }
        
        file << inventory.size() << endl;
        file << nextId << endl;
        
        for (const auto& item : inventory) {
            file << item.id << " "
                 << item.name << " "
                 << item.category << " "
                 << item.quantity << " "
                 << item.price << endl;
        }
        
        file.close();
        cout << "Inventory saved successfully!" << endl;
    }
    
    // Load inventory from file
    void loadInventory() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "No existing inventory file found. Starting fresh." << endl;
            return;
        }
        
        int size;
        file >> size;
        file >> nextId;
        
        inventory.clear();
        for (int i = 0; i < size && file.good(); i++) {
            Item item;
            item.id = readInt(file);
            cin.ignore(); // Clear newline
            getline(file, item.name, ' ');
            getline(file, item.category, ' ');
            item.quantity = readInt(file);
            item.price = readDouble(file);
            
            inventory.push_back(item);
        }
        
        file.close();
        cout << "Loaded " << inventory.size() << " items from file." << endl;
    }
    
    // Helper function to read int safely
    int readInt(ifstream& file) {
        int val;
        file >> val;
        return val;
    }
    
    // Helper function to read double safely
    double readDouble(ifstream& file) {
        double val;
        file >> val;
        return val;
    }
    
    // Add new item to inventory
    void addItem() {
        cout << "\n=== ADD NEW ITEM ===" << endl;
        
        Item newItem;
        newItem.id = generateId();
        
        cout << "Item Name: ";
        cin.ignore();
        getline(cin, newItem.name);
        
        cout << "Category: ";
        getline(cin, newItem.category);
        
        cout << "Quantity: ";
        newItem.quantity = getValidPositiveInt("Please enter a positive number for quantity: ");
        
        cout << "Price: ";
        newItem.price = getValidPositiveDouble("Please enter a valid positive price: ");
        
        inventory.push_back(newItem);
        cout << "Item added successfully! ID: " << newItem.id << endl;
        saveInventory();
    }
    
    // Remove item from inventory
    void removeItem() {
        cout << "\n=== REMOVE ITEM ===" << endl;
        displayInventory();
        
        int idToRemove;
        cout << "Enter Item ID to remove: ";
        idToRemove = getValidPositiveInt("Please enter a valid positive ID: ");
        
        auto it = find_if(inventory.begin(), inventory.end(),
                         [idToRemove](const Item& item) {
                             return item.id == idToRemove;
                         });
        
        if (it != inventory.end()) {
            cout << "Removing: " << it->name << " (ID: " << it->id << ")" << endl;
            inventory.erase(it);
            cout << "Item removed successfully!" << endl;
            saveInventory();
        } else {
            cout << "Item with ID " << idToRemove << " not found!" << endl;
        }
    }
    
    // Update item details
    void updateItem() {
        cout << "\n=== UPDATE ITEM ===" << endl;
        displayInventory();
        
        int idToUpdate;
        cout << "Enter Item ID to update: ";
        idToUpdate = getValidPositiveInt("Please enter a valid positive ID: ");
        
        auto it = find_if(inventory.begin(), inventory.end(),
                         [idToUpdate](const Item& item) {
                             return item.id == idToUpdate;
                         });
        
        if (it != inventory.end()) {
            cout << "Current Details:" << endl;
            cout << "Name: " << it->name << endl;
            cout << "Category: " << it->category << endl;
            cout << "Quantity: " << it->quantity << endl;
            cout << "Price: $" << it->price << endl;
            
            cout << "\nEnter new values (press Enter to keep current):" << endl;
            
            cout << "New Name: ";
            cin.ignore();
            string newName;
            getline(cin, newName);
            if (!newName.empty()) it->name = newName;
            
            cout << "New Category: ";
            string newCategory;
            getline(cin, newCategory);
            if (!newCategory.empty()) it->category = newCategory;
            
            cout << "New Quantity: ";
            int newQty;
            cin >> newQty;
            if (newQty >= 0) it->quantity = newQty;
            
            cout << "New Price: ";
            double newPrice;
            cin >> newPrice;
            if (newPrice >= 0) it->price = newPrice;
            
            cout << "Item updated successfully!" << endl;
            saveInventory();
        } else {
            cout << "Item with ID " << idToUpdate << " not found!" << endl;
        }
    }
    
    // Search items by name or category
    void searchItems() {
        cout << "\n=== SEARCH ITEMS ===" << endl;
        cout << "Search by:\n1. Name\n2. Category\n";
        
        int choice;
        cout << "Enter choice (1-2): ";
        choice = getValidPositiveInt("Please enter 1 or 2: ");
        
        string searchTerm;
        cin.ignore();
        cout << "Enter search term: ";
        getline(cin, searchTerm);
        
        vector<Item> results;
        
        if (choice == 1) {
            for (const auto& item : inventory) {
                if (item.name.find(searchTerm) != string::npos) {
                    results.push_back(item);
                }
            }
        } else {
            for (const auto& item : inventory) {
                if (item.category.find(searchTerm) != string::npos) {
                    results.push_back(item);
                }
            }
        }
        
        if (results.empty()) {
            cout << "No items found matching '" << searchTerm << "'" << endl;
        } else {
            cout << "\nFound " << results.size() << " items:" << endl;
            displayTable(results);
        }
    }
    
    // Display all inventory
    void displayInventory() {
        if (inventory.empty()) {
            cout << "\nInventory is empty!" << endl;
            return;
        }
        
        cout << "\n=== INVENTORY LIST ===" << endl;
        displayTable(inventory);
    }
    
    // Display items in table format
    void displayTable(const vector<Item>& items) {
        cout << left << setw(6) << "ID" 
             << setw(25) << "Name" 
             << setw(20) << "Category" 
             << setw(10) << "Qty" 
             << setw(12) << "Price" << endl;
        cout << string(73, '-') << endl;
        
        for (const auto& item : items) {
            cout << left << setw(6) << item.id 
                 << setw(25) << item.name.substr(0, 24) 
                 << setw(20) << item.category.substr(0, 19) 
                 << setw(10) << item.quantity 
                 << setw(12) << "$" << fixed << setprecision(2) << item.price << endl;
        }
        
        cout << string(73, '=') << endl;
        cout << "Total Items: " << items.size() << endl;
        cout << "Total Value: $" << calculateTotalValue(items) << endl;
    }
    
    // Calculate total inventory value
    double calculateTotalValue(const vector<Item>& items) {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.quantity * item.price;
        }
        return total;
    }
    
    // Get low stock items
    void lowStockAlert(int threshold = 5) {
        cout << "\n=== LOW STOCK ALERT (Threshold: " << threshold << ") ===" << endl;
        
        vector<Item> lowStock;
        for (const auto& item : inventory) {
            if (item.quantity <= threshold) {
                lowStock.push_back(item);
            }
        }
        
        if (lowStock.empty()) {
            cout << "All items have sufficient stock!" << endl;
        } else {
            cout << "Warning: " << lowStock.size() << " items below threshold!" << endl;
            displayTable(lowStock);
        }
    }
    
    // Get category summary
    void categorySummary() {
        if (inventory.empty()) {
            cout << "\nInventory is empty!" << endl;
            return;
        }
        
        cout << "\n=== CATEGORY SUMMARY ===" << endl;
        cout << left << setw(25) << "Category" 
             << setw(15) << "Item Count" 
             << setw(15) << "Total Qty" 
             << setw(15) << "Total Value" << endl;
        cout << string(70, '-') << endl;
        
        vector<string> categories;
        for (const auto& item : inventory) {
            if (find(categories.begin(), categories.end(), item.category) == categories.end()) {
                categories.push_back(item.category);
            }
        }
        
        for (const auto& cat : categories) {
            int count = 0;
            int qty = 0;
            double value = 0.0;
            
            for (const auto& item : inventory) {
                if (item.category == cat) {
                    count++;
                    qty += item.quantity;
                    value += item.quantity * item.price;
                }
            }
            
            cout << left << setw(25) << cat 
                 << setw(15) << count 
                 << setw(15) << qty 
                 << "$" << setw(14) << fixed << setprecision(2) << value << endl;
        }
        
        cout << string(70, '=') << endl;
    }
    
    // Input validation helpers
    int getValidPositiveInt(const string& errorMsg) {
        int value;
        while (!(cin >> value) || value < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << errorMsg << " ";
        }
        cin.ignore();
        return value;
    }
    
    double getValidPositiveDouble(const string& errorMsg) {
        double value;
        while (!(cin >> value) || value < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << errorMsg << " ";
        }
        cin.ignore();
        return value;
    }
    
    // Main menu loop
    void run() {
        int choice;
        
        do {
            cout << "\n========================================" << endl;
            cout << "   INVENTORY MANAGEMENT SYSTEM" << endl;
            cout << "========================================" << endl;
            cout << "1. Add New Item" << endl;
            cout << "2. View All Items" << endl;
            cout << "3. Update Item" << endl;
            cout << "4. Remove Item" << endl;
            cout << "5. Search Items" << endl;
            cout << "6. Low Stock Alert" << endl;
            cout << "7. Category Summary" << endl;
            cout << "8. Save & Exit" << endl;
            cout << "9. Just Exit (No Save)" << endl;
            cout << "========================================" << endl;
            cout << "Enter choice (1-9): ";
            
            choice = getValidPositiveInt("Please enter a valid option (1-9): ");
            
            switch(choice) {
                case 1: addItem(); break;
                case 2: displayInventory(); break;
                case 3: updateItem(); break;
                case 4: removeItem(); break;
                case 5: searchItems(); break;
                case 6: lowStockAlert(); break;
                case 7: categorySummary(); break;
                case 8: 
                    saveInventory();
                    cout << "Goodbye!" << endl;
                    return;
                case 9: 
                    cout << "Exiting without saving changes..." << endl;
                    return;
                default: 
                    cout << "Invalid choice! Please try again." << endl;
            }
            
        } while (true);
    }
};

// Main function
int main() {
    cout << "========================================" << endl;
    cout << " Welcome to Inventory Management System" << endl;
    cout << " Built with C++" << endl;
    cout << "========================================" << endl;
    
    InventoryManagementSystem ims;
    ims.run();
    
    return 0;
}