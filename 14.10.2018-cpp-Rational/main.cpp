#include <iostream>
#include <cassert>

class Rational {
 public:
  Rational() : numerator_(0), denominator_(1) {}

  explicit Rational(int x) : numerator_(x), denominator_(1) {}

  Rational(int x, int y) : numerator_(x), denominator_(y) {
    Normalize();
  }

  double GetValue() const {
    return (double) numerator_ / denominator_;
  }

  void PrintFraction() const {
    std::cout << "Value is " << numerator_ << '/' << denominator_ << "\n";
  }

  Rational operator+() const;
  Rational operator-() const;

  Rational operator+(const Rational &other) const;
  Rational operator+(int num) const;
  Rational &operator+=(const Rational &other);
  friend Rational operator+(int num, const Rational &other);

  Rational operator-(const Rational &other) const;
  Rational operator-(int num) const;
  Rational &operator-=(const Rational &other);
  friend Rational operator-(int num, const Rational &other);

  Rational operator*(const Rational &other) const;
  Rational operator*(int num) const;
  Rational &operator*=(const Rational &other);
  friend Rational operator*(int num, const Rational &other);

  Rational operator/(const Rational &other) const;
  Rational operator/(int num) const;
  Rational &operator/=(const Rational &other);
  friend Rational operator/(int num, const Rational &other);

  bool operator==(const Rational &other) const;
  bool operator!=(const Rational &other) const;
  bool operator<(const Rational &other) const;
  bool operator<=(const Rational &other) const;
  bool operator>(const Rational &other) const;
  bool operator>=(const Rational &other) const;

 private:
  int numerator_;
  int denominator_;

  int Gcd(int a, int b) const {
    return b ? Gcd(b, a % b) : a;
  }

  void Normalize() {
    if (denominator_ == 0) {
      throw std::runtime_error("Diving by zero");
    }
    int gcd = Gcd(abs(numerator_), abs(denominator_));
    numerator_ /= gcd;
    denominator_ /= gcd;
    if (denominator_ < 0) {
      numerator_ *= -1;
      denominator_ *= -1;
    }
  }
};

Rational Rational::operator-() const {
  return {-numerator_, denominator_};
}

Rational Rational::operator+() const {
  return *this;
}

Rational Rational::operator+(const Rational &other) const {
  Rational tmp = *this;
  tmp.numerator_ = tmp.numerator_ * other.denominator_ +
      other.numerator_ * tmp.denominator_;
  tmp.denominator_ = denominator_ * other.denominator_;
  tmp.Normalize();
  return tmp;
}

Rational Rational::operator+(int num) const {
  return {numerator_ + num * denominator_, denominator_};
}

Rational &Rational::operator+=(const Rational &other) {
  *this = *this + other;
  return *this;
}

Rational operator+(int num, const Rational &other) {
  return other + num;
}

Rational Rational::operator-(const Rational &other) const {
  return *this + (-other);
}

Rational Rational::operator-(int num) const {
  return *this + (-num);
}

Rational &Rational::operator-=(const Rational &other) {
  *this = *this - other;
  return *this;
}

Rational operator-(int num, const Rational &other) {
  return -(other - num);
}

Rational Rational::operator*(const Rational &other) const {
  Rational tmp = *this;
  tmp.numerator_ *= other.numerator_;
  tmp.denominator_ *= other.denominator_;
  tmp.Normalize();
  return tmp;
}

Rational Rational::operator*(int num) const {
  return {numerator_ * num, denominator_};
}

Rational &Rational::operator*=(const Rational &other) {
  *this = *this * other;
  return *this;
}

Rational operator*(int num, const Rational &other) {
  return other * num;
}

Rational Rational::operator/(const Rational &other) const {
  Rational tmp = other;
  std::swap(tmp.numerator_, tmp.denominator_);
  tmp = tmp * (*this);
  return tmp;
}

Rational Rational::operator/(int num) const {
  return {numerator_, denominator_ * num};
}

Rational &Rational::operator/=(const Rational &other) {
  *this = *this / other;
  return *this;
}

Rational operator/(int num, const Rational &other) {
  return {num * other.denominator_, other.numerator_};
}

bool Rational::operator==(const Rational &other) const {
  return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}

bool Rational::operator!=(const Rational &other) const {
  return !(*this == other);
}

bool Rational::operator<(const Rational &other) const {
  return numerator_ * other.denominator_ < other.numerator_ * denominator_;
}

bool Rational::operator<=(const Rational &other) const {
  return *this < other || *this == other;
}

bool Rational::operator>(const Rational &other) const {
  return other < *this;
}

bool Rational::operator>=(const Rational &other) const {
  return other <= *this;
}

int main() {
  Rational f1{1, 4};
  Rational f2(1, 100);

  assert(+f1 == f1);
  assert(-f1 == f1 * -1);

  assert(f1 + f2 == Rational(13, 50));
  assert(f1 + 2 == Rational(9, 4));
  assert(3 + f1 == Rational(13, 4));

  assert(f1 - f2 == Rational(6, 25));
  assert(2 - f1 == Rational(7, 4));
  assert(f1 - 4 == Rational(-15, 4));

  assert(f1 * f2 == Rational(1, 400));
  assert(2 * f1 == Rational(1, 2));
  assert(f1 * 3 == Rational(3, 4));

  assert(f1 / f2 == Rational(25, 1));
  assert(2 / f1 == Rational(8, 1));
  assert(f1 / 5 == Rational(1, 20));
  assert(f1 / 1 == f1);

  assert((f1 + 1) / -f2 == Rational(-125, 1));
  assert(1 / f2 - f1 * 4 == Rational(99, 1));

  assert(f1.GetValue() == 0.25);

  Rational f3(1, 4);
  assert(f1 != f2);
  assert(f1 > f2);
  assert(f1 > f2);
  assert(f1 >= f2);
  assert(f1 >= f3);
  assert(f1 <= f3);
  assert(f1 == f3);
  assert(f1 != f2);
  assert(f1 == f3);

  Rational f4(2, -6);
  f4.PrintFraction();
  //prints "Value is -1/3"

  f1 += f2;
  assert(f1 == Rational(13, 50));
  f1 += -f2;

  f1 -= f2;
  assert(f1 == Rational(6, 25));
  f1 -= -f2;

  f1 *= f2;
  assert(f1 == Rational(1, 400));
  f1 *= 1 / f2;

  f1 /= f2;
  assert(f1 == Rational(25, 1));
  f1 /= 1 / f2;

  Rational f6{3, 2};
  f6 = f6 * 2;
  f6.PrintFraction();

  Rational f5(0);
  //throwing an exception
  //f1 / f5;

  return 0;
}
