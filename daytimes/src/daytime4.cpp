#include <array>
#include <iostream>
#include <asio.hpp>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2){
            std::cout << "Usage client <host>" << std::endl;
            return EXIT_FAILURE;
        }
        
        asio::io_context context;
        asio::ip::udp::resolver resolver(context);
        asio::ip::udp::endpoint endpoint_reciever = *resolver.resolve(asio::ip::udp::v4(),
        argv[1], "daytime").begin();

        asio::ip::udp::socket socket(context);
        socket.open(asio::ip::udp::v4());

        std::array<char, 1> send_buf = {{0}};
        socket.send_to(asio::buffer(send_buf), endpoint_reciever);
        
        std::array<char, 128> recv_buf;
        asio::ip::udp::endpoint endpoint_sender;
        size_t len = socket.receive_from(asio::buffer(recv_buf), endpoint_sender);

        std::cout.write(recv_buf.data(), len);

    }
    catch (std::exception& e)
    {
        std::cerr << "Error occured: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}