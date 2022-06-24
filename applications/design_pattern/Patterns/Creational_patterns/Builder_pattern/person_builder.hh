#pragma once

#include <iostream>
class person;
class person_basic_info_builder;
class person_address_builder;
class person_job_builder;

class person_builder_base
{
protected:
    person &m_ref_person;
    person_builder_base(person &_person);

public:
    person_basic_info_builder basic() const;
    person_job_builder work() const;
    person_address_builder live() const;
    operator person() { return std::move(m_ref_person); };
    virtual ~person_builder_base() = default;
};

class person_builder : public person_builder_base
{
    person m_person;

public:
    person_builder();
};

class person_basic_info_builder : public person_builder_base
{
public:
    person_basic_info_builder(person &_person);
    person_basic_info_builder &with_name(const std::string &_name);
    person_basic_info_builder &with_age(const uint8_t &_age);
};

class person_address_builder : public person_builder_base
{
public:
    person_address_builder(person &_person);
    person_address_builder &at(const std::string &_street_address);
    person_address_builder &with_post_code(const std::string &_post_code);
    person_address_builder &with_city(const std::string &_city);
};

class person_job_builder : public person_builder_base
{
public:
    person_job_builder(person &_person);
    person_job_builder &at(const std::string &_company_name);
    person_job_builder &with_position(const std::string &_position);
    person_job_builder &with_income(const uint32_t &_annual_income);
};