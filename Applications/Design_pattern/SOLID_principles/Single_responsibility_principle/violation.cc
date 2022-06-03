#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class document
{ // This class provides document related properties.
private:
    std::string m_header{};
    std::vector<std::string> m_contents;

public:
    explicit document() = default;
    explicit document(const std::string &_header)
        : m_header{_header}
    { // Constructor.
    }
    explicit document(const std::string &_header, std::initializer_list<std::string> _contents)
        : m_header{_header}, m_contents{_contents}
    { // Constructor.
    }

    void add_contents(const std::string &content)
    { // This method is used to add contents to the document.
        m_contents.push_back(content);
    }

    void save(const std::string &filename)
    { // This method saves this document to the file, oh no, it shouldn't be here. Saving document is not responsible of this class.
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
