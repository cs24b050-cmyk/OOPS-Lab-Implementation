#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

string to_lower(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

string to_vowel_key(string s) {
    s = to_lower(s);
    for (char &c : s) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            c = '*';
        }
    }
    return s;
}

string to_anagram_key(string s) {
    s = to_lower(s);
    sort(s.begin(), s.end());
    return s;
}

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;
    
    vector<string> wordlist(n);
    unordered_map<string, string> exact;
    unordered_map<string, string> cap;
    unordered_map<string, string> anagram;
    unordered_map<string, string> vowel;

    for (int i = 0; i < n; ++i) {
        cin >> wordlist[i];
        string w = wordlist[i];

        if (exact.find(w) == exact.end()) exact[w] = w;

        string low = to_lower(w);
        if (cap.find(low) == cap.end()) cap[low] = w;

        string ana = to_anagram_key(w);
        if (anagram.find(ana) == anagram.end()) anagram[ana] = w;

        string v_key = to_vowel_key(w);
        if (vowel.find(v_key) == vowel.end()) vowel[v_key] = w;
    }

    for (int i = 0; i < m; ++i) {
        string q;
        cin >> q;

        if (exact.count(q)) {
            cout << exact[q] << (i == m - 1 ? "" : " ");
            continue;
        }

        string low_q = to_lower(q);
        if (cap.count(low_q)) {
            cout << cap[low_q] << (i == m - 1 ? "" : " ");
            continue;
        }

        string ana_q = to_anagram_key(q);
        if (anagram.count(ana_q)) {
            cout << anagram[ana_q] << (i == m - 1 ? "" : " ");
            continue;
        }

        string v_q = to_vowel_key(q);
        if (vowel.count(v_q)) {
            cout << vowel[v_q] << (i == m - 1 ? "" : " ");
            continue;
        }

        cout << "\"\"" << (i == m - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}


// //M*N solution gives TLE
// #include <iostream>
// #include <vector>
// #include <string>
// #include <algorithm>

// using namespace std;

// bool isVowel(char c) {
//     c = tolower(c);
//     return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
// }

// string toLower(string s) {
//     for (char &c : s) c = tolower(c);
//     return s;
// }

// bool isAnagram(string s1, string s2) {
//     if (s1.length() != s2.length()) return false;
//     string t1 = toLower(s1);
//     string t2 = toLower(s2);
//     sort(t1.begin(), t1.end());
//     sort(t2.begin(), t2.end());
//     return t1 == t2;
// }

// bool isVowelMatch(string s1, string s2) {
//     if (s1.length() != s2.length()) return false;
//     for (int i = 0; i < s1.length(); i++) {
//         char c1 = tolower(s1[i]);
//         char c2 = tolower(s2[i]);
//         if (c1 == c2) continue;
//         if (!isVowel(c1) || !isVowel(c2)) return false;
//     }
//     return true;
// }

// void solve() {
//     int n, m;
//     if (!(cin >> n >> m)) return;

//     vector<string> wordlist(n);
//     for (int i = 0; i < n; i++) cin >> wordlist[i];

//     for (int j = 0; j < m; j++) {
//         string query;
//         cin >> query;
//         string result = "";

//         // Rule 1: Exact Match (Priority 1)
//         for (const string& w : wordlist) {
//             if (query == w) {
//                 result = w;
//                 break;
//             }
//         }

//         // Rule 2: Capitalization (Priority 2)
//         if (result == "") {
//             string lowQuery = toLower(query);
//             for (const string& w : wordlist) {
//                 if (lowQuery == toLower(w)) {
//                     result = w;
//                     break;
//                 }
//             }
//         }

//         // Rule 3: Anagram (Priority 3)
//         if (result == "") {
//             for (const string& w : wordlist) {
//                 if (isAnagram(query, w)) {
//                     result = w;
//                     break;
//                 }
//             }
//         }

//         // Rule 4: Vowel Match (Priority 4)
//         if (result == "") {
//             for (const string& w : wordlist) {
//                 if (isVowelMatch(query, w)) {
//                     result = w;
//                     break;
//                 }
//             }
//         }

//         cout << (result == "" ? "\"\"" : result) << (j == m - 1 ? "" : " ");
//     }
//     cout << endl;
// }

// int main() {
//     ios_base::sync_with_stdio(false);
//     cin.tie(NULL);
//     solve();
//     return 0;
// }