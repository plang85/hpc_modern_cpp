#include "gtest/gtest.h"

#include <iostream>


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::internal::CaptureStdout();
    auto const i = RUN_ALL_TESTS();
    std::cout << ::testing::internal::GetCapturedStdout();
    return i;
}