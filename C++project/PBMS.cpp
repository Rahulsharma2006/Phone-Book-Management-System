#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

//----------------------------------------------------------
// Base Class: Contact
//----------------------------------------------------------
class Contact {
protected:
    string name;
    string phone;
    static int totalContacts;   // static data member

public:
    // Constructors
    Contact() { totalContacts++; }
    Contact(string n, string p) : name(n), phone(p) { totalContacts++; }

    // Destructor
    virtual ~Contact() { totalContacts--; }

    // Getter functions
    string getName() const { return name; }
    string getPhone() const { return phone; }

    // Virtual function (for polymorphism)
    virtual void display() const {
        cout << left << setw(20) << name
             << setw(15) << phone << endl;
    }

    // Overloaded operator
    bool operator==(const Contact &c) const {
        return name == c.name;
    }

    // Static function
    static int getTotalContacts() {
        return totalContacts;
    }

    // Friend function (can access private/protected)
    friend void showContactDetails(const Contact &c);
};

// Definition of static member
int Contact::totalContacts = 0;

// Friend function definition
void showContactDetails(const Contact &c) {
    cout << "\n--- Contact Details ---\n";
    cout << "Name: " << c.name << "\nPhone: " << c.phone << endl;
}

//----------------------------------------------------------
// Derived Class: BusinessContact (Inheritance + Overriding)
//----------------------------------------------------------
class BusinessContact : public Contact {
    string company;
public:
    BusinessContact(string n, string p, string comp)
        : Contact(n, p), company(comp) {}

    void display() const override {
        cout << left << setw(20) << name
             << setw(15) << phone
             << setw(20) << company << endl;
    }
};

//----------------------------------------------------------
// Template function for searching
//----------------------------------------------------------
template <typename T>
int searchContact(const vector<T *> &contacts, const string &key) {
    for (size_t i = 0; i < contacts.size(); ++i) {
        if (contacts[i]->getName() == key)
            return static_cast<int>(i);
    }
    return -1;
}

//----------------------------------------------------------
// Exception Class
//----------------------------------------------------------
class FileException : public exception {
public:
    const char *what() const noexcept override {
        return "File could not be opened!";
    }
};

//----------------------------------------------------------
// Class: PhoneBook
//----------------------------------------------------------
class PhoneBook {
    vector<Contact *> contacts;  // Dynamic memory used (vector of pointers)

public:
    // Add Contact
    void addContact() {
        int type;
        cout << "\n1. Personal Contact\n2. Business Contact\nEnter type: ";
        cin >> type;
        cin.ignore();

        string name, phone, company;
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Phone: ";
        getline(cin, phone);

        if (type == 2) {
            cout << "Enter Company: ";
            getline(cin, company);
            contacts.push_back(new BusinessContact(name, phone, company));
        } else {
            contacts.push_back(new Contact(name, phone));
        }

        cout << "\n Contact added successfully!\n";
    }

    // Display all
    void displayAll() const {
        cout << "\n---------------------------------------------\n";
        cout << left << setw(20) << "Name"
             << setw(15) << "Phone"
             << setw(20) << "Company" << endl;
        cout << "---------------------------------------------\n";
        for (auto c : contacts)
            c->display();
        cout << "---------------------------------------------\n";
        cout << "Total Contacts: " << Contact::getTotalContacts() << endl;
    }

    // Search by name
    void searchByName() {
        string key;
        cin.ignore();
        cout << "Enter name to search: ";
        getline(cin, key);

        int index = searchContact(contacts, key);
        if (index != -1) {
            cout << "\nContact Found!\n";
            showContactDetails(*contacts[index]);
        } else {
            cout << " Contact not found!\n";
        }
    }

    // Delete contact
    void deleteContact() {
        string key;
        cin.ignore();
        cout << "Enter name to delete: ";
        getline(cin, key);

        int index = searchContact(contacts, key);
        if (index != -1) {
            delete contacts[index];
            contacts.erase(contacts.begin() + index);
            cout << " Contact deleted successfully!\n";
        } else {
            cout << " Contact not found!\n";
        }
    }

    // Save to File (File Handling + Exception Handling)
    void saveToFile() {
        ofstream fout("phonebook.txt");
        if (!fout) throw FileException();

        for (auto c : contacts) {
            fout << c->getName() << "," << c->getPhone() << endl;
        }
        fout.close();
        cout << " Contacts saved successfully!\n";
    }

    // Load from File
    void loadFromFile() {
        ifstream fin("phonebook.txt");
        if (!fin) throw FileException();

        string line;
        while (getline(fin, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string n = line.substr(0, pos);
                string p = line.substr(pos + 1);
                contacts.push_back(new Contact(n, p));
            }
        }
        fin.close();
        cout << " Contacts loaded successfully!\n";
    }

    // Destructor (release dynamic memory)
    ~PhoneBook() {
        for (auto c : contacts)
            delete c;
        contacts.clear();
    }
};
/***** MAIN FUNCTION ******/
int main() {
    PhoneBook pb;
    int choice;

    cout << "\n==============================================";
    cout << "\n     PHONE BOOK MANAGEMENT SYSTEM (C++)";
    cout << "\n==============================================\n";

    // Preload contacts from file
    try {
        pb.loadFromFile();
    } catch (FileException &e) {
        cout << " " << e.what() << endl;
    }

    while (true) {
        cout << "\n------------ MAIN MENU ------------";
        cout << "\n1. Add Contact";
        cout << "\n2. Display All Contacts";
        cout << "\n3. Search Contact";
        cout << "\n4. Delete Contact";
        cout << "\n5. Save to File";
        cout << "\n6. Exit";
        cout << "\n-----------------------------------";
        cout << "\nEnter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1: pb.addContact(); break;
            case 2: pb.displayAll(); break;
            case 3: pb.searchByName(); break;
            case 4: pb.deleteContact(); break;
            case 5: pb.saveToFile(); break;
            case 6:
                cout << "\n Exiting program... Goodbye!\n";
                return 0;
            default:
                cout << " Invalid choice! Please try again.\n";
            }
        } catch (FileException &e) {
            cout << " " << e.what() << endl;
        } catch (...) {
            cout << " Unknown error occurred.\n";
        }
    }
}