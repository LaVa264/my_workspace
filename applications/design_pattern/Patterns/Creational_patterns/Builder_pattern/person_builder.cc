#include "person.hh"
#include "person_builder.hh"

person_builder_base::person_builder_base(person &_person) : m_ref_person{_person} {}

person_basic_info_builder person_builder_base::basic() const
{
    return person_basic_info_builder{m_ref_person};
}

person_job_builder person_builder_base::work() const
{
    return person_job_builder{m_ref_person};
}
person_address_builder person_builder_base::live() const
{
    return person_address_builder{m_ref_person};
}

person_builder::person_builder() : person_builder_base{m_person} {}

person_basic_info_builder::person_basic_info_builder(person &_person) : person_builder_base{_person} {}

person_basic_info_builder &person_basic_info_builder::with_name(const std::string &_name)
{
    m_ref_person.m_name = _name;
    return *this;
}

person_basic_info_builder &person_basic_info_builder::with_age(const uint8_t &_age)
{
    m_ref_person.m_age = _age;
    return *this;
}

person_address_builder::person_address_builder(person &_person) : person_builder_base{_person} {}

person_address_builder &person_address_builder::at(const std::string &_street_address)
{
    m_ref_person.m_street_address = _street_address;
    return *this;
}

person_address_builder &person_address_builder::with_post_code(const std::string &_post_code)
{
    m_ref_person.m_post_code = _post_code;
    return *this;
}

person_address_builder &person_address_builder::with_city(const std::string &_city)
{
    m_ref_person.m_city = _city;
    return *this;
}

person_job_builder::person_job_builder(person &_person) : person_builder_base{_person} {}

person_job_builder &person_job_builder::at(const std::string &_company_name)
{
    m_ref_person.m_company_name = _company_name;
    return *this;
}

person_job_builder &person_job_builder::with_position(const std::string &_position)
{
    m_ref_person.m_position = _position;
    return *this;
}

person_job_builder &person_job_builder::with_income(const uint32_t &_annual_income)
{
    m_ref_person.m_annual_income = _annual_income;
    return *this;
}