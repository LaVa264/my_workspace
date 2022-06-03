#include <iostream>
#include <vector>
class document {
  std::string m_header{};
  std::vector<std::string> m_contents;
};

class printer_interface {
  virtual void print(document &doc) = 0;
};

class fax_interface {
  virtual void fax(document &doc) = 0;
};
class scanner_interface {
  virtual void scan(document &doc) = 0;
};

class multi_function_printer : public scanner_interface,
                               public printer_interface,
                               public fax_interface { // OK
  void print(document &doc) override {}
  void fax(document &doc) override {}
  void scan(document &doc) override {}
};

class scanner : public scanner_interface { // OK
  void scan(document &doc) override {
    // Do scanning ...
  }
};

int main() {}