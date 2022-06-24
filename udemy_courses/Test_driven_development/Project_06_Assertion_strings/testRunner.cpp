#include <iostream>
#include "LibraryCode.hpp"
#include "gtest/gtest.h"




TEST(TestToUpperString, normal)
{
        // 1. Arrange
	char inputString[] = "Hello World";
	// 2. act
	toUpper(inputString);
	// 3. assert
	//EXPECT_EQ("HELLO WORLD", inputString); //wrong way, false due to value of inputString and string "HELLO WORLD" is a address.
	EXPECT_STREQ("HELLO WORLD", inputString);
	// or	
	EXPECT_STRCASEEQ("HELLO WORLD", inputString);	
	// or
	EXPECT_EQ("HELLO WORLD", std::string(inputString));	
}


int main(int argc, char** argv){
	std::cout << "test runner is running \n";
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
