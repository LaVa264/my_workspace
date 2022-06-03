#include <iostream>
#include "LibraryCode.hpp"
#include "gtest/gtest.h"


TEST(TestCountPositives, BasicTest)
{
	// 1. Arrange
	std::vector<int> inputVector {1, 2 , 3 , -1, -2};
	// 2. Action
	int count = countPositives(inputVector);

	// 3. assert
	ASSERT_EQ(3, count);
}

TEST(TestCountPositives, EmptyVectorTest)
{
        // 1. Arrange
        std::vector<int> inputVector { -1, -2 , -3, -1, -2};
        // 2. Action
        int count = countPositives(inputVector);

        // 3. assert
        // ASSERT_* stop testing if count != 0 (expression is false) 
	// ASSERT_EQ(0, count);
	
	// EXPECT_ continue testing even though expression is false
	EXPECT_EQ(0, count);
	
	std::cout << "Finish Test empty Vector Test \n";
}


int main(int argc, char** argv){
	std::cout << "test runner is running \n";
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
