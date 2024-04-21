#include "client/TrafficStorage.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <netinet/in.h>

#include "gmock/gmock.h"

using namespace client;
using namespace testing;


class TrafficStorageTest : public Test
{
protected:
    std::string get_storage_output(const TrafficStorage& trafficStorage) const
    {
        std::stringstream buffer;
        buffer << trafficStorage;

        return buffer.str();
    }

    inline static const std::string interface{"eth1"};
    inline static const std::string ip_address{"127.0.0.1"};

    const uint32_t source_ip = htonl(INADDR_LOOPBACK + 1);
    const uint32_t source_ip2 = htonl(INADDR_LOOPBACK + 2);
    const uint32_t dest_ip = htonl(INADDR_LOOPBACK);
    const uint16_t source_port = 2;
    const uint16_t source_port2 = 3;
    const uint16_t dest_port = 1;
    const uint16_t dest_port2 = 2;
};

TEST_F(TrafficStorageTest, givenZeroSizeStorageShouldThrowException)
{
    const size_t storage_size{0};
    try
    {
        TrafficStorage sut{interface, ip_address, storage_size};
        FAIL() << "Throwing exception of type runtime_error was expected!!!" << std::endl;
    }
    catch (const std::runtime_error&) {

    }
}

class SingleElementTrafficStorageTest : public TrafficStorageTest
{
protected:
    inline static const size_t storage_size{1};

    TrafficStorage sut{interface, ip_address, storage_size};
};


TEST_F(SingleElementTrafficStorageTest, When_Empty_OutputsEmptyData)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": []\n"
                                        "}\n";

    const std::string result = get_storage_output(sut);

    EXPECT_THAT(result, Eq(expected_output));
}

TEST_F(SingleElementTrafficStorageTest, When_UpdatedOnce_OutputSingleElement)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": [\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:00 1970\"\n"
                                        "    }\n"
                                        "  ]\n"
                                        "}\n";

    sut.update(TrafficData{0, source_ip, source_port, dest_ip, dest_port});

    EXPECT_THAT(get_storage_output(sut), Eq(expected_output));
}

TEST_F(SingleElementTrafficStorageTest, When_UpdatedTwice_OutputElementAddedAsLast)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": [\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:01 1970\"\n"
                                        "    }\n"
                                        "  ]\n"
                                        "}\n";

    sut.update(TrafficData{0, source_ip, source_port, dest_ip, dest_port});
    sut.update(TrafficData{1, source_ip, source_port, dest_ip, dest_port});

    EXPECT_THAT(get_storage_output(sut), Eq(expected_output));
}


class MultiElementTrafficStorageTest : public TrafficStorageTest
{
protected:
    inline static const size_t storage_size{2};

    TrafficStorage sut{interface, ip_address, storage_size};
};

TEST_F(MultiElementTrafficStorageTest, When_Empty_OutputsEmptyData)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": []\n"
                                        "}\n";

    const std::string result = get_storage_output(sut);

    EXPECT_THAT(result, Eq(expected_output));
}

TEST_F(MultiElementTrafficStorageTest, When_UpdatedOnce_OutputSingleElement)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": [\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:00 1970\"\n"
                                        "    }\n"
                                        "  ]\n"
                                        "}\n";

    sut.update(TrafficData{0, source_ip, source_port, dest_ip, dest_port});

    EXPECT_THAT(get_storage_output(sut), Eq(expected_output));
}

TEST_F(MultiElementTrafficStorageTest, When_UpdatedTwice_OutputTwoElements)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": [\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:00 1970\"\n"
                                        "    },\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:01 1970\"\n"
                                        "    }\n"
                                        "  ]\n"
                                        "}\n";

    sut.update(TrafficData{0, source_ip, source_port, dest_ip, dest_port});
    sut.update(TrafficData{1, source_ip, source_port, dest_ip, dest_port});

    EXPECT_THAT(get_storage_output(sut), Eq(expected_output));
}

TEST_F(MultiElementTrafficStorageTest, When_UpdatedOverTheLimit_DropsTheOldestElement)
{
    const std::string expected_output = "{\n"
                                        "  \"stat\": [\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:01 1970\"\n"
                                        "    },\n"
                                        "    {\n"
                                        "      \"dst_ip\": \"127.0.0.1\",\n"
                                        "      \"dst_port\": \"1\",\n"
                                        "      \"src_ip\": \"127.0.0.2\",\n"
                                        "      \"src_port\": \"2\",\n"
                                        "      \"time\": \"Thu Jan  1 01:00:02 1970\"\n"
                                        "    }\n"
                                        "  ]\n"
                                        "}\n";

    sut.update(TrafficData{0, source_ip, source_port, dest_ip, dest_port});
    sut.update(TrafficData{1, source_ip, source_port, dest_ip, dest_port});
    sut.update(TrafficData{2, source_ip, source_port, dest_ip, dest_port});

    EXPECT_THAT(get_storage_output(sut), Eq(expected_output));
}
