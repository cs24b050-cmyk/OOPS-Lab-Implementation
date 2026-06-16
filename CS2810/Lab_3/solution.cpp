#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// ================= PART 1: GLOBAL FUNCTIONS =================

// 1.1 Linear Search
template <typename T>
int linearSearch(T* arr, int size, T key) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}

// 1.2 Binary Search (Assumes sorted)
template <typename T>
int binarySearch(T* arr, int size, T key) {
    int left = 0;
    int right = size - 1;
    int ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == key) {
            ans = mid;
        }
        if (arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

// 1.3 Sort (Bubble sort for simplicity, O(N^2) is allowed per constraints)
template <typename T>
void sort(T* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// ================= PART 2: CRATE CLASS =================

template <typename T>
class Crate {
private:
    T* container;
    int size;
    int capacity;

    void resize(int newCapacity) {
        T* newContainer = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newContainer[i] = container[i];
        }
        delete[] container;
        container = newContainer;
        capacity = newCapacity;
    }

public:
    Crate() {
        size = 0;
        capacity = 2;
        container = new T[capacity];
    }

    // ~Crate() {
    //     delete[] container;
    // }

    void add(T item) {
        // Expansion Rule: if size == capacity / 2, double capacity
        if (size == capacity / 2) {
            resize(capacity * 2);
        }
        container[size] = item;
        size++;
    }

    void remove() {
        if (size == 0) {
            cout << "Error: Crate is empty" << endl;
            return;
        }

        // Find heaviest (largest) element index
        int maxIndex = 0;
        for (int i = 1; i < size; i++) {
            if (container[i] > container[maxIndex]) {
                maxIndex = i;
            }
        }

        // Print the item being removed
        cout << container[maxIndex] << endl;

        // Shift elements to fill the gap
        for (int i = maxIndex; i < size - 1; i++) {
            container[i] = container[i + 1];
        }
        size--;

        // Contraction Rule: if size < capacity / 4 AND capacity > 2, halve capacity
        if (size < capacity / 4 && capacity > 2) {
            resize(capacity / 2);
        }
    }

    void find(T item) {
        cout << linearSearch(container, size, item) << endl;
    }

    void printSize() {
        cout << "Size: " << size << endl;
    }

    void printCapacity() {
        cout << "Capacity: " << capacity << endl;
    }

    void print() {
        for (int i = 0; i < size; i++) {
            cout << container[i] << (i == size - 1 ? "" : " ");
        }
        cout << endl;
    }
};

// ================= DRIVER CODE =================

template <typename T>
void runGlobalMode() {
    int N;
    cin >> N;
    T* arr = new T[N];
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    int K;
    cin >> K;
    string op;
    for (int i = 0; i < K; i++) {
        cin >> op;
        if (op == "linearSearch") {
            T val; cin >> val;
            cout << linearSearch(arr, N, val) << endl;
        } else if (op == "binarySearch") {
            T val; cin >> val;
            cout << binarySearch(arr, N, val) << endl;
        } else if (op == "sort") {
            sort(arr, N);
            cout << "Sorted" << endl;
        }
    }
    delete[] arr;
}

template <typename T>
void runCrateMode() {
    Crate<T> myCrate;
    string op;
    while (cin >> op) {
        if (op == "add") {
            T val; cin >> val;
            myCrate.add(val);
        } else if (op == "remove") {
            myCrate.remove();
        } else if (op == "find") {
            T val; cin >> val;
            myCrate.find(val);
        } else if (op == "size") {
            myCrate.printSize();
        } else if (op == "capacity") {
            myCrate.printCapacity();
        } else if (op == "print") {
            myCrate.print();
        }
    }
}

int main() {
    // Faster I/O for automated testing
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string mode;
    cin >> mode;
    if (mode == "global") {
        string type;
        cin >> type;
        if (type == "int") runGlobalMode<int>();
        else if (type == "double") runGlobalMode<double>();
    } else {
        // The first word read was actually the data type for Crate mode
        string type = mode; 
        if (type == "int") runCrateMode<int>();
        else if (type == "double") runCrateMode<double>();
    }
    return 0;
}