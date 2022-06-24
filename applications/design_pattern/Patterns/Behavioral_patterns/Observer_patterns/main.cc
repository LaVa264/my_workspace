
#include <string>
#include <mutex>
#include <vector>
#include <thread>
#include <iostream>

template <class T>
class observer
{
public:
    virtual void field_changed(T &source, const std::string &_field_name) = 0;
};

template <class T>
class safe_observable
{
    std::vector<observer<T> *> m_observers;
    using mutex_t = std::mutex;

    mutex_t mtx;

public:
    void notify(T &source, const std::string &_field_name)
    {
        std::scoped_lock<mutex_t> lock{mtx};
        for (auto obs : m_observers)
        {
            obs->field_changed(source, _field_name);
        }
    }

    void subscribe(observer<T> &obs)
    {
        std::scoped_lock<mutex_t> lock{mtx};
        m_observers.push_back(&obs);
    }

    void un_subscribe(observer<T> &obs)
    {
        std::scoped_lock<mutex_t> lock{mtx};
        m_observers.erase(remove(m_observers.begin(), m_observers.end(), obs), m_observers.end());
    }
};

class person : public safe_observable<person>
{
    std::string m_name;
    uint8_t m_age;

public:
    uint8_t get_age() const { return m_age; }
    void set_age(uint8_t _age)
    {
        if (m_age == _age)
            return;
        m_age = _age;
        notify(*this, "age");
    }
};

class console_person_observer : public observer<person>
{
public:
    void field_changed(person &source, const std::string &_field_name) override
    {
        if (_field_name == "age")
        {
            std::cout << int(source.get_age()) << std::endl;
        }
    }
};

int main()
{
    person p;
    console_person_observer console;
    p.subscribe(console);
    p.set_age(10);
}