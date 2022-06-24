#include <assert.h>
#include <iostream>
#include <stdint.h>

class Shape
{
public:
  virtual uint32_t area() = 0;
};

class Rectangle : public Shape
{
  uint32_t m_height;
  uint32_t m_width;

public:
  Rectangle(uint32_t height, uint32_t width)
      : m_height(height), m_width(width) {}
  uint32_t get_height() { return m_height; }
  uint32_t get_width() { return m_width; }
  void set_height(uint32_t height) { m_height = height; }
  void set_width(uint32_t width) { m_width = width; }
  uint32_t area() override { return m_height * m_width; }
};

class Square : public Shape
{
  uint32_t m_size;

public:
  Square(uint32_t _size) : m_size{_size} {};
  uint32_t get_size() { return m_size; }
  void set_size(uint32_t _size) { m_size = _size; }
  uint32_t area() override { return m_size * m_size; }
};

void process(Shape &r)
{
  // Use polymorphic behaviour only i.e. area()
  r.area();

  // DO SOME THING ...
}

int main() {}