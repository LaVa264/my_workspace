#include <iostream>
#include <tuple>
#include <vector>

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

class human_resource_manager
{
public:
  std::vector<std::tuple<person, relationship, person>> m_relations;
  void add_boss_and_employer(const person &_boss, const person &_employer)
  {
    m_relations.push_back(std::make_tuple(_boss, relationship::boss, _employer));
    m_relations.push_back(std::make_tuple(_employer, relationship::employer, _boss));
  }

  auto &get_relations() const { return m_relations; }
};

class research
{
public:
  std::vector<person> search_boss_by_name(const human_resource_manager &hr, const std::string &_name)
  {
    std::vector<person> result;
    auto relations = hr.get_relations();
    for (auto &&[first, rel, second] : relations)
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