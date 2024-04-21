#include "utils/SocketUtils.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <errno.h>

#include "gmock/gmock.h"

using namespace utils;
using namespace testing;

class RaiiFd
{
public:
    RaiiFd(const char *pathname, int flags, mode_t mode): fd{open(pathname, flags, mode)}
    {
        if(fd == -1)
        {
            throw std::runtime_error{std::string{"RaiiFd::RaiiFd: "} + strerror(errno)};
        }
    }

    void write(const std::string& data) const
    {
        if(::write(fd, data.c_str(), data.length()) != data.length())
        {
            throw std::runtime_error{std::string{"RaiiFd:write: "} + data + " " + strerror(errno)};
        }
        ::fsync(fd);
    }

    std::string readLineWrapper() const
    {
        return readLine(fd);
    }

    ~RaiiFd()
    {
        close(fd);
    }

private:
    const int fd{-1};
};

class SocketUtilsTest : public Test
{
public:
    void TearDown() override
    {
        std::filesystem::remove(dataFile);
    }

protected:
    inline const static std::string dataFile{"data.txt"};
    RaiiFd wfd{dataFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR};
};



TEST_F(SocketUtilsTest, GivenClosedFileReadLineShouldReturnEmptyString)
{
    int faulty_fd =  open(dataFile.c_str(), O_RDONLY, 0);
    close(faulty_fd);

    EXPECT_THROW(readLine(faulty_fd), std::runtime_error);
}

TEST_F(SocketUtilsTest, GivenEmptyInputReadLineShouldReturnEmptyString)
{
    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};
    const auto s = rfd.readLineWrapper();

    EXPECT_THAT(s, Eq(""));
}


TEST_F(SocketUtilsTest, GivenNewLineInputReadLineShouldReturnEmptyString)
{
    wfd.write("\n");

    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};
    const auto l = rfd.readLineWrapper();

    EXPECT_THAT(l, Eq("\n"));
}

TEST_F(SocketUtilsTest, GivenSingleLineWithoutNewLineInputReadLineShouldReturnThatLine)
{
    wfd.write("Hello World");

    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};
    const auto l = rfd.readLineWrapper();

    EXPECT_THAT(l, Eq("Hello World"));
}

TEST_F(SocketUtilsTest, GivenSingleLineWithNewLineInputReadLineShouldReturnThatLine)
{
    wfd.write("Hello World\n");

    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};
    const auto l = rfd.readLineWrapper();

    EXPECT_THAT(l, Eq("Hello World\n"));
}

TEST_F(SocketUtilsTest, GivenTwoLinesWithoutNewLineInputReadLineShouldReturnThatLines)
{
    wfd.write("Hello World1\nHello World2");

    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};

    auto l1 = rfd.readLineWrapper();
    EXPECT_THAT(l1, Eq("Hello World1\n"));

    auto l2 = rfd.readLineWrapper();
    EXPECT_THAT(l2, Eq("Hello World2"));
}

TEST_F(SocketUtilsTest, GivenTwoLinesWithNewLineInputReadLineShouldReturnThatLines)
{
    wfd.write("Hello World1\nHello World2\n");

    RaiiFd rfd{dataFile.c_str(), O_RDONLY, 0};

    auto l1 = rfd.readLineWrapper();
    EXPECT_THAT(l1, Eq("Hello World1\n"));

    auto l2 = rfd.readLineWrapper();
    EXPECT_THAT(l2, Eq("Hello World2\n"));
}
