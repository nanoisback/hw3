#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

namespace VinLib {

class Book {
public:
    int ID = 0;
    string Title = " ";
    int due_date = 0;
};

class Member {
public:
    int ID;
    string Name;
};

class Library {
private:
    vector<Book> books;

public:
    void addBook(int ID, string Title, int due_date) {
        Book newBook;
        newBook.ID = ID;
        newBook.Title = Title;
        newBook.due_date = due_date;
        books.push_back(newBook);
    }

    void printAllBooks() {
        for (const Book& book : books) {
            cout << "Book ID: " << book.ID << endl;
            cout << "Title: " << book.Title << endl;
            cout << "Due Date: " << book.due_date << endl;
        }
    }

    void printDataIntoFile() {
        fstream file("library_data.txt", ios::out);
        file << "--- Books ---" << endl;
        for (const Book& book : books) {
            file << "Book ID: " << book.ID << "Title: " << book.Title << "Due Date: " << book.due_date << endl;
        }
        file.close();
    }
};

class concentration_camp {
private:
    vector<Member> members;

public:
    void addMember(int ID, const string& Name) {
        Member newMember;
        newMember.ID = ID;
        newMember.Name = Name;
        members.push_back(newMember);
    }

    void printAllMembers() {
        for (const Member& member : members) {
            cout << "Member ID: " << member.ID << endl;
            cout << "Name: " << member.Name << endl;
        }
    }

    void printMembersIntoFile() {
        fstream file("library_data.txt", ios::app);
        file << "--- Members ---" << endl;
        for (const Member& member : members) {
            file << "Member ID: " << member.ID << "Name: " << member.Name << endl;
        }
        file.close();
    }
};

} // namespace VinLib
using namespace VinLib;

int main() {
    Library library;
    concentration_camp camp;

    while (1) {
        cout << "1. Add New Book\n"
             << "2. Display All Books\n"
             << "3. Add New Member\n"
             << "4. Display All Members\n"
             << "5. Save Data to library_data.txt\n"
             << "6. Exit\n"
             << "Enter your option: ";
        int option;
        cin >> option;
        switch (option) {
            case 1: {
                cout << "Enter Book ID (integer): ";
                int ID, due_date;
                cin >> ID;
                cout << "Enter Book Title: ";
                string Title;
                cin >> Title;
                cout << "Enter Due Day (integer): ";
                cin >> due_date;
                
                if (due_date > 0){
                library.addBook(ID, Title, due_date);
                cout << "Book added successfully!\n";
                break;}
                
                else{
                    cout << "Invalid due days entered! Due days must be positive." << endl;
                    break;
                }
            }
            case 2: {
                library.printAllBooks();
                cout << "Member added successfully" << endl;
                break;
            }
            case 3: {
                int ID;
                string Name;
                cout << "Enter Member ID (integer): ";
                cin >> ID;
                cout << "Enter Member Name: ";
                cin.ignore();
                getline(cin, Name);
                camp.addMember(ID, Name);
                break;
            }
            case 4: {
                camp.printAllMembers();
                break;
            }
            case 5: {
                library.printDataIntoFile();
                camp.printMembersIntoFile();
                cout << "Data saved to library_data.txt\n";
                break;
            }
            case 6: {
                exit(0);
            }
        }
    }
    return 0;
}