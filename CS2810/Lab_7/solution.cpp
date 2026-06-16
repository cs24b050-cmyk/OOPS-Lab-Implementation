#include <bits/stdc++.h>
using namespace std;

// -------------------- STRUCT --------------------
struct Meeting {
    int id;
    int start;
    int end;
    int room;
    int index;   // original input order (0-indexed)
};

// -------------------- FUNCTOR: SORT BY START TIME --------------------
struct CmpStart {
    bool operator()(const Meeting &a, const Meeting &b) const {
        if (a.start == b.start)
            return a.index < b.index;   // tie-break: preserve input order
        return a.start < b.start;
    }
};

// -------------------- FUNCTOR: SORT BY ROOM --------------------
struct CmpRoom {
    bool operator()(const Meeting &a, const Meeting &b) const {
        if (a.room == b.room)
            return a.start < b.start;   // secondary: start time within same room
        return a.room < b.room;         // primary: room number
    }
};

// -------------------- FUNCTION 1: ASSIGN ROOMS --------------------
int assignRooms(vector<Meeting> &meetings) {
    sort(meetings.begin(), meetings.end(), CmpStart());

    set<pair<int, int>> active;   // {endTime, roomNumber}
    int roomCount = 0;

    for (auto &m : meetings) {
        if (!active.empty() && active.begin()->first <= m.start) {
            auto it = active.begin();
            int roomNo = it->second;
            active.erase(it);

            m.room = roomNo;
            active.insert({m.end, roomNo});
        } else {
            roomCount++;
            m.room = roomCount;
            active.insert({m.end, roomCount});
        }
    }

    return roomCount;
}

// -------------------- FUNCTION 2: MOST USED ROOM --------------------
int findMostUsedRoom(const vector<Meeting> &meetings, int totalRooms) {
    vector<int> freq(totalRooms + 1, 0);

    for (const auto &m : meetings)
        freq[m.room]++;

    int bestRoom = 1;
    for (int i = 2; i <= totalRooms; i++) {
        if (freq[i] > freq[bestRoom])
            bestRoom = i;
    }

    return bestRoom;
}

// -------------------- FUNCTION 3: LONGEST MEETING --------------------
int findLongestMeeting(const vector<Meeting> &meetings) {
    int bestId = meetings[0].id;
    int bestDur = meetings[0].end - meetings[0].start;

    for (const auto &m : meetings) {
        int dur = m.end - m.start;
        if (dur > bestDur || (dur == bestDur && m.id < bestId)) {
            bestDur = dur;
            bestId = m.id;
        }
    }

    return bestId;
}

// -------------------- FUNCTION 4: FIND MEETINGS AT TIME --------------------
void findMeetingsAtTime(const vector<Meeting> &meetings, int time) {
    vector<Meeting> sorted_meetings = meetings;
    sort(sorted_meetings.begin(), sorted_meetings.end(), CmpStart());

    bool found = false;
    for (const auto &m : sorted_meetings) {
        if (m.start <= time && time < m.end) {
            if (found) cout << " ";
            cout << m.id;
            found = true;
        }
    }

    if (!found)
        cout << "None";

    cout << "\n";
}

// -------------------- FUNCTION 5: PRINT SORTED LIST --------------------
void printSortedList(vector<Meeting> meetings) {
    sort(meetings.begin(), meetings.end(), CmpStart());

    for (const auto &m : meetings) {
        cout << "Meeting " << m.id
             << " [" << m.start << "," << m.end << "]\n";
    }
}

// -------------------- FUNCTION 6: PRINT SCHEDULE --------------------
void printSchedule(vector<Meeting> meetings) {
    sort(meetings.begin(), meetings.end(), CmpRoom());

    for (const auto &m : meetings) {
        cout << "Meeting " << m.id
             << " -> Room " << m.room
             << " [" << m.start << "," << m.end << "]\n";
    }
}

// -------------------- MAIN --------------------
int main() {

    int n;
    cin >> n;

    vector<Meeting> meetings(n);

    for (int i = 0; i < n; i++) {
        cin >> meetings[i].id >> meetings[i].start >> meetings[i].end;
        meetings[i].room = -1;
        meetings[i].index = i;
    }

    // assign rooms
    int totalRooms = assignRooms(meetings);

    // precompute query results
    int mostUsedRoom = findMostUsedRoom(meetings, totalRooms);
    int longestMeetingId = findLongestMeeting(meetings);

    // process queries
    int q;
    cin >> q;

    while (q--) {
        string command;
        cin >> command;

        if (command == "sortedlist") {
            printSortedList(meetings);
        }
        else if (command == "schedule") {
            printSchedule(meetings);
        }
        else if (command == "rooms") {
            cout << totalRooms << "\n";
        }
        else if (command == "mostused") {
            cout << mostUsedRoom << "\n";
        }
        else if (command == "longest") {
            cout << longestMeetingId << "\n";
        }
        else if (command == "find") {
            int time;
            cin >> time;
            findMeetingsAtTime(meetings, time);
        }
    }

    return 0;
}

// //N^2 solution gives TLE 
// #include <bits/stdc++.h>
// using namespace std;

// struct Meeting {
//     int id;
//     int start;
//     int end;
//     int room;
//     int index; 
// };

// // -------------------- O(N^2) SELECTION SORT --------------------
// // Replaces std::sort to ensure O(N^2) complexity
// void selectionSortStart(vector<Meeting> &arr) {
//     int n = arr.size();
//     for (int i = 0; i < n - 1; i++) {
//         int min_idx = i;
//         for (int j = i + 1; j < n; j++) {
//             // Primary: Start time, Secondary: Original Index [cite: 16, 31]
//             if (arr[j].start < arr[min_idx].start || 
//                (arr[j].start == arr[min_idx].start && arr[j].index < arr[min_idx].index)) {
//                 min_idx = j;
//             }
//         }
//         swap(arr[i], arr[min_idx]);
//     }
// }

// void selectionSortRoom(vector<Meeting> &arr) {
//     int n = arr.size();
//     for (int i = 0; i < n - 1; i++) {
//         int min_idx = i;
//         for (int j = i + 1; j < n; j++) {
//             // Primary: Room number, Secondary: Start time [cite: 35]
//             if (arr[j].room < arr[min_idx].room || 
//                (arr[j].room == arr[min_idx].room && arr[j].start < arr[min_idx].start)) {
//                 min_idx = j;
//             }
//         }
//         swap(arr[i], arr[min_idx]);
//     }
// }

// // -------------------- FUNCTION 1: ASSIGN ROOMS O(N^2) --------------------
// int assignRooms(vector<Meeting> &meetings) {
//     selectionSortStart(meetings);

//     // Track the end time of the last meeting in each room
//     vector<int> roomLastEnd; 
//     int roomCount = 0;

//     for (auto &m : meetings) {
//         int targetRoom = -1;
//         int earliestEnd = INT_MAX;

//         // Linear scan to find a room that freed earliest 
//         for (int i = 0; i < roomCount; i++) {
//             if (roomLastEnd[i] <= m.start) {
//                 if (roomLastEnd[i] < earliestEnd) {
//                     earliestEnd = roomLastEnd[i];
//                     targetRoom = i;
//                 }
//             }
//         }

//         if (targetRoom != -1) {
//             // Reuse existing room 
//             m.room = targetRoom + 1;
//             roomLastEnd[targetRoom] = m.end;
//         } else {
//             // Open new room [cite: 21]
//             roomCount++;
//             m.room = roomCount;
//             roomLastEnd.push_back(m.end);
//         }
//     }
//     return roomCount;
// }

// // -------------------- REMAINING FUNCTIONS (Standard scans) --------------------

// int findMostUsedRoom(const vector<Meeting> &meetings, int totalRooms) {
//     vector<int> freq(totalRooms + 1, 0);
//     for (const auto &m : meetings) freq[m.room]++;
//     int bestRoom = 1;
//     for (int i = 2; i <= totalRooms; i++) {
//         if (freq[i] > freq[bestRoom]) bestRoom = i; // Tie-break: smallest room [cite: 41]
//     }
//     return bestRoom;
// }

// int findLongestMeeting(const vector<Meeting> &meetings) {
//     int bestId = meetings[0].id;
//     int bestDur = meetings[0].end - meetings[0].start;
//     for (const auto &m : meetings) {
//         int dur = m.end - m.start;
//         if (dur > bestDur || (dur == bestDur && m.id < bestId)) { // Tie-break: smallest ID [cite: 44]
//             bestDur = dur;
//             bestId = m.id;
//         }
//     }
//     return bestId;
// }

// void findMeetingsAtTime(vector<Meeting> meetings, int time) {
//     selectionSortStart(meetings);
//     bool found = false;
//     for (const auto &m : meetings) {
//         if (m.start <= time && time < m.end) { // [cite: 48]
//             if (found) cout << " ";
//             cout << m.id;
//             found = true;
//         }
//     }
//     if (!found) cout << "None";
//     cout << "\n";
// }

// void printSortedList(vector<Meeting> meetings) {
//     selectionSortStart(meetings);
//     for (const auto &m : meetings) {
//         cout << "Meeting " << m.id << " [" << m.start << "," << m.end << "]\n";
//     }
// }

// void printSchedule(vector<Meeting> meetings) {
//     selectionSortRoom(meetings);
//     for (const auto &m : meetings) {
//         cout << "Meeting " << m.id << " -> Room " << m.room << " [" << m.start << "," << m.end << "]\n";
//     }
// }

// int main() {
//     int n;
//     if(!(cin >> n)) return 0;
//     vector<Meeting> meetings(n);
//     for (int i = 0; i < n; i++) {
//         cin >> meetings[i].id >> meetings[i].start >> meetings[i].end;
//         meetings[i].room = -1;
//         meetings[i].index = i;
//     }

//     int totalRooms = assignRooms(meetings);
//     int mostUsedRoom = findMostUsedRoom(meetings, totalRooms);
//     int longestMeetingId = findLongestMeeting(meetings);

//     int q;
//     cin >> q;
//     while (q--) {
//         string command;
//         cin >> command;
//         if (command == "sortedlist") printSortedList(meetings);
//         else if (command == "schedule") printSchedule(meetings);
//         else if (command == "rooms") cout << totalRooms << "\n";
//         else if (command == "mostused") cout << mostUsedRoom << "\n";
//         else if (command == "longest") cout << longestMeetingId << "\n";
//         else if (command == "find") {
//             int t; cin >> t;
//             findMeetingsAtTime(meetings, t);
//         }
//     }
//     return 0;
// }