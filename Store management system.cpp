#include <iostream>
#include <string>
using namespace std;

//class for Person
class Person {
protected:
    string firstName;
    string lastName;
    string nationalID;
    int birthyear;
    string gender;

public:
    Person(string fname, string lname, string nid, int byear, string g)
            : firstName(fname), lastName(lname), nationalID(nid), birthyear(byear), gender(g) {}

    virtual void showInfo() {
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "National ID: " << nationalID << endl;
        cout << "Birth Year: " << birthyear << endl;
        cout << "Gender: " << gender << endl;
    }

    string getNationalID() { return nationalID; }
};

// Customer class derived from Person
class Customer : public Person {
private:
    string customerID;
    string province;
    string city;

public:
    Customer(string fname, string lname, string nid, int byear, string g,
             string cid, string prov, string cty)
            : Person(fname, lname, nid, byear, g), customerID(cid), province(prov), city(cty) {}

    void showInfo() override {
        Person::showInfo();
        cout << "Customer ID: " << customerID << endl;
        cout << "Location: " << city << ", " << province << endl;
    }

    string getCustomerID() { return customerID; }
};

// Product class
class Product {
private:
    string productID;
    string name;
    double price;
    string brand;
    double weight;

public:
    Product(string pid, string n, double p, string b, double w)
            : productID(pid), name(n), price(p), brand(b), weight(w) {
        if (price <= 0 || weight <= 0) {
            throw invalid_argument("Price and weight must be positive");
        }
    }

    void showInfo() {
        cout << "Product ID: " << productID << endl;
        cout << "Name: " << name << endl;
        cout << "Price: " << price << endl;
        cout << "Brand: " << brand << endl;
        cout << "Weight(kg): " << weight << endl;
    }

    string getProductID() { return productID; }
    double getPrice() { return price; }
};

// Dealer class
class Dealer {
private:
    string dealerID;
    string name;
    int establishmentYear;
    string economicCode;
    string ownerFirstName;
    string ownerLastName;
    string province;
    string city;

public:
    Dealer(string did, string n, int eyear, string ecode,
           string ofname, string olname, string prov, string cty)
            : dealerID(did), name(n), establishmentYear(eyear), economicCode(ecode),
              ownerFirstName(ofname), ownerLastName(olname), province(prov), city(cty) {}

    void showInfo() {
        cout << "Dealer ID: " << dealerID << endl;
        cout << "Name: " << name << endl;
        cout << "Established: " << establishmentYear << endl;
        cout << "Economic Code: " << economicCode << endl;
        cout << "Owner: " << ownerFirstName << " " << ownerLastName << endl;
        cout << "Location: " << city << ", " << province << endl;
    }

    string getDealerID() { return dealerID; }
};

// Purchase class
class Purchase {
private:
    Customer* customer;
    Product* product;
    Dealer* dealer;
    int quantity;
    string purchaseDate;

public:
    Purchase(Customer* c, Product* p, Dealer* d, int q, string date)
            : customer(c), product(p), dealer(d), quantity(q), purchaseDate(date) {
        if (quantity <= 0) {
            throw invalid_argument("Quantity must be positive");
        }
    }
    void showInfo() {
        cout << "Purchase Details:" << endl;
        cout << "Customer: " << customer->getCustomerID() << endl;
        cout << "Product: " << product->getProductID() << endl;
        cout << "Dealer: " << dealer->getDealerID() << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Date: " << purchaseDate << endl;
    }

    Customer* getCustomer() { return customer; }
    Product* getProduct() { return product; }
    Dealer* getDealer() { return dealer; }
    int getQuantity() { return quantity; }
    string getDate() { return purchaseDate; }
};

// Database class to manage all entities
class ShopDatabase {
private:
    const int MAX_SIZE = 100;

    Customer* customers[100];
    Product* products[100];
    Dealer* dealers[100];
    Purchase* purchases[100];

    int customerCount = 0;
    int productCount = 0;
    int dealerCount = 0;
    int purchaseCount = 0;

public:
    ~ShopDatabase() {
        // Clean up memory
        for (int i = 0; i < customerCount; i++) delete customers[i];
        for (int i = 0; i < productCount; i++) delete products[i];
        for (int i = 0; i < dealerCount; i++) delete dealers[i];
        for (int i = 0; i < purchaseCount; i++) delete purchases[i];
    }

    // Add a new product
    bool addProduct(string pid, string name, double price, string brand, double weight) {
        if (productCount >= MAX_SIZE) return false;

        // Check if product ID already exists
        for (int i = 0; i < productCount; i++) {
            if (products[i]->getProductID() == pid) return false;
        }

        try {
            products[productCount++] = new Product(pid, name, price, brand, weight);
            return true;
        } catch (...) {
            return false;
        }
    }

    // Remove a product
    bool removeProduct(string pid) {
        int index = -1;
        for (int i = 0; i < productCount; i++) {
            if (products[i]->getProductID() == pid) {
                index = i;
                break;
            }
        }

        if (index == -1) return false;

        // Delete the product
        delete products[index];

        // Shift remaining elements
        for (int i = index; i < productCount - 1; i++) {
            products[i] = products[i + 1];
        }
        productCount--;

        // Remove related purchases
        for (int i = 0; i < purchaseCount; ) {
            if (purchases[i]->getProduct()->getProductID() == pid) {
                delete purchases[i];
                for (int j = i; j < purchaseCount - 1; j++) {
                    purchases[j] = purchases[j + 1];
                }
                purchaseCount--;
            } else {
                i++;
            }
        }

        return true;
    }

    // Add a new customer
    bool addCustomer(string fname, string lname, string nid, int byear, string g,
                     string cid, string prov, string cty) {
        if (customerCount >= MAX_SIZE) return false;

        // Check if national ID already exists
        for (int i = 0; i < customerCount; i++) {
            if (customers[i]->getNationalID() == nid) return false;
        }

        customers[customerCount++] = new Customer(fname, lname, nid, byear, g, cid, prov, cty);
        return true;
    }

    // Remove a customer
    bool removeCustomer(string nid) {
        int index = -1;
        for (int i = 0; i < customerCount; i++) {
            if (customers[i]->getNationalID() == nid) {
                index = i;
                break;
            }
        }

        if (index == -1) return false;

        // Delete the customer
        delete customers[index];

        // Shift remaining elements
        for (int i = index; i < customerCount - 1; i++) {
            customers[i] = customers[i + 1];
        }
        customerCount--;

        // Remove related purchases
        for (int i = 0; i < purchaseCount; ) {
            if (purchases[i]->getCustomer()->getNationalID() == nid) {
                delete purchases[i];
                for (int j = i; j < purchaseCount - 1; j++) {
                    purchases[j] = purchases[j + 1];
                }
                purchaseCount--;
            } else {
                i++;
            }
        }

        return true;
    }

    // Add a new dealer
    bool addDealer(string did, string name, int eyear, string ecode,
                   string ofname, string olname, string prov, string cty) {
        if (dealerCount >= MAX_SIZE) return false;

        // Check if dealer ID already exists
        for (int i = 0; i < dealerCount; i++) {
            if (dealers[i]->getDealerID() == did) return false;
        }

        dealers[dealerCount++] = new Dealer(did, name, eyear, ecode, ofname, olname, prov, cty);
        return true;
    }

    // Remove a dealer
    bool removeDealer(string did) {
        int index = -1;
        for (int i = 0; i < dealerCount; i++) {
            if (dealers[i]->getDealerID() == did) {
                index = i;
                break;
            }
        }

        if (index == -1) return false;

        // Delete the dealer
        delete dealers[index];

        // Shift remaining elements
        for (int i = index; i < dealerCount - 1; i++) {
            dealers[i] = dealers[i + 1];
        }
        dealerCount--;

        // Remove related purchases
        for (int i = 0; i < purchaseCount; ) {
            if (purchases[i]->getDealer()->getDealerID() == did) {
                delete purchases[i];
                for (int j = i; j < purchaseCount - 1; j++) {
                    purchases[j] = purchases[j + 1];
                }
                purchaseCount--;
            } else {
                i++;
            }
        }

        return true;
    }

    // Record a new purchase
    bool recordPurchase(string nid, string pid, string did, int qty, string date) {
        if (purchaseCount >= MAX_SIZE) return false;

        // Find customer, product, dealer
        Customer* cust = nullptr;
        Product* prod = nullptr;
        Dealer* deal = nullptr;

        for (int i = 0; i < customerCount; i++) {
            if (customers[i]->getNationalID() == nid) {
                cust = customers[i];
                break;
            }
        }
        if (!cust) return false;

        for (int i = 0; i < productCount; i++) {
            if (products[i]->getProductID() == pid) {
                prod = products[i];
                break;
            }
        }
        if (!prod) return false;

        for (int i = 0; i < dealerCount; i++) {
            if (dealers[i]->getDealerID() == did) {
                deal = dealers[i];
                break;
            }
        }
        if (!deal) return false;

        try {
            purchases[purchaseCount++] = new Purchase(cust, prod, deal, qty, date);
            return true;
        } catch (...) {
            return false;
        }
    }

    // Calculate total purchase amount for a customer
    double getCustomerTotalPurchase(string nid) {
        double total = 0;
        for (int i = 0; i < purchaseCount; i++) {
            if (purchases[i]->getCustomer()->getNationalID() == nid) {
                total += purchases[i]->getProduct()->getPrice() * purchases[i]->getQuantity();
            }
        }
        return total;
    }

    // Display customers who bought a specific product
    void displayCustomersOfProduct(string pid) {
        bool found = false;
        for (int i = 0; i < purchaseCount; i++) {
            if (purchases[i]->getProduct()->getProductID() == pid) {
                if (!found) {
                    cout << "Customers who purchased product " << pid << ":" << endl;
                    found = true;
                }
                purchases[i]->getCustomer()->showInfo();
                cout << "----------" << endl;
            }
        }
        if (!found) {
            cout << "No customers found for product " << pid << endl;
        }
    }

    // Display products sold by a dealer
    void displayProductsOfDealer(string did) {
        bool found = false;
        for (int i = 0; i < purchaseCount; i++) {
            if (purchases[i]->getDealer()->getDealerID() == did) {
                if (!found) {
                    cout << "Products sold by dealer " << did << ":" << endl;
                    found = true;
                }
                purchases[i]->getProduct()->showInfo();
                cout << "Quantity: " << purchases[i]->getQuantity() << endl;
                cout << "----------" << endl;
            }
        }
        if (!found) {
            cout << "No products found for dealer " << did << endl;
        }
    }

    // Display total units sold of a product
    int getProductSalesCount(string pid) {
        int total = 0;
        for (int i = 0; i < purchaseCount; i++) {
            if (purchases[i]->getProduct()->getProductID() == pid) {
                total += purchases[i]->getQuantity();
            }
        }
        return total;
    }

    // Display products purchased by a customer
    void displayCustomerPurchases(string nid) {
        bool found = false;
        for (int i = 0; i < purchaseCount; i++) {
            if (purchases[i]->getCustomer()->getNationalID() == nid) {
                if (!found) {
                    cout << "Products purchased by customer " << nid << ":" << endl;
                    found = true;
                }
                purchases[i]->getProduct()->showInfo();
                cout << "Quantity: " << purchases[i]->getQuantity() << endl;
                cout << "Date: " << purchases[i]->getDate() << endl;
                cout << "----------" << endl;
            }
        }
        if (!found) {
            cout << "No purchases found for customer " << nid << endl;
        }
    }

    // Display sales report for all dealers
    void displayDealerSalesReport() {
        cout << "Dealer Sales Report:" << endl;
        for (int i = 0; i < dealerCount; i++) {
            string did = dealers[i]->getDealerID();
            double total = 0;

            for (int j = 0; j < purchaseCount; j++) {
                if (purchases[j]->getDealer()->getDealerID() == did) {
                    total += purchases[j]->getProduct()->getPrice() * purchases[j]->getQuantity();
                }
            }

            cout << "Dealer: " << did << endl;
            cout << "Total Sales: " << total << endl;
            cout << "----------" << endl;
        }
    }
};

// User interface functions
void displayMenu() {
    cout << "\n========== Store Management System ==========" << endl;
    cout << "1.  Add Product       2.  Remove Product" << endl;
    cout << "3.  Add Customer      4.  Remove Customer" << endl;
    cout << "5.  Add Dealer        6.  Remove Dealer" << endl;
    cout << "7.  Record Purchase   8.  Calculate total purchase amount for a customer"<< endl;
    cout << "9. Display customers who bought a specific product" << endl;
    cout << "10. Display products sold by a dealer" << endl;
    cout << "11. Display total units sold for a product" << endl;
    cout << "12. Display products purchased by a customer" << endl;
    cout << "13. Display sales report for all dealers    14. Exit" << endl;
    cout << "=============================================" << endl;
    cout << "Enter choice(1-14):";
}

int main() {
    ShopDatabase db;
    int choice;
    string input;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline character

        try {
            switch (choice) {
                case 1: { // Add Product
                    string pid, name, brand;
                    double price, weight;

                    cout << "Enter Product ID: ";
                    getline(cin, pid);
                    cout << "Enter Product Name: ";
                    getline(cin, name);
                    cout << "Enter Price: ";
                    cin >> price;
                    cout << "Enter Brand: ";
                    cin.ignore();
                    getline(cin, brand);
                    cout << "Enter Weight: ";
                    cin >> weight;

                    if (db.addProduct(pid, name, price, brand, weight)) {
                        cout << "Product added successfully!" << endl;
                    } else {
                        cout << "Failed to add product (ID might exist or invalid data)" << endl;
                    }
                    break;
                }

                case 2: { // Remove Product
                    string pid;
                    cout << "Enter Product ID to remove: ";
                    getline(cin, pid);

                    if (db.removeProduct(pid)) {
                        cout << "Product removed successfully!" << endl;
                    } else {
                        cout << "Product not found!" << endl;
                    }
                    break;
                }

                case 3: { // Add Customer
                    string fname, lname, nid, gender, cid, prov, city;
                    int byear;

                    cout << "Enter First Name: ";
                    getline(cin, fname);
                    cout << "Enter Last Name: ";
                    getline(cin, lname);
                    cout << "Enter National ID: ";
                    getline(cin, nid);
                    cout << "Enter Birth Year: ";
                    cin >> byear;
                    cout << "Enter Gender: ";
                    cin.ignore();
                    getline(cin, gender);
                    cout << "Enter Customer ID: ";
                    getline(cin, cid);
                    cout << "Enter Province: ";
                    getline(cin, prov);
                    cout << "Enter City: ";
                    getline(cin, city);

                    if (db.addCustomer(fname, lname, nid, byear, gender, cid, prov, city)) {
                        cout << "Customer added successfully!" << endl;
                    } else {
                        cout << "Failed to add customer (National ID might exist)" << endl;
                    }
                    break;
                }

                case 4: { // Remove Customer
                    string nid;
                    cout << "Enter National ID to remove: ";
                    getline(cin, nid);

                    if (db.removeCustomer(nid)) {
                        cout << "Customer removed successfully!" << endl;
                    } else {
                        cout << "Customer not found!" << endl;
                    }
                    break;
                }

                case 5: { // Add Dealer
                    string did, name, ecode, ofname, olname, prov, city;
                    int eyear;

                    cout << "Enter Dealer ID: ";
                    getline(cin, did);
                    cout << "Enter Dealer Name: ";
                    getline(cin, name);
                    cout << "Enter Establishment Year: ";
                    cin >> eyear;
                    cout << "Enter Economic Code: ";
                    cin.ignore();
                    getline(cin, ecode);
                    cout << "Enter Owner First Name: ";
                    getline(cin, ofname);
                    cout << "Enter Owner Last Name: ";
                    getline(cin, olname);
                    cout << "Enter Province: ";
                    getline(cin, prov);
                    cout << "Enter City: ";
                    getline(cin, city);

                    if (db.addDealer(did, name, eyear, ecode, ofname, olname, prov, city)) {
                        cout << "Dealer added successfully!" << endl;
                    } else {
                        cout << "Failed to add dealer (ID might exist)" << endl;
                    }
                    break;
                }

                case 6: { // Remove Dealer
                    string did;
                    cout << "Enter Dealer ID to remove: ";
                    getline(cin, did);

                    if (db.removeDealer(did)) {
                        cout << "Dealer removed successfully!" << endl;
                    } else {
                        cout << "Dealer not found!" << endl;
                    }
                    break;
                }

                case 7: { // Record Purchase
                    string nid, pid, did, date;
                    int qty;

                    cout << "Enter Customer National ID: ";
                    getline(cin, nid);
                    cout << "Enter Product ID: ";
                    getline(cin, pid);
                    cout << "Enter Dealer ID: ";
                    getline(cin, did);
                    cout << "Enter Quantity: ";
                    cin >> qty;
                    cout << "Enter Purchase Date (YYYY/MM/DD): ";
                    cin.ignore();
                    getline(cin, date);

                    if (db.recordPurchase(nid, pid, did, qty, date)) {
                        cout << "Purchase recorded successfully!" << endl;
                    } else {
                        cout << "Failed to record purchase (invalid data or IDs not found)" << endl;
                    }
                    break;
                }

                case 8: { // Customer total purchase
                    string nid;
                    cout << "Enter Customer National ID: ";
                    getline(cin, nid);

                    double total = db.getCustomerTotalPurchase(nid);
                    cout << "Total purchase amount for customer " << nid << ": " << total << endl;
                    break;
                }

                case 9: { // Customers of product
                    string pid;
                    cout << "Enter Product ID: ";
                    getline(cin, pid);

                    db.displayCustomersOfProduct(pid);
                    break;
                }

                case 10: { // Products of dealer
                    string did;
                    cout << "Enter Dealer ID: ";
                    getline(cin, did);

                    db.displayProductsOfDealer(did);
                    break;
                }

                case 11: { // Product sales count
                    string pid;
                    cout << "Enter Product ID: ";
                    getline(cin, pid);

                    int count = db.getProductSalesCount(pid);
                    cout << "Total units sold for product " << pid << ": " << count << endl;
                    break;
                }

                case 12: { // Customer purchases
                    string nid;
                    cout << "Enter Customer National ID: ";
                    getline(cin, nid);

                    db.displayCustomerPurchases(nid);
                    break;
                }

                case 13: { // Dealer sales report
                    db.displayDealerSalesReport();
                    break;
                }

                case 14: // Exit
                    cout << "Exiting program..." << endl;
                    break;

                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } catch (...) {
            cout << "An error occurred. Please check your input and try again." << endl;
        }

    } while (choice != 14);

    return 0;
}