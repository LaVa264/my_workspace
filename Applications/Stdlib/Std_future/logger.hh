#pragma once
#include <iostream>
#define _LOG std::cout << std::boolalpha << "[" << __FILE__ << ":" << __LINE__ << "][" << __FUNCTION__ << "][tid:" << std::this_thread::get_id() << "]: "