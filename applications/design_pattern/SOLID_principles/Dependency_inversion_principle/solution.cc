#include <iostream>
#include <vector>
#include <tuple>

enum class relationship
{
    boss,
    employer,
    peer
};

class person
{
    std::string m_name;
    uint8_t m_age;

public:
    std::string get_name() const { return m_name; }
};

class human_resource_manager_interface
{
public:
    virtual std::vector<person> get_all_boss_by_employer_name(const std::string &_name) const = 0;
};

class human_resource_manager : public human_resource_manager_interface
{
    std::vector<std::tuple<person, relationship, person>> m_relations;

public:
    void add_boss_and_employer(const person &_boss, const person &_employer)
    {
        m_relations.push_back(std::make_tuple(_boss, relationship::boss, _employer));
        m_relations.push_back(std::make_tuple(_employer, relationship::employer, _boss));
    }

    std::vector<person> get_all_boss_by_employer_name(const std::string &_name) const override
    {
        std::vector<person> result;
        for (auto &[first, rel, second] : m_relations)
        {
            if (first.get_name() == _name && rel == relationship::employer)
            {
                result.push_back(first);
            }
        }
        return result;
    }
};

class research
{
public:
    void search_boss_by_name(const human_resource_manager_interface &hr, const std::string &_name)
    {
        auto person_list = hr.get_all_boss_by_employer_name(_name);
        for (auto &&it : person_list)
        {
            std::cout << it.get_name() << std::endl;
        }
    }
};

int main()
{
}