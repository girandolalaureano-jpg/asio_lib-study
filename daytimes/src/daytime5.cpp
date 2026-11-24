#include <array>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

std::string make_daytime_string()
{
    std::time_t now = time(0);
    return ctime(&now);
};

int main()
{
    try
    {
        asio::io_context udp_context;
        asio::ip::udp::socket socket(udp_context, asio::ip::udp::endpoint(asio::ip::udp::v4() , 13));

        for(;;){
            std::array<char, 1> recv_buf;
            asio::ip::udp::endpoint remote_endpoint;
            socket.receive_from(asio::buffer(recv_buf), remote_endpoint);

            std::string message = make_daytime_string();

            std::array<char, 128> send_buf;
            std::error_code ignored;
            socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored);

        }
    }
    catch (std::exception &e)
    {
        std::cerr << "An error occured: \t" << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}