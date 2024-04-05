#include "tests.hpp"

namespace
{
    class BinaryTests : public ::testing::Test
    {

    protected:
        BinaryTests()
        {
        }

        virtual ~BinaryTests()
        {
        }

        virtual void SetUp()
        {
            std::cout << "SetUp" << std::endl;
        }

        virtual void TearDown()
        {
            std::cout << "TearDown" << std::endl;
        }
    };
}

TEST(ArgumentParser, ParseArguments)
{
}