#pragma once
#include <string>
#include <iostream>

class person_builder;
class person_basic_info_builder;
class person_address_builder;
class person_job_builder;
class person
{
    std::string m_name;
    uint8_t m_age{0};

    std::string m_street_address;
    std::string m_post_code;
    std::string m_city;

    std::string m_company_name;
    std::string m_position;
    uint32_t m_annual_income{0};

public:
    explicit person() = default;
    static person_builder create();
    ~person() = default;
    friend person_basic_info_builder;
    friend person_address_builder;
    friend person_job_builder;
    friend std::ostream &operator<<(std::ostream &os, const person &obj)
    {
        return os
               << "Person name: " << obj.m_name
               << "\nage: " << int(obj.m_age)
               << "\nstreet: " << obj.m_street_address
               << "\ncity: " << obj.m_city
               << "\ncompany: " << obj.m_company_name
               << "\nposition: " << obj.m_position
               << "\nannual income: " << obj.m_annual_income;
    }
};
