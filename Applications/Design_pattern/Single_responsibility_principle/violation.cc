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

    void save(const std::string &filename)
    {
        std::ofstream ofs{filename};
        for (std::string &str : m_contents)
        {
            ofs << str << std::endl;
        }
    }
    ~document() = default;
};

int main()
{
    document doc{"Larva", {"I hate fixing bugs!!", "I'm going to sleep."}};
    doc.save("database.txt");
}
