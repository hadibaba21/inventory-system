#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 100;

int ids[MAX];
string names[MAX];
int qty[MAX];
double price[MAX];
int n = 0;

int findItem(int id) {
    for (int i = 0; i < n; i++)
        if (ids[i] == id) return i;
    return -1;
}

void showItem(int i) {
    cout << left << setw(8) << ids[i] << setw(25) << names[i]
         << setw(12) << qty[i] << price[i] << endl;
}

void showHeader() {
    cout << left << setw(8) << "ID" << setw(25) << "Name"
         << setw(12) << "Quantity" << "Price" << endl;
}

void addItem() {
    if (n == MAX) {
        cout << "Inventory full!\n";
        return;
    }
    int id = 0, q = 0;
    double p = 0;
    string name;
    cout << "Enter ID: ";
    cin >> id;
    if (id <= 0 || findItem(id) != -1) {
        cout << "Invalid or duplicate ID!\n";
        return;
    }
    cin.ignore(1000, '\n');
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter quantity: ";
    cin >> q;
    cout << "Enter price: ";
    cin >> p;
    if (cin.fail() || q < 0 || p < 0) {
        cout << "Invalid quantity or price!\n";
        return;
    }
    ids[n] = id;
    names[n] = name;
    qty[n] = q;
    price[n] = p;
    n++;
    cout << "Item added!\n";
}

void displayAll() {
    if (n == 0) {
        cout << "Inventory is empty!\n";
        return;
    }
    double total = 0;
    showHeader();
    for (int i = 0; i < n; i++) {
        showItem(i);
        total += qty[i] * price[i];
    }
    cout << "Total items: " << n << endl;
    cout << "Total stock value: " << total << endl;
}

void searchItem() {
    int id = 0;
    cout << "Enter ID: ";
    cin >> id;
    int i = findItem(id);
    if (i == -1) {
        cout << "Item not found!\n";
        return;
    }
    showHeader();
    showItem(i);
}

void updateItem() {
    int id = 0, q = 0;
    double p = 0;
    cout << "Enter ID: ";
    cin >> id;
    int i = findItem(id);
    if (i == -1) {
        cout << "Item not found!\n";
        return;
    }
    cout << "Enter new quantity: ";
    cin >> q;
    cout << "Enter new price: ";
    cin >> p;
    if (cin.fail() || q < 0 || p < 0) {
        cout << "Invalid quantity or price!\n";
        return;
    }
    qty[i] = q;
    price[i] = p;
    cout << "Item updated!\n";
}

void deleteItem() {
    int id = 0;
    cout << "Enter ID: ";
    cin >> id;
    int i = findItem(id);
    if (i == -1) {
        cout << "Item not found!\n";
        return;
    }
    for (int j = i; j < n - 1; j++) {
        ids[j] = ids[j + 1];
        names[j] = names[j + 1];
        qty[j] = qty[j + 1];
        price[j] = price[j + 1];
    }
    n--;
    cout << "Item deleted!\n";
}

void lowStockReport() {
    int count = 0;
    showHeader();
    for (int i = 0; i < n; i++) {
        if (qty[i] < 5) {
            showItem(i);
            count++;
        }
    }
    if (count == 0)
        cout << "No item is low on stock.\n";
}

void showMenu() {
    cout << "\n===== INVENTORY MENU =====\n";
    cout << "1. Add item\n";
    cout << "2. Display all items\n";
    cout << "3. Search item\n";
    cout << "4. Update quantity and price\n";
    cout << "5. Delete item\n";
    cout << "6. Low stock report\n";
    cout << "7. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    int choice = 0;
    cout << fixed << setprecision(2);
    do {
        showMenu();
        choice = 0;
        cin >> choice;
        switch (choice) {
            case 1: addItem(); break;
            case 2: displayAll(); break;
            case 3: searchItem(); break;
            case 4: updateItem(); break;
            case 5: deleteItem(); break;
            case 6: lowStockReport(); break;
            case 7: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (choice != 7);
    return 0;
}

