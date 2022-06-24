#include "person.hh"
#include "person_builder.hh"
int main()
{
    person p = person::create()
                   .basic().with_name("Larva").with_age(23)
                   .live().at("Wall").with_city("New york").with_post_code("LV324")
                   .work().at("Netflix").with_income(100).with_position("Actor");
    std::cout << p << std::endl;
}
