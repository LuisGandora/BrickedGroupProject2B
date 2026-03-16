#ifndef LRUCACHE_H
#define LRUCACHE_H
#include <iostream>
using namespace std;
//One other issue, we need to restrucure public so certain filenames like index.html dont break the app
//Desperately Need to test this
class LRUCache{
public:
    //Custom one to handle the case of 3
    
    struct LRUNode
    {
        string data; //Will represent the filepath saved for download path
        LRUNode* next;
        LRUNode* prev;

        LRUNode(string _data = "")
        {
            data = _data;
            next = nullptr;
            prev = nullptr;
        }

    };
    //Still use LRU but uses the typeT for better access Ig
    struct basicHashingMap
    {
        LRUNode** arr;
        int curr_size;
        int max_cap;
        basicHashingMap() //Seperate into another initilization function for workflow
        {
            max_cap = 0;
            curr_size = 0;
            arr = nullptr;
            
        }  
        void initialize(int size)
        {
            max_cap = size;
            arr = new LRUNode*[size];
            for(int i = 0; i < size; i++)
            {
                arr[i] = nullptr;
            }
        }
        ~basicHashingMap()
        {
            delete[] arr;
        }
        int hashThatThang(LRUNode* node)
        {
            return node->data.size() % max_cap;
        }
        //Insert and remove avg: O(1) worst: O(n)
        //No need to worry if the capacity is full, the LRUCache itself will clear out free space to insert
        void insert(LRUNode* node)
        {
            int key = hashThatThang(node);
            if(arr[key] == nullptr)
            {
                arr[key] = node;
                curr_size++;
                return;
            }
            else
            {
                int tmp = key+1;
                while(tmp != key)
                {
                    if(tmp % max_cap == 0)
                    {
                        tmp = 0;
                    }
                    if(arr[tmp] == nullptr)
                    {
                        arr[tmp] = node;
                        curr_size++;
                        return;
                    }
                    tmp++;
                }
            }
            throw std::runtime_error("LRU Cache: hashing map was not empty upon full insertion!");
        }
        //meant to be called BEFORE node is deleted
        int removal(LRUNode* node)
        {
            int key = hashThatThang(node);
            if(arr[key]->data == node->data)
            {
                arr[key] = nullptr;
                curr_size--;
                return key;
            }
            else
            {
                int tmp = key+1;
                while(tmp != key)
                {
                    if(tmp % max_cap == 0)
                    {
                        tmp = 0;
                    }
                    if(arr[tmp]->data == node->data)
                    {
                        arr[tmp] = nullptr;
                        curr_size--;
                        return tmp;
                    }
                    tmp++;
                }
            }
            return -1;//The node doesnt exist
        }
    };
    //also add some kind of indicator for has map later
    int size;
    int capacity;
    basicHashingMap hash_map;
    LRUNode* head;
    LRUNode* tail;

    LRUCache()
    {
        capacity = 3; //max size
        size = 0;
        hash_map.initialize(capacity);
        head = nullptr;
        tail = nullptr;
    }

    ~LRUCache()
    {
        LRUNode* temp = head;
        while(temp != nullptr)
        {
            head = temp->next;
            delete temp;
            temp = head;
        }
        tail = nullptr;
    }

    //get key and push to front of linked list (access based on index access in webpage (will be pushed to front))
    string get(int key) //should be 0-2?
    {
        LRUNode* temp = hash_map.arr[key];
        if(temp == tail) // Get value and change correctly
        {
            tail = tail->prev;
            tail->next = nullptr;
            temp->prev = nullptr;
            temp->next = head;
            head->prev = temp;
            head = temp; 
        }
        else if(head != temp)//case where its in the middle
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            temp->next = head;
            head->prev = temp;
            head= temp; //should work??? Test later
            
        }
        return temp->data;
    }

    //Insert the filepath of the already made return files (from main.cpp) (Insert in front)
    void insert(string filePath)
    {
        //Hashing function is the size of name
        LRUNode* temp = new LRUNode(filePath);
        if(head == nullptr && tail == nullptr)
        {
            head = temp;
            tail = temp;
            hash_map.insert(temp);
            size++;
            return;
        }
        if(size == capacity)
        {
            hash_map.removal(tail);
            //replace tail
            LRUNode* prevReplace = tail->prev;
            delete tail;
            prevReplace->next = nullptr;
            tail = prevReplace;
            size--;
        }
        //should increment correctly
        temp->next = head;
        head->prev = temp;
        head = temp;
        hash_map.insert(temp); //same address
        size++;
       
        //Might need to delete
        
    }

    //get least (tail) and remove from the list
    void removeLeastPriority()
    {
        if(tail == nullptr)
        {
            return;
        }
        if(tail == head)
        {
            delete head;
            delete tail;
            head = nullptr;
            tail = nullptr;
            return;
        }
        hash_map.removal(tail);
        LRUNode* prevReplace = tail->prev;
        prevReplace->next = nullptr;
        delete tail;
        tail = prevReplace;
        size--;
    }
    //for debugging
    void traverse()
    {
        LRUNode* temp = head;
        cout<<"Linked List: ";
        while(temp != nullptr)
        {
            cout<< temp->data << " ";
            temp = temp->next;
        }
        cout << '\n';
        cout << "Map: ";
        for(int i = 0; i < hash_map.curr_size; i++)
        {
            if(hash_map.arr[i] != nullptr)
            {
                cout << (hash_map.arr[i])->data << " ";
            }
            else
            {
                cout << "nullptr here ";
            }
            
        }
        cout << '\n';

    }
    
};
#endif

//Testing Code
// #include "LRUCache.h"
// #include <iostream>
// using namespace std;

// int main()
// {
//     LRUCache hashStuff;
//     cout <<"Passed Initialization test"<<endl;
//     hashStuff.insert("filePath1");
//     cout <<"Passed insert 1 test"<<endl;
//     hashStuff.insert("filePath2");
//     cout <<"Passed insert 2 test"<<endl;
//     hashStuff.insert("filePath3");
//     cout <<"Passed insert 3 test"<<endl;
//     hashStuff.traverse();
//     hashStuff.insert("filePath4");
//     cout <<"Passed insert 4 test"<<endl;
//     hashStuff.traverse();
//     cout << hashStuff.get(0) << endl;
//     hashStuff.traverse();
//     cout << hashStuff.get(1) << endl;
//     hashStuff.traverse();
//     cout << hashStuff.get(2) << endl;
//     hashStuff.traverse();
// }