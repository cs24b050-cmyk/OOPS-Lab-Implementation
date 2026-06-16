#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

template <typename T>
class SkipList {
private:
    struct Node {
        T key;
        vector<Node*> forward;
        Node(T k, int level) : key(k), forward(level, nullptr) {}
    };

    int maxLevel;
    Node* head;

    vector<int> coinFlips;
    int flipIndex;

    vector<string>& outputs;  

    bool isEqual(T a, T b) {
        if constexpr (is_floating_point<T>::value)
            return fabs(a - b) < 1e-6;
        else
            return a == b;
    }

    int getLevel() {
        int level = 1;
        while (flipIndex < coinFlips.size() &&
               coinFlips[flipIndex] == 1 &&
               level < maxLevel) {
            level++;
            flipIndex++;
        }
        if (flipIndex < coinFlips.size()) flipIndex++;
        return level;
    }

public:
    SkipList(int maxL, const vector<int>& flips, vector<string>& out)
        : maxLevel(maxL), coinFlips(flips), flipIndex(0), outputs(out) {
        head = new Node(T(), maxLevel);
    }

    bool search(T key) {
        Node* curr = head;

        for (int i = maxLevel - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
        }

        curr = curr->forward[0];

        if (curr && isEqual(curr->key, key)) {
            outputs.push_back("Found");
            return true;
        }

        outputs.push_back("Not Found");
        return false;
    }

    void insert(T key) {
        vector<Node*> update(maxLevel, nullptr);
        Node* curr = head;

        for (int i = maxLevel - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];

        if (curr && isEqual(curr->key, key)) {
            outputs.push_back("Already Exists");
            return;
        }

        int level = getLevel();
        Node* newNode = new Node(key, level);

        for (int i = 0; i < level; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }

        outputs.push_back("Inserted");
    }

    void deleteKey(T key) {
        vector<Node*> update(maxLevel, nullptr);
        Node* curr = head;

        for (int i = maxLevel - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }

        curr = curr->forward[0];

        if (!curr || !isEqual(curr->key, key)) {
            outputs.push_back("Not Found");
            return;
        }

        for (int i = 0; i < maxLevel; i++) {
            if (update[i]->forward[i] != curr) break;
            update[i]->forward[i] = curr->forward[i];
        }

        delete curr;
        outputs.push_back("Deleted");
    }

    void print() {
        for (int i = maxLevel - 1; i >= 0; i--) {
            Node* curr = head->forward[i];

            if (!curr) continue;

            string line = "Level " + to_string(i + 1) + ": ";
            while (curr) {
                line += to_string(curr->key) + " ";
                curr = curr->forward[i];
            }
            outputs.push_back(line);
        }
    }
};


template <typename T>
void processCommands(int maxLevel, vector<int>& flips, int n) {
    vector<string> outputs;
    SkipList<T> sl(maxLevel, flips, outputs);

    string command;

    for (int i = 0; i < n; i++) {
        cin >> command;

        if (command == "insert") {
            T key;
            cin >> key;
            sl.insert(key);
        } 
        else if (command == "delete") {
            T key;
            cin >> key;
            sl.deleteKey(key);
        } 
        else if (command == "search") {
            T key;
            cin >> key;
            sl.search(key);
        } 
        else if (command == "print") {
            sl.print();
        }
    }

    for (auto &s : outputs) {
        cout << s << "\n";
    }
}

int main() {
    string type;
    cin >> type;

    int maxLevel;
    cin >> maxLevel;

    cin.ignore();

    string line;
    getline(cin, line);
    stringstream ss(line);

    vector<int> flips;
    int x;
    while (ss >> x) flips.push_back(x);

    int n;
    cin >> n;

    if (type == "int") {
        processCommands<int>(maxLevel, flips, n);
    } 
    else if (type == "float") {
        processCommands<float>(maxLevel, flips, n);
    }

    return 0;
}