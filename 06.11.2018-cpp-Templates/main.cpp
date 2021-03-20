#include <vector>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

template<typename T>
class MyClass {
 public:
  T operator()(T value) {
    return value + 1;
  }
};

template<typename T>
T GetSum(const vector<T>& data) {
  T res{};
  for (const auto& elem : data) {
    res += elem;
  }
  return res;
}

template<>
bool GetSum<bool>(const vector<bool>& data) {
  bool res = false;
  for (const auto& elem : data) {
    res ^= elem;
  }
  return res;
}

int Square(int value) {
  return value * value;
}

template<typename T, typename Func>
void Transform(vector<T>& data, Func func) {
  for (int i = 0; i < data.size(); ++i) {
    data[i] = func(data[i]);
  }
}

template<typename T>
void PrintVector(const vector<T>& data) {
  for (const auto& elem : data) {
    cout << elem << " ";
  }
  cout << "\n";
}

template<typename First, typename Second>
ostream& operator<<(ostream& out, const pair<First, Second>& p) {
  return out << '(' << p.first << ',' << p.second << ')';
}

template<typename Collection>
string Join(const Collection& c, char d) {
  stringstream ss;
  bool first = true;
  for (const auto& i : c) {
    if (!first) {
      ss << d;
    }
    first = false;
    ss << i;
  }
  return ss.str();
}

template<typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& m) {
  return out << '{' << Join(m, ',') << '}';
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vi) {
  return out << '[' << Join(vi, ',') << ']';
}

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  if (range_end == range_begin + 1) return;

  RandomIt range_middle = range_begin + (range_end - range_begin) / 2;
  MergeSort(range_begin, range_middle);
  MergeSort(range_middle, range_end);

  vector<typename RandomIt::value_type> elements_left(range_begin, range_middle);
  vector<typename RandomIt::value_type> elements_right(range_middle, range_end);

  merge(begin(elements_left), end(elements_left),
        begin(elements_right), end(elements_right),
        range_begin);
}

int main() {
  {
    int (* func1)(int) = &Square;
    vector<double> data1 = {1, 2, 3};
    const vector<bool> data2 = {false, true, true, false, true};

    cout << GetSum(data1) << "\n";
    cout << GetSum(data2) << "\n";

    Transform(data1, func1);
    PrintVector(data1);

    Transform(data1, [](int value) { return value * value; });
    PrintVector(data1);

    Transform(data1, MyClass<int>());
    PrintVector(data1);

    cout << "\n";
  }
  {
    vector<double> vi = {1.4, 2, 3};
    cout << vi << endl;
    map<int, double> m = {{1, 2.5}, {3, 4}};
    cout << m << endl;
  }
  {
    vector<int> v = {5, 4, 3, 2, 1, 6, 5, 8, 1, 2, 3};
    MergeSort(begin(v), end(v));
    for (int x : v) {
      cout << x << " ";
    }
    cout << "\n";
  }

  return 0;
}
