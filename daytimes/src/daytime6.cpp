#include <array>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

std::string make_daytime_string(){
    std::time_t now = time(0);
    return ctime(&now);
}

class udp_server
{
public:
    /// @brief Constructor for the udp server, calls on start receive by passing the execution context.
    /// @param io_context 
    udp_server(asio::io_context& io_context) : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 13))
    {
        start_receive();
    }
private:
    ///calls on the socket_ member of the obj to start receiving using the previous buffer
    ///and the endpoint declared within the class.
    ///once it receives something calls on the handle_receive to handle that entry.
    void start_receive()
    {
        socket_.async_receive_from(recv_buffer_ , remote_endpoint_, 
        std::bind(&udp_server::handle_receive, this,
        asio::placeholders::error,
        asio::placeholders::bytes_transferred));
    };
    /// @brief completion handler for start_receive creates a new message string
    /// (via shared pointer) and tasks the socket_ to send the message through the same remote_endpoint_
    /// after that is done the server has sent the time and no longer is occupied. it calls on an empty completion
    /// handler and once again calls on start receive to begin the process again.
    void handle_receive(const std::error_code& error, std::size_t /*bytes transfered/unused*/)
    {
        if(!error)
        {
            std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
            socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
            std::bind(&udp_server::handle_send, this, message,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));

            start_receive();
        }
    }

    void handle_send(std::shared_ptr<std::string> /*message*/, std::error_code& /*error*/,
        std::size_t /*bytes*/)
    {
    };
/// socket, endpoint y buffer. The buffer simply works as a receiving end for the request.
asio::ip::udp::socket socket_;
asio::ip::udp::endpoint remote_endpoint_;
std::array<char , 1> recv_buffer_; 
};

int main()
{
    try
    {
        asio::io_context io_context;
        udp_server server(io_context);
        io_context.run();
    }
    catch(std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}