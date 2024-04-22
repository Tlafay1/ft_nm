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
    const char *args[3] = {"./ft_nm", "tests/binaries/valid", nullptr};
    testing::internal::CaptureStdout();
    ft_nm_main(args);
    std::string output = testing::internal::GetCapturedStdout();
    testing::internal::CaptureStdout();
    system("nm tests/binaries/valid");
    std::string expected = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, expected);
}