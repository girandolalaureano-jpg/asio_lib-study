#include <ctime>
#include <string>
#include <iostream>
#include <asio.hpp>

/// @return Returns a string composed of the date + time
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try
    {
        asio::io_context io;
        asio::ip::tcp::acceptor acceptor(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));
        
        for(;;)
        {
            /// creates the socket, the entry point your client will go through.
            asio::ip::tcp::socket socket(io);
            /// tells the asio context to accept connections made with the specific port
            /// both socket and accept work to make the socket and connection entry for the user
            /// if kept outside the loop, they never go out of scope meaning the connection is never closed.
            acceptor.accept(socket);
            std::string message = make_daytime_string();
            asio::error_code ignored;
            asio::write(socket, asio::buffer(message), ignored);
        };
    }
    catch (std::exception& e)
    {
        std::cerr << "Error " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}