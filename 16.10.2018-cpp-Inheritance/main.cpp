#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
 public:
  explicit Person(string name)
      : name_(std::move(name)) {}

  string Name() const {
    return name_;
  }

  string Data() const {
    return Occupation() + ": " + Name();
  }

  virtual string Occupation() const = 0;

  virtual void Walk(const string& destination) const {
    cout << Occupation() << ": " << Name() << " walks to: " << destination << endl;
  }

 private:
  string name_;
};

class Student : public Person {
 public:
  Student(string name, string favouriteSong)
      : Person(std::move(name))
      , favourite_song_(std::move(favouriteSong)) {}

  string Occupation() const override {
    return "Student";
  }

  void SingSong() const {
    cout << Data() << " sings a song: " << favourite_song_ << endl;
  }

  void Walk(const string& destination) const override {
    Person::Walk(destination);
    SingSong();
  }

  void Learn() const {
    cout << Data() << " learns" << endl;
  }

 private:
  string name_;
  string favourite_song_;
};

class Teacher : public Person {
 public:
  Teacher(string name, string subject)
      : Person(std::move(name))
      , subject_(std::move(subject)) {}

  string Occupation() const override {
    return "Teacher";
  }

  void Teach() const {
    cout << Data() << " teaches: " << subject_ << endl;
  }

 private:
  string subject_;
};

class Policeman : public Person {
 public:
  explicit Policeman(string name)
      : Person(std::move(name)) {}

  string Occupation() const override {
    return "Policeman";
  }

  void Check(const Person& person) const {
    cout << Data() << " checks " << person.Occupation() << ". "
         << person.Occupation() << "'s name is: " << person.Name() << endl;
  }
};

void VisitPlaces(const Person& person, const vector<string>& places) {
  for (const auto& place : places) {
    person.Walk(place);
  }
}

int main() {
  Teacher t("Jim", "Math");
  Student s("Ann", "We will rock you");
  Policeman p("Bob");

  VisitPlaces(t, {"Moscow", "London"});
  p.Check(s);
  VisitPlaces(s, {"Moscow", "London"});

  return 0;
}
