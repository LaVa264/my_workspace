#include <iostream>
#include <vector>
class document {
  std::string m_header{};
  std::vector<std::string> m_contents;
};

class machine_interface {
  virtual void print(document &doc) = 0;
  virtual void fax(document &doc) = 0;
  virtual void scan(document &doc) = 0;
};

class multi_function_printer : machine_interface { // OK
  void print(document &doc) override {}
  void fax(document &doc) override {}
  void scan(document &doc) override {}
};

class scanner : machine_interface {    // Not OK
  void print(document &doc) override { /* Blank */
  }
  void fax(document &doc) override { /* Blank */
  }
  void scan(document &doc) override {
    // Do scanning ...
  }
};

int main() {}