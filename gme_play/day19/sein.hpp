#pragma once

#pragma once
#include <iostream>           // Stream
#include <fstream>
#include <vector>             // STL
#include <string>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <tuple>
#include <deque>
#include <array>
#include <stack>
#include <queue>
#include <bitset>
#include <iterator>
#include <memory>             // Memory Management
#include <chrono>             // Time
#include <cassert>            // Assert
#include <exception>          // Exception
#include <thread>             // Multithread
#include <mutex>
#include <future>
#include <condition_variable>
#include <cmath>              // Numerics
#include <filesystem>         // File System
using namespace std::chrono;
using namespace std;
namespace fs = std::filesystem;

#include <numeric>  // accumulate, iota
#include <iterator> //ostream_iterator
#include <bits/stdc++.h>

using VB = vector<bool>;
using VVB = vector<VB>;
using VS = vector<string>;
using VC = vector<char>;
using VVS = vector<VS>;
using VVC = vector<VC>;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<long long>;
using VVL = vector<VL>;
using VD = vector<double>;
using VVD = vector<VD>;
using PII = pair<int,int>;
using VVS = vector<vector<string>>;
using USS = unordered_set<string>;
using TMatrix=VVI;

#define REP(i,begin,end) for(int i=begin;i<end;i++)
#define REP2(i,begin,end,step) for(int i=begin;i<end;i+=step)

class auto_profiler {
public:
  auto_profiler(std::string name)
      : m_name(std::move(name)),
        m_beg(std::chrono::high_resolution_clock::now()) {}

  ~auto_profiler() {
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - m_beg);
    cout << m_name << " : " << dur.count() << " us\n";
  }

private:
  std::string m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_beg;
};



