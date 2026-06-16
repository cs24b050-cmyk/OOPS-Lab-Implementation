#include <iostream>
#include <exception>
using namespace std;

// Exception Classes
class WarehouseException: public exception {
    const char* err_msg;
public:
    WarehouseException(const char* msg) : err_msg(msg) {}
    const char* what() const noexcept override {
        return err_msg;
    }
};

class PowerFailure : public WarehouseException {
public:
    PowerFailure() : WarehouseException("ERR_POWER") {}
};

class WarehouseFull : public WarehouseException {
public:
    WarehouseFull() : WarehouseException("ERR_FULL") {}
};

class Overloaded : public WarehouseException {
public:
    Overloaded() : WarehouseException("ERR_WEIGHT") {}
};

class OutofStock : public WarehouseException {
public:
    OutofStock() : WarehouseException("ERR_EMPTY") {}
};

// Box template structure
template <typename T>
struct Box {
    T data;
    int weight;
    
    Box() : data(T()), weight(0) {}
    Box(T d, int w) : data(d), weight(w) {}
};

// Storage class - manual stack implementation using dynamic array
template <typename T>
class Storage {
private:
    Box<T>* arr;
    int capacity;
    int size;
    
    void resize() {
        int newCapacity = capacity * 2;
        Box<T>* newArr = new Box<T>[newCapacity];
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }
    
public:
    Storage() : capacity(2), size(0) {
        arr = new Box<T>[capacity];
    }
    
    ~Storage() {
        delete[] arr;
    }
    
    void push(T item, int weight) {
        if (size == capacity) {
            resize();
        }
        arr[size++] = Box<T>(item, weight);
    }
    
    Box<T> pop() {
        if (size == 0) {
            throw OutofStock();
        }
        return arr[--size];
    }
    
    int currentSize() {
        return size;
    }
};

// Warehouse class
template <typename T>
class Warehouse {
private:
    Storage<T> storage;
    int maxCapacity;
    int maxWeight;
    int currentEnergy;
    int currentWeight;
    
public:
    Warehouse(int cap, int maxWt, int energy) 
        : maxCapacity(cap), maxWeight(maxWt), currentEnergy(energy), currentWeight(0) {}
    
    void receive(T data, int weight) {
        // Check capacity first (WarehouseFull takes precedence over Overloaded)
        if (storage.currentSize() >= maxCapacity) {
            throw WarehouseFull();
        }
        // Check weight limit
        if (currentWeight + weight > maxWeight) {
            throw Overloaded();
        }
        // Push item and update weight
        storage.push(data, weight);
        currentWeight += weight;
    }
    
    void dispatch() {
        // Step 1: Check energy first
        if (currentEnergy < 2) {
            throw PowerFailure();
        }
        // Step 2: Check inventory
        if (storage.currentSize() == 0) {
            throw OutofStock();
        }
        // Step 3: Execute
        currentEnergy -= 2;
        Box<T> box = storage.pop();
        currentWeight -= box.weight;
        cout<< box.data <<endl;
    }
    
    void recharge(int value) {
        currentEnergy += value;
    }
    
    void bulkDispatch(int k) {
        // Save initial state for rollback
        int initialEnergy = currentEnergy;
        int initialWeight = currentWeight;
        
        // Temporary buffer to store removed items for potential rollback
        Box<T>* buffer = new Box<T>[k];
        int removedCount = 0;
        
        try {
            for (int i = 0; i < k; i++) {
                // Step 1: Check energy first
                if (currentEnergy < 2) {
                    throw PowerFailure();
                }
                // Step 2 & 3: Pop item (will throw OutofStock if empty)
                currentEnergy -= 2;
                Box<T> box = storage.pop();
                currentWeight -= box.weight;
                buffer[removedCount++] = box;
            }
            
            // Success - print all removed items in order they were removed
            for (int i = 0; i < removedCount; i++) {
                cout << buffer[i].data << endl;
            }
            
            delete[] buffer;
            
        } catch (...) {
            // Rollback: push all removed items back in reverse order
            for (int i = removedCount - 1; i >= 0; i--) {
                storage.push(buffer[i].data, buffer[i].weight);
            }
            
            // Restore energy and weight
            currentEnergy = initialEnergy;
            currentWeight = initialWeight;
            
            delete[] buffer;
            
            // Re-throw the exception
            throw;
        }
    }
};

template <typename T>
class StartProcess {
    Warehouse<T> warehouse;
public:
    StartProcess(int capacity, int maxWeight, int initialEnergy)
      : warehouse(capacity, maxWeight, initialEnergy) {}

    void run(int N) {
        for (int i = 0; i < N; ++i) {
            int cmd;
            if (!(cin >> cmd)) return;
            try {
                if (cmd == 1) {
                    T data; int w;
                    cin >> data >> w;
                    warehouse.receive(data, w);
                } else if (cmd == 2) {
                    warehouse.dispatch();
                } else if (cmd == 3) {
                    int v; cin >> v;
                    warehouse.recharge(v);
                } else if (cmd == 4) {
                    int k; cin >> k;
                    warehouse.bulkDispatch(k);
                }
            } catch (const WarehouseException& e) {
                cout << e.what() << endl;
            }
        }
    }
};

int main() {
    int capacity, maxWeight, initialEnergy;
    char dataType;
    cin >> capacity >> maxWeight >> initialEnergy >> dataType;
    int N; cin >> N;

    if (dataType == 'I') {
        StartProcess<int> process(capacity, maxWeight, initialEnergy);
        process.run(N);
    } else if (dataType == 'C') {
        StartProcess<char> process(capacity, maxWeight, initialEnergy);
        process.run(N);
    } else if (dataType == 'F') {
        StartProcess<float> process(capacity, maxWeight, initialEnergy);
        process.run(N);
    }
    return 0;
}