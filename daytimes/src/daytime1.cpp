#include <iostream>
#include <array>
#include <asio.hpp>

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return EXIT_FAILURE;
        }
        asio::io_context io;
        asio::error_code ec;
        ///makes a list of valid endpoints once called
        asio::ip::tcp::resolver resolver(io);
        ///cycles said list until it finds a connection
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(argv[1] , "daytime");
        ///regular socket
        asio::ip::tcp::socket socket(io);
        ///connection.
        asio::connect(socket, endpoints);

        for(;;)
        {
            ///the buffer we'll use for the message AND an error code.
            std::array<char, 128> buf;
            std::error_code error;

            size_t len = socket.read_some(asio::buffer(buf), ec);
            /// eof means end of file, or end of stream. Expected behavior as
            /// it's equivalen to FYN tcp.
            if(ec == asio::error::eof)
            {
                break; 
            } else if (error)
            {
                ///Throws an error to the exception catcher below.
                throw std::system_error(error);
            }
            ///writes to console (cout) the buffer according to it's length.
            std::cout.write(buf.data(), len);
        }

    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
