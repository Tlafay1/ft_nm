#include "tests.hpp"

class BinariesTests : public ::testing::Test
{

protected:
    void SetUp() override
    {
        std::cout << "SetUp" << std::endl;
    }

    void TearDown() override
    {
        std::cout << "TearDown" << std::endl;
    }
};

TEST(BinariesTests, ParseArguments)
{
    std::cout << "IURHIGUHERIUGHIRG" << std::endl;
}