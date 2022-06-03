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
    std::string get_name() { return m_name; }
};

class human_resource_manager_interface
{
};

class human_resource_manager
{
    std::vector<std::tuple<person, relationship, person>> m_relations;

public:
    void add_boss_and_employer(const person &_boss, const person &_employer)
    {
        m_relations.push_back(std::make_tuple(_boss, relationship::boss, _employer));
        m_relations.push_back(std::make_tuple(_employer, relationship::employer, _boss));
    }

    std::vector<person> find_all_boss_by_employer_name(const std::string &_name)
    {
        std::vector<person> result;
        for (auto &&[first, rel, second] : m_relations)
        {
            if (first.get_name() == _name && rel == relationship::employer)
            {
                result.push_back(first);
            }
        }
        return result;
    }
};

int main()
{
}