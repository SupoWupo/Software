/**
 * main function for all time tests
 */

#include <gtest/gtest.h>

#include <iostream>

int main(int argc, char **argv)
{
    std::cout << argv[0] << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
