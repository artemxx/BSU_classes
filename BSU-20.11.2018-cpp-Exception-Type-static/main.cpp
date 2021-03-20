#include <iostream>

using namespace std;

class Test {
 public:
  explicit operator int() const {
    return 5;
  }
};

class Test2 {
 public:
  operator int() const {
    return 6;
  }
};

double Divide(int a, int b) {
  if (b == 0) {
    throw std::exception();
  }
  return static_cast<double>(a) / b;
}

double Divide2(int a, int b) {
  if (b == 0) {
    throw std::runtime_error("Division by zero3");
  }
  return static_cast<double>(a) / b;
}

int Mul(int a, int b) noexcept {
  return a * b;
}

class Test3 {
 public:
  static int GetNextId() {
    static int i = 0;
    return i++;
  }
};

int main() {
  {
    Test a;
    cout << (int) a << " " << int(a) << "\n";
    Test2 b;
    cout << b << "\n";
    cout << "\n";
  }

  {
    try {
      cout << Divide(1, 0) << "\n";
    } catch (...) {
      cout << "Division by zero" << "\n";
    }

    try {
      cout << Divide(1, 0) << "\n";
    } catch (const std::exception& e) {
      cout << "Division by zero2" << "\n";
    }

    try {
      cout << Divide2(1, 0) << "\n";
    } catch (const std::runtime_error& e) {
      cout << e.what() << "\n";
    }

    cout << Mul(3, 4) << "\n";

    cout << "\n";
  }

  {
    Test3 a;
    for (int i = 0; i < 5; ++i) {
      cout << a.GetNextId() << " ";
    }
    cout << "\n";

    for (int i = 0; i < 5; ++i) {
      cout << Test3::GetNextId() << " ";
    }
    cout << "\n";

    cout << "\n";
  }

  return 0;
}
