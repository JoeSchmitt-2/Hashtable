#include "passserver.h"
//constructor, create a hash table of the specified size.
// You just need to pass this size parameter to the constructor of the HashTable.
// Therefore, the real hash table size could be different from the parameter
// size (because prime_below() will be called in
// the constructor of the HashTable).
PassServer::PassServer(size_t size)
{
    HashTable<string, string> ps(size);
}
//destructor. You need to decide what you should do based
// on your design of PassServer (how you develop the adaptor
// class based on the adaptee HashTable). In essence,
// we do not want to have memory leak.
PassServer::~PassServer()
{
    ps.clear();
}
//load a password file into the HashTable object.
// Each line contains a pair of username and encrypted password.
bool PassServer::load(const char *filename)
{
    return ps.load(filename);
}
//add a new username and password.
// The password passed in is in plaintext, it should be
// encrypted before insertion. The pair should not be added
// if the username already exists in the hash table.
bool PassServer::addUser(pair<string, string> &kv)
{
    return ps.insert(make_pair(kv.first,encrypt(kv.second)));
}
//move version of addUser.
bool PassServer::addUser(pair<string, string> &&kv)
{
    return ps.insert(move(make_pair(kv.first,encrypt(kv.second))));
}
//delete an existing user with username k.
bool PassServer::removeUser(const string &k)
{
    return ps.remove(k);
}
//check if a user exists (if user is in the hash table).
bool PassServer::find(const string &user) const
{
    return ps.contains(user);
}
//show the structure and contents of the HashTable object to the screen.
//Same format as the dump() function in the HashTable class template.
void PassServer::dump()
{
    ps.dump();
}
//return the size of the HashTable
//(the number of username/password pairs in the table).
size_t PassServer::size() const
{
    return ps.size();
}
// save the username and password combination into a file.
// Same format as the write_to_file() function in the HashTable class template.
bool PassServer::write_to_file(const char *filename) const
{
    return ps.write_to_file(filename);
}

string PassServer::encrypt(const string &str)
{
    char salt[] = "$1$########";
    char *password = new char [100];
    strcpy(password, str.c_str());
    string newPassword = crypt(password, salt);
    return newPassword.substr(newPassword.find_last_of("$") + 1);
}
//change an existing user's password.
// Note that both passwords passed in are in plaintext.
// They should be encrypted before you interact with the hash table.
// If the user is not in the hash table, return false.
// If p.second does not match the current password, return false.
// Also return false if the new password and the old password are
// the same (i.e., we cannot update the password).
bool PassServer::changePassword(const pair<string, string> &p,
        const string &newPassword)
{
    if(ps.match(make_pair(p.first, encrypt(p.second))) == true
    && encrypt(p.second) != encrypt(newPassword))
    {
        return true;
    }
    else
        return false;
}
