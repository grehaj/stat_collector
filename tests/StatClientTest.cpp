#include "client/StatClient.h"
#include "utils/Consts.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "mocks/ServerGuardMock.h"
#include "mocks/SocketMock.h"
#include "mocks/SystemMock.h"

using namespace client;
using namespace testing;
using namespace std::string_literals;

class StatClientTest : public Test
{
public:
    StatClientTest()
    {
        sa.sa_family = 11;
        addrinfo_result.ai_family = AF_INET;
        addrinfo_result.ai_socktype = SOCK_STREAM;
        addrinfo_result.ai_protocol = 100;
        addrinfo_result.ai_addr = &sa;
        addrinfo_result.ai_addrlen = 20;

    }
    std::unique_ptr<utils::SocketMock> socketMock = std::make_unique<utils::SocketMock>();
    utils::SocketMock* socketMockPtr = socketMock.get();

    std::unique_ptr<utils::SystemtMock> systemMock = std::make_unique<utils::SystemtMock>();
    utils::SystemtMock* systemtMockPtr = systemMock.get();

    addrinfo addrinfo_result{};
    sockaddr sa{};
    const int cerror = -1;
    const int csuccess = 0;
    const int socket_id = 112;
    const std::string ifc = "ifc";
    const std::string ip = "1.1.1.1";
    utils::storage_size_t storage_size = 2;
    std::map<std::string, std::string> ifcs = {{ifc, ip}};
};


TEST_F(StatClientTest, given_getaddrinfo_Fail_shouldThrowWhenCreated)
{
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(Return(128));

    try {
        StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatClientTest, given_interface_ip_Fail_shouldThrowWhenCreated)
{
    std::map<std::string, std::string> empty_ifcs = {};
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(empty_ifcs));

    try {
        StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatClientTest, given_getaddrinfo_ReturnsNoAddr_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(Return(0));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));

    try {
        StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatClientTest, given_SocketFailsAndNoOtherAddressAvailable_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(-1));

    try {
        StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatClientTest, given_ConnectFailsAndNoOtherAddressAvailable_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, connect(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, close(socket_id));

    try {
        StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatClientTest, given_SocketCreated_shouldFreeAddrInfo)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, connect(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&addrinfo_result));

    StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
}

TEST_F(StatClientTest, given_SocketFailsFirstTime_shouldContinueWithNextAddress)
{
    addrinfo multi_addrinfo_result{};
    multi_addrinfo_result.ai_next = &addrinfo_result;
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&multi_addrinfo_result), Return(0)));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, socket(multi_addrinfo_result.ai_family, multi_addrinfo_result.ai_socktype, multi_addrinfo_result.ai_protocol)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, connect(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&multi_addrinfo_result));

    StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
}

TEST_F(StatClientTest, given_ConnectFailsFirstTime_shouldContinueWithNextAddress)
{
    addrinfo multi_addrinfo_result{};
    multi_addrinfo_result.ai_next = &addrinfo_result;
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&multi_addrinfo_result), Return(0)));
    EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
    EXPECT_CALL(*socketMockPtr, socket(multi_addrinfo_result.ai_family, multi_addrinfo_result.ai_socktype, multi_addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, connect(socket_id, multi_addrinfo_result.ai_addr, multi_addrinfo_result.ai_addrlen)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, close(socket_id));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, connect(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&multi_addrinfo_result));

    StatClient{std::move(socketMock), std::move(systemMock), ifc, storage_size};
}

MATCHER_P(HasWritten, written_str, "")
{
    return std::string{static_cast<char*>(arg), written_str.size()} == written_str;
}



class StatClientRunTest : public StatClientTest
{
public:
    StatClientRunTest()
    {
        EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
        EXPECT_CALL(*systemtMockPtr, get_active_interfaces_ip()).WillOnce(Return(ifcs));
        EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
        EXPECT_CALL(*socketMockPtr, connect(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));

        EXPECT_CALL(*socketMockPtr, freeaddrinfo(&addrinfo_result));
    }

    void expectWrite(const std::string& msg) const
    {
        EXPECT_CALL(*socketMockPtr, write(socket_id, HasWritten(msg), msg.size())).WillOnce(Return(msg.size()));
        EXPECT_CALL(*socketMockPtr, write(socket_id, HasWritten("\n"s), 1)).WillOnce(Return(1));
    }

    std::unique_ptr<StatClient> sut;
};

TEST_F(StatClientRunTest, given_PopenReurnsNull_ShouldThrow)
{
    sut = std::make_unique<StatClient>(std::move(socketMock), std::move(systemMock), ifc, storage_size);
    EXPECT_CALL(*systemtMockPtr, popen(_, _)).WillOnce(Return(static_cast<FILE*>(0)));

    EXPECT_THROW(sut->run(), std::runtime_error);
}

class SingleElementStatClientRunTest : public StatClientRunTest
{
public:
    SingleElementStatClientRunTest()
    {
        utils::storage_size_t storage_size = 1;
        sut = std::make_unique<StatClient>(std::move(socketMock), std::move(systemMock), ifc, storage_size);
    }
};

TEST_F(SingleElementStatClientRunTest, given_PopenReurnsProperFile_ShouldCollectDataAndSendItViaSocket)
{
    FILE* f = ::popen("echo '1718440530.187770 IP 1.1.1.1.101 > 2.2.2.2.102'", "r");
    EXPECT_CALL(*systemtMockPtr, popen(_, _)).WillOnce(Return(static_cast<FILE*>(f)));

    {
        InSequence is;
        expectWrite("{");
        expectWrite("  \"stat\": [");
        expectWrite("    {");
        expectWrite("      \"dst_ip\": \"2.2.2.2\",");
        expectWrite("      \"dst_port\": \"102\",");
        expectWrite("      \"src_ip\": \"1.1.1.1\",");
        expectWrite("      \"src_port\": \"101\",");
        expectWrite("      \"time\": \"Sat Jun 15 10:35:30 2024\"");
        expectWrite("    }");
        expectWrite("  ]");
        expectWrite("}");
    }
    sut->run();
}

class MultiElementStatClientRunTest : public StatClientRunTest
{
public:
    MultiElementStatClientRunTest()
    {
        utils::storage_size_t storage_size = 2;
        sut = std::make_unique<StatClient>(std::move(socketMock), std::move(systemMock), ifc, storage_size);
    }
};

TEST_F(MultiElementStatClientRunTest, given_PopenReurnsProperFile_ShouldCollectDataAndSendItViaSocket)
{
    FILE* f = ::popen("echo '1718440530.187770 IP 1.1.1.1.101 > 2.2.2.2.102\n"
                            "1718440531.187770 IP 3.3.3.3.103 > 4.4.4.4.104'", "r");
    EXPECT_CALL(*systemtMockPtr, popen(_, _)).WillOnce(Return(static_cast<FILE*>(f)));

    {
        InSequence is;
        expectWrite("{");
        expectWrite("  \"stat\": [");
        expectWrite("    {");
        expectWrite("      \"dst_ip\": \"2.2.2.2\",");
        expectWrite("      \"dst_port\": \"102\",");
        expectWrite("      \"src_ip\": \"1.1.1.1\",");
        expectWrite("      \"src_port\": \"101\",");
        expectWrite("      \"time\": \"Sat Jun 15 10:35:30 2024\"");
        expectWrite("    },");
        expectWrite("    {");
        expectWrite("      \"dst_ip\": \"4.4.4.4\",");
        expectWrite("      \"dst_port\": \"104\",");
        expectWrite("      \"src_ip\": \"3.3.3.3\",");
        expectWrite("      \"src_port\": \"103\",");
        expectWrite("      \"time\": \"Sat Jun 15 10:35:31 2024\"");
        expectWrite("    }");
        expectWrite("  ]");
        expectWrite("}");
    }
    sut->run();
}
