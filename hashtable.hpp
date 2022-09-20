#include "hashtable.h"

    template<typename K, typename V>
    HashTable<K, V>::HashTable(size_t size)
    {
        tableSize = 0;
        vList.resize(prime_below(size));
    }
    //destructor. Delete all elements in hash table.
    template<typename K, typename V>
    HashTable<K, V>::~HashTable()
    {
        makeEmpty();
    }
    //check if key k is in the hash table.
    template<typename K, typename V>
    bool HashTable<K, V>::contains(const K &k) const
    {
        auto &whichList = vList[myhash(k)];
        for(auto &thisList : whichList)
            if(thisList.first == k)
                return true;

        return false;
    }
    //check if key-value pair is in the hash table.
    template<typename K, typename V>
    bool HashTable<K, V>::match(const std::pair<K, V> &kv) const
    {
        auto &whichList = vList[myhash(kv.first)];
        if(whichList.size() > 0)
        {
            for(auto &thisList : whichList)
            {
                if(kv.first == thisList.first)
                    if(kv.second == thisList.second)
                        return true;
            }
            return false;
        } else
            return false;
    }
    //add  the key-value pair kv into the hash table.
    // Don't add if kv is already in the hash table.
    // If the key is the hash table but with a different value,
    // the value should be updated to the new one with kv.
    // Return true if kv is inserted or the value is updated;
    // return false otherwise (i.e., if kv is in the hash table).
    template<typename K, typename V>
    bool HashTable<K, V>::insert(const std::pair<K, V> &kv)
    {
        if(match(kv))
            return false;
        else if(contains(kv.first))
        {
            auto &thisList = vList[myhash(kv.first)];
            for(auto i = thisList.begin(); i != thisList.end(); ++i)
            {
                if (kv.first == i->first)
                {
                    auto temp = thisList.erase(i);
                    thisList.insert(temp, kv);
                }
            }
            return true;
        }
        else
        {
            vList[myhash(kv.first)].push_back(kv);
            if(++tableSize > vList.size())
                rehash();
            return true;
        }
    }
    // move version of insert.
    template<typename K, typename V>
    bool HashTable<K, V>::insert(const std::pair<K, V> &&kv)
    {
        const std::pair<K, V> temp = move(kv);
        return insert(temp);
    }
    //delete the key k and the corresponding value if it is in the hash table.
    // Return true if k is deleted,
    // return false otherwise (i.e., if key k is not in the hash table).
    template<typename K, typename V>
    bool HashTable<K, V>::remove(const K &k)
    {
        bool found = false;
        auto &whichList = vList[myhash(k)];
        int temp = 0;

        for(auto &thisList : whichList)
        {
            if(thisList.first == k)
            {
                auto itr = whichList.begin();
                std::advance(itr, temp);
                whichList.erase(itr);
                --tableSize;
                found = true;
                break;
            }
            temp++;
        }
        return found;
    }
    //delete all elements in the hash table
    template<typename K, typename V>
    void HashTable<K, V>::clear()
    {
        makeEmpty();
    }
    //load the content of the file with name filename into the hash table.
    //In the file, each line contains a single pair of key and value,
    //separated by a white space.
    template<typename K, typename V>
    bool HashTable<K, V>::load(const char *filename)
    {
        std::ifstream is;
        is.open(filename);
        K key;
        V value;

        if(!is)
            return false;

        while(!is.eof())
        {
            is >> key >> value;
            insert(make_pair(key, value));
        }

        is.close();
        return true;
    }
    //display all entries in the hash table.
    // If an entry contains multiple key-value pairs,
    // separate them by a semicolon character (:)
    // (see the provided executable for the exact output format).
    template<typename K, typename V>
    void HashTable<K, V>::dump() const
    {
        int temp = 0;
        for(auto &thisList : vList)
        {
            int temp2 = 0;
            cout << "v[" << temp++ << "]: ";
            for(auto &x : thisList)
            {
                if(++temp2 > 1)
                    cout << " : ";
                cout << x.first << " " << x.second;
            }
            cout << endl;
        }
    }
    //return the number of elements in the hash table.
    template<typename K, typename V>
    size_t HashTable<K, V>::size() const
    {
        return tableSize;
    }
    //write all elements in the hash table into a file with name filename.
    //Similar to the file format in the load function,
    //each line contains a pair of key-value pair, separated by a white space.
    template<typename K, typename V>
    bool HashTable<K, V>::write_to_file(const char *filename) const
    {
        ofstream of;
        of.open(filename);

        if(!of)
            return false;

        for(auto & thisList : vList)
            for(auto & x : thisList)
                of << x.first << " " << x.second << "\n";
            of.close();
            return true;
    }
    //delete all elements in the hash table.
    //The public interface clear() will call this function.
    template<typename K, typename V>
    void HashTable<K, V>::makeEmpty()
    {
        tableSize = 0;
        for(auto &thisList : vList)
            thisList.clear();
    }
    //Rehash function.
    // Called when the number of elements in the hash table is greater..
    //than the size of the vector.
    template<typename K, typename V>
    void HashTable<K, V>::rehash()
    {
        vector<list<pair<K, V>>> temp = vList;
        temp.resize(prime_below(2 * vList.size()));
        for(auto &thisList : vList)
            thisList.clear();

        tableSize = 0;
        for(auto & thisList : temp)
            for(auto & tList : thisList)
                insert(move(tList));
    }
    //return the index of the vector entry where k should be stored.
    template<typename K, typename V>
    size_t HashTable<K, V>::myhash(const K &k) const
    {
        static hash<K> indexK;
        return indexK(k) % vList.size();
    }
    // returns largest prime number <= n or zero if input is too large
    // This is likely to be more efficient than prime_above(), because
    // it only needs a vector of size n
    template <typename K, typename V>
    unsigned long HashTable<K, V>::prime_below(unsigned long n)
    {
        if (n > max_prime)
        {
            std::cerr << "** input too large for prime_below()\n";
            return 0;
        }
        if (n == max_prime)
        {
            return max_prime;
        }
        if (n <= 1)
        {
            std::cerr << "** input too small \n";
            return 0;
        }

        // now: 2 <= n < max_prime
        std::vector <unsigned long> v (n+1);
        setPrimes(v);
        while (n > 2)
        {
            if (v[n] == 1)
                return n;
            --n;
        }

        return 2;
    }
    //Sets all prime number indexes to 1. Called by method prime_below(n)
    template <typename K, typename V>
    void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
    {
        int i = 0;
        int j = 0;

        vprimes[0] = 0;
        vprimes[1] = 0;
        int n = vprimes.capacity();

        for (i = 2; i < n; ++i)
            vprimes[i] = 1;

        for( i = 2; i*i < n; ++i)
        {
            if (vprimes[i] == 1)
                for(j = i + i ; j < n; j += i)
                    vprimes[j] = 0;
        }
    }
