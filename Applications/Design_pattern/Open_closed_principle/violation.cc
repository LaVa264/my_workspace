#include <iostream>
#include <vector>

enum class Color : uint8_t
{
    red,
    blue,
    green,
    yellow
};

enum class Size : uint8_t
{
    mall,
    medium,
    large
};

class Product
{
    std::string m_name;
    Color m_color;
    Size m_size;

public:
    explicit Product(const std::string &_name, const Color &_color, const Size &_size)
        : m_name{_name}, m_color{_color}, m_size{_size}
    {
    }

    Size size() const { return m_size; }
    void setSize(const Size &size) { m_size = size; }

    Color color() const { return m_color; }
    void setColor(const Color &color) { m_color = color; }

    std::string name() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }
};

class Product_filter
{
public:
    static std::vector<Product *> filter_by_size(std::vector<Product *> products, const Size &size)
    {
        std::vector<Product *> list_is_filtered;
        for (auto &entity : products)
        {
            if (entity && entity->size() == size)
            {
                list_is_filtered.push_back(entity);
            }
        }
        return list_is_filtered;
    }
    
    static std::vector<Product *> filter_by_color(std::vector<Product *> products, const Color &color)
    {
        std::vector<Product *> list_is_filtered;
        for (auto &entity : products)
        {
            if (entity && entity->color() == color)
            {
                list_is_filtered.push_back(entity);
            }
        }
        return list_is_filtered;
    }
};

int main()
{
}
