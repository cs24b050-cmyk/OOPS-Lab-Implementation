// You Do NOT Suppose to Change the Starter Template except the Mentioned Parts.
// But if you do then I'm not responsible for your failed testcases.
// It is a Very Easy to do Lab Assignment. Best of Luck...

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Order {
    string user;
    int qty;
    double price;
};

class Book {
public:
    string ticker;
    vector<Order> buy, sell;
    Book(string t) : ticker(t) {}
};


void part1() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    // Write your Solution Below
    ifstream fin(file);
    if(!fin) cout << "Error in opening file\n";
    vector<pair<string,int>> a;
    string s;
    while(getline(fin, s)) {
        stringstream ss(s);
        string type,name,ticker,q,p;
        if(!getline(ss,type,',')) continue;
        if(!getline(ss,name,',')) continue;
        if(!getline(ss,ticker,',')) continue;
        if(!getline(ss,q,',')) continue;
        if(!getline(ss,p,',')) continue;
        int i = -1;
        int flag = 0;
        for(i=0;i<a.size();i++) {
            if(a[i].first == ticker) {
                a[i].second++;
                flag = 1;
                break;
            }
        }
        if(i == a.size() && flag == 0) a.push_back({ticker,1});
    }
    sort(a.begin(),a.end());
    for(int i=0;i<a.size();i++) {
        cout << a[i].first << " " << a[i].second << endl;
    }
}

void part2() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    // Write your Solution Below
     ifstream fin(file);
    if(!fin) cout << "Error in opening file\n";
    vector<pair<string,int>> a;
    string s;
    while(getline(fin, s)) {
        stringstream ss(s);
        string type,name,ticker,q,p;
        if(!getline(ss,type,',')) continue;
        if(!getline(ss,name,',')) continue;
        if(!getline(ss,ticker,',')) continue;
        if(!getline(ss,q,',')) continue;
        if(!getline(ss,p,',')) continue;
        int i = -1;
        int flag = 0;
        int q_i = stoi(q);
        for(i=0;i<a.size();i++) {
            if(a[i].first == ticker) {
                a[i].second += q_i;
                flag = 1;
                break;
            }
        }
        if(i == a.size() && flag == 0) a.push_back({name,q_i});
    }
    sort(a.begin(),a.end());
    while(cin >> s) {
        int i = -1;
        int flag = 0;
        for(i=0;i<a.size();i++) {
            if(a[i].first == s) {
                cout << s << " " << a[i].second << endl;
                flag = 1;
                break;
            }
        }
        if(i == a.size() && flag == 0) cout << s << "0\n";
    }
}

void part3() {
    string filePath = "./actual_output/Q1/CSV/";
    string YOUR_ROLL_NUMBER = "CS24B050"; // Change with Your Actual Roll Number
    string fileName;
    cin >> fileName;
    string file = filePath + YOUR_ROLL_NUMBER + "/" + fileName;
    ofstream fout(file);
    fout << "Ticker,Seller,Buyer,Qty,Price,Time\n";
    cin.ignore(1000, '\n');
    // Write your Solution Below
    vector<Book> b;
    string s;
    int count = 0;
        string type,name,ticker,q,p;
        int time = 0;
    while(cin >> type >> name >> ticker >> q >> p) {
        // fout << "this is working\n";
        // stringstream ss(s);
        // cout << "this\n";
        // if(!getline(ss,type,' ')) continue;
        // if(!getline(ss,name,' ')) continue;
        // if(!getline(ss,ticker,' ')) continue;
        // if(!getline(ss,q,' ')) continue;
        // if(!getline(ss,p,' ')) continue;
        // fout << type << " " << name << "this\n";
        // if(count == 0) {
        //     type = s;
        //     count++;
        //     continue;
        // }
        // else if(count ==1) {
        //     name = s;
        //     count++;
        //     continue;
        // }
        // else if(count == 2) {
        //     ticker = s;
        //     count++;
        //     continue;
        // }
        // else if(count == 3) {
        //     q = s;
        //     count++;
        //     continue;
        // }
        // else if(count == 4) {
        //     p = s;
        //     count++;
        //     continue;
        // }
        
            count = 0;
        int i = -1;
        int flag1 = 0;
        int q_i = stoi(q);
        double p_i = stoi(p);
        for(i=0;i<b.size();i++) {
            if(b[i].ticker == ticker) {
                flag1 = 1;
                Order o;
                o.price = p_i;
                o.qty = q_i;
                o.user = name;
                if(type == "BUY") {
                    int j = -1;
                    int flag = 0;
                    for(j=0;j<b[i].sell.size();j++) {
                        if(p_i >= b[i].sell[j].price) {
                            // successful purchase
                            // fout << "buuy\n";
                            flag = 1;
                            int mini = min(q_i,b[i].sell[j].qty);
                            if(q_i >= b[i].sell[j].qty) {
                                q_i -= b[i].sell[j].qty;
                                auto it = b[i].sell.begin()+j;
                                b[i].sell.erase(it);
                                if(q_i != 0) {
                                    // we need to add this to buy list
                                    o.qty = q_i;
                                    b[i].buy.push_back(o);
                                }
                            }
                            else {
                                // we dont need to add this into buy list but we need to update the sell list
                                b[i].sell[j].qty -= q_i;
                            }
                            fout << ticker << "," << b[i].sell[j].user << ","<< name << "," << mini << "," << b[i].sell[j].price <<".00," << time << endl;
                            ++time; 
                        }
                    }
                    if(flag == 0) {
                        // add this order to but list
                        b[i].buy.push_back(o);
                    }
                }
                else {
                    int j = -1;
                    int flag = 0;
                    for(j=0;j<b[i].buy.size();j++) {
                        if(p_i <= b[i].buy[j].price) {
                            // successful purchase
                            flag = 1;
                            int mini = min(q_i,b[i].buy[j].qty);
                            if(q_i >= b[i].buy[j].qty) {
                                q_i -= b[i].buy[j].qty;
                                auto it = b[i].buy.begin()+j;
                                b[i].buy.erase(it);
                                if(q_i != 0) {
                                    // we need to add this to buy list
                                    o.qty = q_i;
                                    b[i].buy.push_back(o);
                                }
                            }
                            else {
                                // we dont need to add this into buy list but we need to update the buy list
                                b[i].buy[j].qty -= q_i;
                            }
                            fout << ticker << "," << name << ","<< b[i].buy[j].user << "," << mini << "," << p_i<<".00," << time << endl; 
                            time++;
                        }
                    }
                    if(flag == 0) {
                        // add this order to sell list
                        b[i].sell.push_back(o);
                    }
                }
                // at the end of computation
                break;
            }
        }
        if(flag1 == 0) {
            Book b1(ticker);
            Order o;
            o.price = p_i;
            o.qty = q_i;
            o.user = name;
            if(type == "BUY") {
                b1.buy.push_back(o);
            }
            else{
                b1.sell.push_back(o);
            }
            b.push_back(b1);
        }
    
    }
}

int main() {
    string part;
    cin >> part;
    
    if (part == "P1") {
        part1();
    } else if (part == "P2") {
        part2();
    } else {
        part3();
    }
    return 0;
}
