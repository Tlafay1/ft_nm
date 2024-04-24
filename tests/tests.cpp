#include "tests.hpp"

TEST(ReadFile, FileDoesntExist)
{
    const char *argv[] = {"./ft_nm", "binaries/dontexist", NULL};
    ASSERT_EQ(ft_nm_main(argv), 1);
}

TEST(ReadFile, FileIsDirectory)
{
    const char *argv[] = {"./ft_nm", "binaries", NULL};
    ASSERT_EQ(ft_nm_main(argv), 1);
}
