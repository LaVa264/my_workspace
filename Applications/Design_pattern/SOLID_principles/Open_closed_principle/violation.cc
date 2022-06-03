#include <iostream>
#include <vector>

enum class Color : uint8_t
{ // This enum class describes colors of the product.
    red,
    blue,
    green,
    yellow
};

enum class Size : uint8_t
{ // This enum class describes sizes of the product.
    mall,
    medium,
    large
};

class Product
{ // This class provides document related properties and their getter and setter.
    std::string m_name;
    Color m_color;
    Size m_size;

public:
    explicit Product(const std::string &_name, const Color &_color, const Size &_size)
        : m_name{_name}, m_color{_color}, m_size{_size}
    { // Constructor.
    }

    Size size() const { return m_size; }
    void setSize(const Size &size) { m_size = size; }

    Color color() const { return m_color; }
    void setColor(const Color &color) { m_color = color; }

    std::string name() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }
};

class Product_filter
{ // This class provides methods to filter products by its properties.
public:
    static std::vector<Product *> filter_by_size(std::vector<Product *> products, const Size &size)
    { // This method filters products by size.
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

    /* Oh no, customer requested to add a new feature, filtering by color. */
    static std::vector<Product *> filter_by_color(std::vector<Product *> products, const Color &color)
    { // This method filters products by color.
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

    /* Now, they want a method that can filter by both color and size. */
    static std::vector<Product *> filter_by_color_and_size(std::vector<Product *> products, const Color &color, const Size &size)
    { // This method filters products by color and size.
        std::vector<Product *> list_is_filtered;
        for (auto &entity : products)
        {
            if (entity && entity->color() == color && entity->size() == size)
            {
                list_is_filtered.push_back(entity);
            }
        }
        return list_is_filtered;
    }
};

int main()
{
    Product apple{"Apple", Color::red, Size::mall};
    Product house{"House", Color::yellow, Size::large};
    Product tree{"Tree", Color::green, Size::medium};

    std::vector<Product *> items{&apple, &house, &tree};
    std::vector<Product *> items_result;
    items_result = Product_filter::filter_by_size(items, Size::mall);
    items_result = Product_filter::filter_by_color(items, Color::green);
    items_result = Product_filter::filter_by_color_and_size(items, Color::green, Size::mall);
}
