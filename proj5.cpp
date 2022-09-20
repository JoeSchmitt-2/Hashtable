#include "hashtable.h"
#include "passserver.h"
using namespace cop4530;
using namespace std;

    void Menu()
    {
        cout << "\n\n";
        cout << "l - Load From File" << endl;
        cout << "a - Add User" << endl;
        cout << "r - Remove User" << endl;
        cout << "c - Change User Password" << endl;
        cout << "f - Find User" << endl;
        cout << "d - Dump HashTable" << endl;
        cout << "s - HashTable Size" << endl;
        cout << "w - Write to Password File" << endl;
        cout << "x - Exit program" << endl;
        cout << "\nEnter choice : ";
    }

    int main()
    {
        int capacity;
        bool exitMenu = false;

        cout << "Enter preferred hash table capacity: ";
        cin >> capacity;

        PassServer ps1(capacity);

        if(capacity < 2)
        {
            cout << "** input too small" << endl;
            cout << "set to default capacity" << endl;
            capacity = default_capacity;
            cout << capacity;
            PassServer ps1(capacity);
        }


        while(!exitMenu)
        {
            string fileIn, fileOut, username, password, newPassword;
            char input;

            Menu();
            cin >> input;

            switch(input)
            {
                case 'l':
                {
                    cout << "Enter password file name to load from:";
                    cin >> fileIn;
                    if(!ps1.load(fileIn.c_str()))
                        cout << "\nError: cannot open file" << fileIn << endl;
                    break;
                }
                case 'a':
                {
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    pair<string, string> newPair(username,password);
                    if(ps1.addUser(newPair))
                        cout << "\nUser " << username << " added." << endl;
                    else
                        cout << "*****Error: User already exists. Could not add user." << endl;
                    break;
                }
                case 'r':
                {
                    cout << "Enter username: ";
                    cin >> username;

                    if(ps1.removeUser(username))
                        cout << "\nUser " << username << " deleted." << endl;
                    else
                        cout << "*****Error: User not found. Could not delete user" << endl;
                    break;
                }
                case 'c':
                {
                    cout << "Enter username: ";
                    cin >> username;

                    cout << "\nEnter password: ";
                    cin >> password;

                    cout << "\nEnter new password: ";
                    cin >> newPassword;

                    if(ps1.changePassword(make_pair(username, password), newPassword))
                        cout << "\nPassword changed for user " << username;
                    else
                        cout << "*****Error: Could not change user password" << endl;
                    break;
                }
                case 'f':
                {
                    cout << "Enter username: ";
                    cin >> username;

                    if(ps1.find(username))
                        cout << "User '" << username << "' found." << endl;
                    else
                        cout << "User '" << username << "' not found." << endl;
                    break;
                }
                case 'd':
                {
                    ps1.dump();
                    break;
                }
                case 's':
                {
                    cout << "Size of hashtable: " << ps1.size() << endl;
                    break;
                }
                case 'w':
                {
                    cout << "Enter password file name to write to: ";
                    cin >> fileOut;
                    if(!ps1.write_to_file(fileOut.c_str()))
                        cout << "\nError: Could not write to file" << endl;
                    break;
                }
                case 'x':
                {
                    exitMenu = true; break;
                }
                default :
                {
                    cout << "*****Error: Invalid entry. Try again."; break;
                }
            }
        }
        return 0;
    }
