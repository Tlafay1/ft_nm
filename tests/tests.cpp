#include "tests.hpp"

TEST(ReadFile, FileDoesntExist)
{
    const char *argv[] = {"./ft_nm", "tests/binaries/dontexist", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(output, "./ft_nm: 'tests/binaries/dontexist': No such file\n");
}

TEST(ReadFile, FileIsDirectory)
{
    const char *argv[] = {"./ft_nm", "tests/binaries", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(output, "./ft_nm: Warning: 'tests/binaries' is a directory\n");
}

TEST(ReadFile, FileIsEmpty)
{
    const char *argv[] = {"./ft_nm", "tests/binaries/empty", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(output, "");
}

TEST(ReadFile, FileIsNotAnElf)
{
    const char *argv[] = {"./ft_nm", "tests/binaries/notanelf", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 1);
    ASSERT_EQ(output, "./ft_nm: tests/binaries/notanelf: file format not recognized\n");
}

std::string FileIsAnElf_output = R""""(0000000000402000 T __bss_start
0000000000402000 T _edata
0000000000402000 T _end
0000000000401000 T _start
)"""";

TEST(ReadFile, FileIsAnElf)
{
    const char *argv[] = {"./ft_nm", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, FileIsAnElf_output);
}

TEST(ReadFile, FileIsAnElfWithOptionA)
{
    const char *argv[] = {"./ft_nm", "-a", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, FileIsAnElf_output);
}

TEST(ReadFile, FileIsAnElfWithOptionG)
{
    const char *argv[] = {"./ft_nm", "-g", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, "");
}

std::string FileIsAnElf_output_with_option_p = R""""(0000000000401000 T _start
0000000000402000 T __bss_start
0000000000402000 T _edata
0000000000402000 T _end
)"""";

TEST(ReadFile, FileIsAnElfWithOptionP)
{
    const char *argv[] = {"./ft_nm", "-p", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, FileIsAnElf_output_with_option_p);
}

TEST(ReadFile, FileIsAnElfWithOptionR)
{
    const char *argv[] = {"./ft_nm", "-r", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, "0000000000402000 T _end\n0000000000402000 T _edata\n0000000000402000 T __bss_start\n0000000000401000 T _start\n");
}

TEST(ReadFile, FileIsAnElfWithOptionU)
{
    const char *argv[] = {"./ft_nm", "-u", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, "");
}
