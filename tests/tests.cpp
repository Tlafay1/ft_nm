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

// TEST(ReadFile, FileIsAnElfWithOptionG)
// {
//     const char *argv[] = {"./ft_nm", "-g", "tests/binaries/hello", NULL};
//     ::testing::internal::CaptureStdout();
//     int ret = ft_nm_main(argv);
//     std::string output = ::testing::internal::GetCapturedStdout();
//     ASSERT_EQ(ret, 0);
//     ASSERT_EQ(output, "");
// }

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

std::string FileIsAnElf_output_with_option_r = R""""(0000000000401000 T _start
0000000000402000 T _end
0000000000402000 T _edata
0000000000402000 T __bss_start
)"""";

TEST(ReadFile, FileIsAnElfWithOptionR)
{
    const char *argv[] = {"./ft_nm", "-r", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::cout << output;
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, FileIsAnElf_output_with_option_r);
}

std::string FileIsAnElf_output_with_option_u = R""""(                 U free@@GLIBC_2.2.5
                 U putchar@@GLIBC_2.2.5
                 U __errno_location@@GLIBC_2.2.5
                 w _ITM_deregisterTMCloneTable
                 U puts@@GLIBC_2.2.5
                 U ft_lstclear
                 U ft_strlen
                 U ft_lstadd_back
                 U ft_lstnew
                 U __stack_chk_fail@@GLIBC_2.4
                 U mmap@@GLIBC_2.2.5
                 U printf@@GLIBC_2.2.5
                 U close@@GLIBC_2.2.5
                 U __libc_start_main@@GLIBC_2.2.5
                 U ft_isalnum
                 w __gmon_start__
                 U malloc@@GLIBC_2.2.5
                 U __fxstat@@GLIBC_2.2.5
                 U munmap@@GLIBC_2.2.5
                 U ft_tolower
                 U open@@GLIBC_2.2.5
                 U ft_strjoin
                 U ft_memcmp
                 U ft_strncmp
                 U ft_lstadd_sorted
                 w _ITM_registerTMCloneTable
                 U ft_lstiter
                 w __cxa_finalize@@GLIBC_2.2.5
)"""";

TEST(ReadFile, FileIsAnElfWithOptionU)
{
    const char *argv[] = {"./ft_nm", "-u", "tests/binaries/valid", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, FileIsAnElf_output_with_option_u);
}

TEST(ReadFile, FileIsAnElfWithOptionUWithNoObjects)
{
    const char *argv[] = {"./ft_nm", "-u", "tests/binaries/hello", NULL};
    ::testing::internal::CaptureStdout();
    int ret = ft_nm_main(argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output, "");
}
