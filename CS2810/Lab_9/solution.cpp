#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Item
{
    int weight;
    int value;
};

class Knapsack
{
  private:
    int capacity;
    vector<Item> items;
    vector<vector<int>> dp;

  public:
    Knapsack(int capacity)
    {
        this->capacity = capacity;
    }

    void addItem(int weight, int value)
    {
        items.push_back(Item{.weight = weight, .value = value});
    }

    int solve()
    {
        dp.resize(items.size());
        for (auto &v : dp)
        {
            v.resize(capacity + 1);
        }

        for (int j = 0; j < dp[0].size(); j++)
        {
            if (j < items[0].weight)
            {
                dp[0][j] = 0;
            }
            else
            {
                dp[0][j] = items[0].value;
            }
        }

        for (int i = 1; i < dp.size(); i++)
        {
            for (int j = 0; j < dp[i].size(); j++)
            {
                dp[i][j] = dp[i - 1][j];
                int rem = j - items[i].weight;
                if (rem >= 0)
                {
                    dp[i][j] = max(dp[i][j], dp[i - 1][rem] + items[i].value);
                }
            }
        }
        return dp.back().back();
    }

    void print()
    {
        for (auto &v : dp)
        {
            for (auto i : v)
            {
                cout << i << " ";
            }
            cout << "\n";
        }
    }

    void backtrace()
    {
        vector<int> idx;
        int i = dp.size() - 1;
        int j = dp[i].size() - 1;

        while (i > 0)
        {
            if (dp[i][j] == dp[i - 1][j])
            {
                i--;
            }
            else
            {
                idx.push_back(i);
                j -= items[i].weight;
                i--;
            }
        }

        if (j > 0 && dp[0][j] > 0)
        {
            idx.push_back(i);
        }

        reverse(idx.begin(), idx.end());
        cout << idx.size() << " ";
        for (int i = 0; i < idx.size(); i++)
        {
            cout << idx[i] << " ";
        }
        cout << "\n";
    }
};

class HouseRobber
{
  private:
    vector<int> houses;
    vector<int> dp;

  public:
    HouseRobber(vector<int> houseValues)
    {
        houses = houseValues;
    }

    int solve()
    {
        dp.resize(houses.size());
        dp[0] = houses[0];

        for (int i = 1; i < dp.size(); i++)
        {
            dp[i] = dp[i - 1];
            int val = houses[i];
            if (i - 2 >= 0)
            {
                val += dp[i - 2];
            }
            dp[i] = max(dp[i], val);
        }

        return dp.back();
    }

    void print()
    {
        for (auto i : dp)
        {
            cout << i << "\n";
        }
    }

    void backtrace()
    {
        vector<int> idx;
        int i = dp.size() - 1;
        while (i > 0)
        {
            if (dp[i] == dp[i - 1])
            {
                i--;
            }
            else
            {
                idx.push_back(i);
                i -= 2;
            }
        }
        if (i == 0 && houses[0] > 0)
        {
            idx.push_back(0);
        }

        reverse(idx.begin(), idx.end());
        cout << idx.size() << " ";
        for (int i = 0; i < idx.size(); i++)
        {
            cout << idx[i] << " ";
        }
        cout << "\n";
    }
};

constexpr int MOD = 1e9 + 7;

class TwoSets
{
  private:
    int n;
    vector<vector<int>> dp;

  public:
    TwoSets(int n)
    {
        this->n = n;
    }

    int solve()
    {
        dp.resize(n + 1);

        int x = n * (n + 1);
        if (x % 4 != 0)
        {
            return 0;
        }

        x /= 4;

        for (auto &v : dp)
        {
            v.resize(x + 1, 0);
        }

        dp[0][0] = 1;

        for (int i = 1; i < dp.size(); i++)
        {
            for (int j = 0; j < dp[i].size(); j++)
            {
                dp[i][j] = dp[i - 1][j];
                if (j - i >= 0)
                {
                    (dp[i][j] += dp[i - 1][j - i]) %= MOD;
                }
            }
        }

        return dp[n - 1][x];
    }

    void print()
    {
        for (auto &v : dp)
        {
            for (auto i : v)
            {
                cout << i << " ";
            }
            cout << "\n";
        }
    }

    void backtrace() {}
};

template <typename T> void execute(T &obj)
{
    string cmd;
    while (cin >> cmd && cmd != "end")
    {
        if (cmd == "solve")
        {
            cout << obj.solve() << "\n";
        }
        else if (cmd == "backtrace")
        {
            obj.backtrace();
        }
    }
}

int main()
{
    int problem;
    cin >> problem;
    if (problem == 1)
    {
        int n, w;
        cin >> n >> w;
        Knapsack k(w);
        for (int i = 0; i < n; i++)
        {
            int w, v;
            cin >> w >> v;
            k.addItem(w, v);
        }
        execute(k);
        return 0;
    }
    if (problem == 2)
    {
        int n;
        cin >> n;
        vector<int> house(n);
        for (int i = 0; i < n; i++)
        {
            cin >> house[i];
        }
        HouseRobber h(house);
        execute(h);
        return 0;
    }
    if (problem == 3)
    {
        int n;
        cin >> n;
        TwoSets t(n);
        execute(t);
        return 0;
    }
}