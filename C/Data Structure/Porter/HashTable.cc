/*********************************************************************
  Description:   Implementation of a hash table.
**********************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;

#include "HashTable.h"

// Constructor with given initial capacity.
HashTable::HashTable(int size)
    : storage(size)
{
    capacity = size;
    numObjects = 0;
} // HashTable::HashTable()

// Destructor: call the function makeEmpty instead, then deallocate
// the memory inside the storage vector
HashTable::~HashTable()
{
    makeEmpty();
    storage.clear();
} // HashTable::~HashTable()

// Delete all the objects without deleting the storage.
void HashTable::makeEmpty()
{
    for(int i=0; i<capacity; i++)
        storage[i].clear(); // delete all the nodes in the list
    numObjects = 0;
} // HashTable::makeEmpty()

// Checks if the access to a given position is safe in the
// storage. This means if the position is smaller than the capacity
// and non negative.
bool HashTable::isValid(int position)
{
    return (position >= 0 && position < capacity);
}

// Prints all the objects in the table.
void HashTable::print()
{
    ListIter iter;
    int i;
    
    cout << "The table has " << numObjects << " objects." << endl;
    if (numObjects) 
    {
        cout << "Here are the objects:" << endl;
        for (i = 0; i < capacity; i++)
            if (storage[i].size()) 
                // traverse the list at index i
                for (iter=storage[i].begin(); iter!=storage[i].end(); ++iter)
                    cout << *iter << endl;
    }
} // HashTable::print()

// Resizes the table and rehashes it, moving each stored element
// at the index corresponding to the resized hash function.
void HashTable::resize(int newSize)
{
    int i, newIndex;
    ListIter iter;
    ListWordC save;
    WordCounter wordc;

    if (capacity == newSize)
        return;

    // save the elements from the old storage to the save list
    for (i = 0; i < capacity; i++)
    {
        // splice concatenates storage[i] to save at the end
        // and deletes the content of storage[i]
        save.splice(save.end(), storage[i]);
    }

    // vector's resize does the job
    storage.resize(newSize);
    capacity = newSize;

    // now rehash everything
    for (iter=save.begin(); iter!=save.end(); ++iter)
    {
        wordc = *iter;                  // save it
        newIndex = hashing(key(wordc)); // based on new size
        storage[newIndex].push_back(wordc);
    }
    save.clear(); // delete the saved list
} // HashTable::resize(int newSize)

// This private function inserts an object in the table when we
// already know what position we want it to be inserted at. It is
// declared as private because the user should not be able to call
// it. It is designed to be called internally by the public insertion
// function after getting the position from the hashing table.
bool HashTable::insert(WordCounter &wordc, int position)
{
    if (!isValid(position)) 
    {
        cerr << "Invalid position in insert: " << position << endl;
        return false;
    }
    else {
        storage[position].push_back(wordc);
        numObjects++;
        return true;
    }
} // HashTable::insert()

// This private function removes the word counter object from the list
// at the given position. It is also intended to be called internally
// because public remove function should determine the position based
// on the hash function before it calls this one. They key is used to
// locate the object which is copied to the object wordc before it is
// erased.
bool HashTable::remove(WordCounter &wordc, string k, int position)
{
    if (!isValid(position))
        return false;
    else 
    {
        ListIter iter = storage[position].begin();
        while (iter != storage[position].end() && 
               key(*iter) != k)
            ++iter;
        if (iter == storage[position].end())
            return false;
        else 
        {
            wordc = *iter;
            storage[position].erase(iter);
            numObjects--;
            return true;
        }
    }
} // HashTable::remove()

// Private functions to be implemented by the student.

// This private function should find if the string k has been used as
// a key to insert an object in the list at the given position. If
// yes, then it should copy that object into the object wordc and
// return true. If not, it should return false.
bool HashTable::find(WordCounter &wordc, string k, int position)
{
    if (!isValid(position))
        return false;
    else
    {
        ListIter iter = storage[position].begin(); //set the iter to current position
        
        if (key(*iter) == k)    // check strings for equality
        {
            wordc = *iter;  // copy the value
            wordc.increment();  // increment the duplicate word count
            return true;
        }
            
        else
            return false;
    }
} // HashTable::find()

// The most important function of the class. This function takes a
// string (a key) and determines the index in the storage array where
// an object with that key should go.
int HashTable::hashing(string s)
{
    /********** First Hashing test (good hash) *******/
    int hash_val = 0;
    for (int i=0; s[i]!='\0'; i++)
        hash_val = (hash_val*37 + int(s[i])) % capacity;
    return hash_val;// Hashing 1
    
    /********** Second Hashing test (pretty bad hash) *
    int hashVal = 0;
    for (int i = 0; s[i]!='\0'; ++i)
        hashVal += (5 * i + 3) * int(s[i]);
    return hashVal % capacity;*/// Hashing 2
 
    /********** Third Hashing test  (terrible hash) *****

     int hash_val = 0; 
     for (int i=0; s[i]!='\0'; i++)
        hash_val += int(s[i]);
     return hash_val % capacity;*/// Hashing 3
    
} // HashTable::hashing()


// This function should first get the hash index for the key of that
// object, then find out if the key has been inserted in the list at
// that index, and if not, then it should insert the object in that
// list and return true. If the key has already been used in the
// table, it should return false.
bool HashTable::insert(WordCounter &wordc)
{
    string s = key(wordc); // convert wordc to pure string for hash
    int hashI = hashing(s);
    
    if (!find(wordc,s,hashI))   // see if key has been used before
    {
        insert(wordc,hashI);
        return true;
    }
    else
        return false;
} // HashTable::insert()

// This function should first get the hash index for the string, then
// find out if the string has been used as a key to insert an object in
// the list at that index. If yes, it should copy that object into the
// object wordc and return true, and if not, it should just return false.
bool HashTable::access(WordCounter &wordc, string s)
{
    int hashI = hashing(s);
    
    if (find(wordc,s,hashI))
    {
        insert(wordc,hashI);
        return true;
    }
    else
        return false;
} // HashTable::access()

// This function should first get the hash index for the string, then
// find out if the string has been used as a key to insert an object in
// the list at that index. If yes, it should copy that object into the
// object wordc, remove the object from that list, and return true. If
// not, it should just return false.
bool HashTable::remove(WordCounter &wordc, string s)
{
    int hashI = hashing(s);
    
    if (find(wordc,s,hashI))    // see if key has been used
    {
        ListIter iter = storage[hashI].begin(); // create iter to copy into wordc
        wordc = *iter;
        remove(wordc,s,hashI);  // now perform remove
        return true;
    }
    else
        return false;
} // HashTable::remove()

// This function should print a statistic of usage of each index in
// the table (number of objects stored at each index).  Note: you
// should only need one loop in this function, and no iterator. Use
// the method size() for each element of the vector to print the
// number of nodes in that list.
void HashTable::statistic()
{
    cout << "The table has " << numObjects << " objects." << endl;
    if (numObjects)
    {
        cout << "Here are the objects:" << endl;
        for (int i = 0; i < capacity; i++)
            cout << "Index: " << i << " Usage: " << storage[i].size() << endl;
    }
} // HashTable::statistic()
