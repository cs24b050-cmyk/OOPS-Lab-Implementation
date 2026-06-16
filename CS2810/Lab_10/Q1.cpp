#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// =====================================================
// ==================== PROBLEM 1 =======================
// =====================================================

class BaseColumn {
public:
  BaseColumn() {}
  virtual void addFromString(const string &) = 0;
  virtual void printStats(int idx) const = 0;
  virtual ~BaseColumn() {}
};

template <typename T> class CSVColumn : public BaseColumn {
public:
  string type;
  vector<T> data;

  CSVColumn<T>() : BaseColumn() {}

  void addValue(const T &v) { data.push_back(v); }

  void addFromString(const string &s) override {
    if constexpr (is_same<T, int>::value) {
      try {
        data.push_back(stoi(s));
      } catch (...) {
        throw runtime_error("ERR_CELL");
      }
    } else if constexpr (is_same<T, double>::value) {
      try {
        data.push_back(stod(s));
      } catch (...) {
        throw runtime_error("ERR_CELL");
      }
    } else {
      data.push_back(s);
    }
  }

  void printStats(int idx) const override {
    if constexpr (is_same<T, string>::value)
      throw runtime_error("ERR_TYPE");
    else {
      double mn = static_cast<double>(data[0]),
             mx = static_cast<double>(data[0]);
      double sum = 0, sq = 0;

      for (auto &x : data) {
        mn = min(mn, static_cast<double>(x));
        mx = max(mx, static_cast<double>(x));
        sum += static_cast<double>(x);
        sq += static_cast<double>(x) * static_cast<double>(x);
      }

      double n = data.size();
      double mean = sum / n;
      double var = sq / n - mean * mean;

      cout << fixed << setprecision(2);
      cout << "Col " << idx << ": min=" << mn << " max=" << mx
           << " mean=" << mean << " var=" << var << "\n";
    }
  }
};

class CSVData {
public:
  vector<BaseColumn *> cols;

  void parse(const string &file) {
    ifstream in(file);
    if (!in) {
      cout << "ERR_FILE\n";
      return;
    }

    string line;
    getline(in, line);

    stringstream ss(line);
    string token;
    vector<string> types;

    while (getline(ss, token, ','))
      types.push_back(token);

    for (auto &t : types) {
      if (t == "int")
        cols.push_back(new CSVColumn<int>());
      else if (t == "double")
        cols.push_back(new CSVColumn<double>());
      else
        cols.push_back(new CSVColumn<string>());
    }

    while (getline(in, line)) {
      stringstream s2(line);
      vector<string> cells;

      while (getline(s2, token, ','))
        cells.push_back(token);

      if ((int)cells.size() != (int)cols.size()) {
        cout << "ERR_ROW\n";
        continue;
      }

      bool bad = false;
      for (int i = 0; i < (int)cells.size(); i++) {
        try {
          cols[i]->addFromString(cells[i]);
        } catch (...) {
          cout << "ERR_CELL\n";
          bad = true;
          break;
        }
      }
      if (bad)
        continue;
    }
  }

  void query(int idx) {
    if (idx < 0 || idx >= (int)cols.size()) {
      cout << "ERR_INDEX\n";
      return;
    }
    try {
      cols[idx]->printStats(idx);
    } catch (...) {
      cout << "ERR_TYPE\n";
    }
  }
};

// =====================================================
// ==================== PROBLEM 2 =======================
// =====================================================

struct Node {
  int dist, v;
  bool operator<(const Node &o) const {
    if (dist != o.dist)
      return dist > o.dist;
    return v > o.v;
  }
};

void solveGraph() {
  int V, E;
  cin >> V >> E;

  vector<vector<pair<int, int>>> adj(V);

  for (int i = 0; i < E; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    adj[u].push_back({v, w});
  }

  int s, t;
  cin >> s >> t;

  vector<int> dist(V, INT_MAX), par(V, -1);

  priority_queue<Node> pq;
  dist[s] = 0;
  pq.push({0, s});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (d > dist[u])
      continue;

    for (auto [v, w] : adj[u]) {
      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        par[v] = u;
        pq.push({dist[v], v});
      }
    }
  }

  for (int i = 0; i < V; i++) {
    if (dist[i] == INT_MAX)
      cout << i << " : INF\n";
    else
      cout << i << " : " << dist[i] << "\n";
  }

  cout << "\n";

  if (dist[t] == INT_MAX) {
    cout << "No path\n";
    return;
  }

  vector<int> path;
  for (int cur = t; cur != -1; cur = par[cur])
    path.push_back(cur);

  reverse(path.begin(), path.end());

  for (int i = 0; i < (int)path.size(); i++) {
    if (i)
      cout << " -> ";
    cout << path[i];
  }
  cout << "\n";
}

// =====================================================
// ==================== MAIN ============================
// =====================================================

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int p;
  cin >> p;

  if (p == 1) {
    string file;
    cin >> file;
    CSVData data;
    data.parse(file);
    // cout << "Parsed " << data.cols.size() << " columns successfully.\n";
    string cmd;
    while (cin >> cmd && cmd != "end") {
      if (cmd == "query_stats") {
        int x;
        cin >> x;
        data.query(x);
      }
    }
  } else {
    solveGraph();
  }
}