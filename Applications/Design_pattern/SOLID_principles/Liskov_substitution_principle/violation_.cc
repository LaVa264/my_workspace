#include <iostream>
#include <typeinfo>
class bird {
public:
  void fly() { std::cout << typeid(*this).name() << " is flying !! \n"; }

  virtual ~bird() = default;
};

class penguin : public bird {
public:
  void fly() = delete;
};

void moving(bird &b) { b.fly(); }

int main() {
  penguin larva;
  // ERROR: larva.fly(); - Can't call deleted method.
  moving(larva);
}