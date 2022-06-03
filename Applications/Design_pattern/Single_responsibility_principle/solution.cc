#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class document
{
private:
    std::string m_header{};
    std::vector<std::string> m_contents;

public:
    explicit document() = default;
    explicit document(const std::string &_header)
        : m_header{_header}
    {
    }
    explicit document(const std::string &_header, std::initializer_list<std::string> _contents)
        : m_header{_header}, m_contents{_contents}
    {
    }

    void add_contents(const std::string &content)
    {
        m_contents.push_back(content);
    }

    const std::vector<std::string> &get_contents() const
    {
        return m_contents;
    }

    ~document() = default;
};

class saving_manager
{
public:
    static void save(const document &doc, const std::string &filename)
    {
        std::ofstream ofs{filename};
        auto contents = doc.get_contents();
        for (auto str : contents)
        {
            ofs << str << std::endl;
        }
    }
};

int main()
{
    document doc{"Larva", {"I hate fixing bugs!!", "I'm going to sleep."}};
    saving_manager::save(doc, "database.txt");
}
