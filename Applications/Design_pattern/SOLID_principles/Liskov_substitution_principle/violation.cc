#include <iostream>
#include <assert.h>
#include <stdint.h>

class Rectangle
{
protected:
    uint32_t m_height;
    uint32_t m_width;

public:
    Rectangle(uint32_t height, uint32_t width) : m_height(height), m_width(width) {}
    uint32_t get_height() { return m_height; }
    uint32_t get_width() { return m_width; }
    virtual void set_height(uint32_t height) { m_height = height; }
    virtual void set_width(uint32_t width) { m_width = width; }
    uint32_t area() { return m_height * m_width; }
    virtual ~Rectangle() = default;
};

class Square : public Rectangle
{
public:
    Square(uint32_t size) : Rectangle(size, size) {}
    void set_height(uint32_t height) override { m_height = m_width = height; }
    void set_width(uint32_t width) override { m_height = m_width = width; }
};

void process(Rectangle &r)
{
    uint32_t width = r.get_width();
    r.set_width(10);
    assert((width * 10) == r.area()); // Fails if r is square.
}

int main()
{
    Rectangle rec{10, 6};
    process(rec);

    Square square{5};
    process(square);
}