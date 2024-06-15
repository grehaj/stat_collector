#include "server/StatServer.h"
#include "utils/Consts.h"

#include <memory>
#include <thread>

#include "mocks/ServerGuardMock.h"
#include "mocks/SocketMock.h"
#include "gmock/gmock.h"

using namespace server;
using namespace testing;

class StatServerTest : public Test
{
public:
    StatServerTest()
    {
        sa.sa_family = 11;
        addrinfo_result.ai_family = AF_INET;
        addrinfo_result.ai_socktype = SOCK_STREAM;
        addrinfo_result.ai_protocol = 100;
        addrinfo_result.ai_addr = &sa;
        addrinfo_result.ai_addrlen = 20;

    }
    std::unique_ptr<utils::SocketMock> socketMock = std::make_unique<utils::SocketMock>();
    std::unique_ptr<ServerRunGuardMock> serverGuardMock = std::make_unique<ServerRunGuardMock>();
    utils::SocketMock* socketMockPtr = socketMock.get();
    ServerRunGuardMock* serverRunGuardMockPtr = serverGuardMock.get();
    addrinfo addrinfo_result{};
    sockaddr sa{};
    const int cerror = -1;
    const int csuccess = 0;
    const int socket_id = 112;
};


TEST_F(StatServerTest, given_getaddrinfo_Fail_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(Return(128));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatServerTest, given_getaddrinfo_ReturnsNoAddr_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(Return(0));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatServerTest, given_SocketFailsAndNoOtherAddressAvailable_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(-1));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatServerTest, given_BindFailsAndNoOtherAddressAvailable_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, close(socket_id));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatServerTest, given_ListenFails_shouldThrowWhenCreated)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, listen(socket_id, utils::BACKLOG)).WillOnce(Return(cerror));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        FAIL();
    }  catch (const std::runtime_error&) {
        SUCCEED();
    }
}

TEST_F(StatServerTest, given_SocketCreated_shouldFreeAddrInfo)
{
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, listen(socket_id, utils::BACKLOG)).WillOnce(Return(csuccess));

    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&addrinfo_result));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        SUCCEED();
    }  catch (const std::runtime_error&) {
        FAIL();
    }
}

TEST_F(StatServerTest, given_SocketFailsFirstTime_shouldContinueWithNextAddress)
{
    addrinfo multi_addrinfo_result{};
    multi_addrinfo_result.ai_next = &addrinfo_result;
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&multi_addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(multi_addrinfo_result.ai_family, multi_addrinfo_result.ai_socktype, multi_addrinfo_result.ai_protocol)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, listen(socket_id, utils::BACKLOG)).WillOnce(Return(csuccess));

    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&multi_addrinfo_result));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        SUCCEED();
    }  catch (const std::runtime_error&) {
        FAIL();
    }
}

TEST_F(StatServerTest, given_BindFailsFirstTime_shouldContinueWithNextAddress)
{
    addrinfo multi_addrinfo_result{};
    multi_addrinfo_result.ai_next = &addrinfo_result;
    EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&multi_addrinfo_result), Return(0)));
    EXPECT_CALL(*socketMockPtr, socket(multi_addrinfo_result.ai_family, multi_addrinfo_result.ai_socktype, multi_addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, multi_addrinfo_result.ai_addr, multi_addrinfo_result.ai_addrlen)).WillOnce(Return(cerror));
    EXPECT_CALL(*socketMockPtr, close(socket_id));
    EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
    EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
    EXPECT_CALL(*socketMockPtr, listen(socket_id, utils::BACKLOG)).WillOnce(Return(csuccess));

    EXPECT_CALL(*socketMockPtr, freeaddrinfo(&multi_addrinfo_result));

    try {
        StatServer{std::move(socketMock), std::move(serverGuardMock)};
        SUCCEED();
    }  catch (const std::runtime_error&) {
        FAIL();
    }
}

class StatServerRunTest : public StatServerTest
{
public:
    StatServerRunTest()
    {
        EXPECT_CALL(*socketMockPtr, getaddrinfo(_, _, _, _)).WillOnce(DoAll(SetArgPointee<3>(&addrinfo_result), Return(0)));
        EXPECT_CALL(*socketMockPtr, socket(addrinfo_result.ai_family, addrinfo_result.ai_socktype, addrinfo_result.ai_protocol)).WillOnce(Return(socket_id));
        EXPECT_CALL(*socketMockPtr, bind(socket_id, addrinfo_result.ai_addr, addrinfo_result.ai_addrlen)).WillOnce(Return(csuccess));
        EXPECT_CALL(*socketMockPtr, listen(socket_id, utils::BACKLOG)).WillOnce(Return(csuccess));

        EXPECT_CALL(*socketMockPtr, freeaddrinfo(&addrinfo_result));

        sut = std::make_unique<StatServer>(std::move(socketMock), std::move(serverGuardMock));

    }

    std::unique_ptr<StatServer> sut;
    int client_socket_id =113;
};

TEST_F(StatServerRunTest, given_NotAllowedToRun_ShouldStop)
{
    EXPECT_CALL(*serverRunGuardMockPtr, should_stop()).WillOnce(Return(true));
    EXPECT_CALL(*socketMockPtr, accept(socket_id, _, _)).Times(0);

    sut->run();
}

TEST_F(StatServerRunTest, given_AcceptFails_ShouldTryAgain)
{
    {
        InSequence is;
        EXPECT_CALL(*serverRunGuardMockPtr, should_stop()).WillOnce(Return(false));
        EXPECT_CALL(*socketMockPtr, accept(socket_id, _, _)).WillOnce(Return(cerror));

        EXPECT_CALL(*serverRunGuardMockPtr, should_stop()).WillOnce(Return(true));
    }
    sut->run();
}

TEST_F(StatServerRunTest, given_AcceptSucceeds_ShouldReadData)
{
    {
        InSequence is;
        EXPECT_CALL(*serverRunGuardMockPtr, should_stop()).WillOnce(Return(false));
        EXPECT_CALL(*socketMockPtr, accept(socket_id, _, _)).WillOnce(Return(client_socket_id));
        EXPECT_CALL(*socketMockPtr, readLine(client_socket_id)).WillOnce(Return("Hello Socket"));
        EXPECT_CALL(*socketMockPtr, readLine(client_socket_id)).WillOnce(Return("of C++.\n"));
        EXPECT_CALL(*socketMockPtr, readLine(client_socket_id)).WillOnce(Return(""));
        EXPECT_CALL(*socketMockPtr, close(client_socket_id));
        EXPECT_CALL(*serverRunGuardMockPtr, should_stop()).WillOnce(Return(true));
    }
    sut->run();
}
