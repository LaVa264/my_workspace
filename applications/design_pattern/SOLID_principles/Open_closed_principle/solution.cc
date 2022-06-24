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

template <typename T>
class Specification
{ // Base class for specifications.
public:
    virtual bool is_satisfied(T *item) const = 0;
};

template <typename T>
class Filter
{ // Base class for filter.
public:
    virtual std::vector<T *> filter(std::vector<T *> items, const Specification<T> &spec) = 0;
};

class Product_filter : Filter<Product>
{ // This class provides the filter for products.
public:
    std::vector<Product *> filter(std::vector<Product *> items, const Specification<Product> &spec) override
    { // This API will be made available to customer who request the filters for the products.
        std::vector<Product *> result;
        for (auto &item : items)
        {
            if (spec.is_satisfied(item))
            {
                result.push_back(item);
            }
        }
        return result;
    };
};

class Color_specification : public Specification<Product>
{ // Color specification.
    Color m_color;

public:
    Color_specification(const Color &color) : m_color(color){};
    bool is_satisfied(Product *item) const override
    {
        return this->m_color == item->color();
    }
};

class Size_specification : public Specification<Product>
{ // Size specification.
    Size m_size;

public:
    Size_specification(const Size &size) : m_size(size){};
    bool is_satisfied(Product *item) const override
    {
        return this->m_size == item->size();
    }
};

template <typename T>
struct And_specification : Specification<T>
{
    Specification<T> &first;
    Specification<T> &second;
    And_specification(Specification<T> &first, Specification<T> &second) : first(first), second(second) {}
    bool is_satisfied(Product *item) const override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

template <typename T>
And_specification<T> operator&&(const Specification<T> &first, const Specification<T> &second)
{
    return And_specification<T>(first, second);
};

int main()
{
    Product apple{"Apple", Color::red, Size::mall};
    Product house{"House", Color::yellow, Size::large};
    Product tree{"Tree", Color::green, Size::medium};

    std::vector<Product *> items{&apple, &house, &tree};
    std::vector<Product *> items_result;

    items_result = Product_filter().filter(items, Color_specification{Color::green});
    items_result = Product_filter().filter(items, Size_specification{Size::large});
    items_result = Product_filter().filter(items, Size_specification{Size::large} && Color_specification{Color::green});
}