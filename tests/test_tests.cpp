#include <gtest/gtest.h>

// Include the header file for the code you want to test
#include "ft_nm.h"

// Define a test fixture
class FtNmTests : public ::testing::Test
{
protected:
    // Set up the test fixture
    void SetUp() override
    {
        // Perform any necessary setup for the tests
    }

    // Tear down the test fixture
    void TearDown() override
    {
        // Perform any necessary cleanup after the tests
    }
};

// Define a test case
TEST_F(FtNmTests, Test1)
{
    // Arrange: Set up the necessary inputs for the test

    // Act: Call the function you want to test

    // Assert: Check the expected output or behavior
    ASSERT_TRUE(true); // Replace with your actual assertions
}

TEST_F(FtNmTests, Test2)
{
    // Arrange: Set up the necessary inputs for the test

    // Act: Call the function you want to test

    // Assert: Check the expected output or behavior
    ASSERT_TRUE(true); // Replace with your actual assertions
}

// Add more test cases as needed

// Run the tests
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}