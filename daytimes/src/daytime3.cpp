#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <asio.hpp>

using asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  /// @brief defines a shared pointer to be used later for the creation of the connection_tcp and names it pointer
  typedef std::shared_ptr<tcp_connection> pointer;
  /// defines pointer behavior, returns a new shared pointer to a tcp_connection using the a passed io_context
  /// and the tcp_connection's constructor
  static pointer create(asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }
  /// @return returns a reference to the tcp_connection's socket value. 
  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    /// initiates the daytime service and writes the message through the socket
    message_ = make_daytime_string();

    asio::async_write(socket_, asio::buffer(message_),
        std::bind(&tcp_connection::handle_write, shared_from_this(),
          asio::placeholders::error,
          asio::placeholders::bytes_transferred));
  }

private:
  /// @brief tcp_connection constructor, essentially a tcp socket with some specified behaviors.
  /// @param io_context 
  tcp_connection(asio::io_context& io_context)
    : socket_(io_context)
  {
  }
  /// @brief empty completion handler for the start() function.
  /// could handle exeptions or take in some other value though in this example is a placeholder.
  void handle_write(const std::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};
/// @brief basic tcp_server class
class tcp_server
{
public:
  /// @brief basic tcp_server class constructor. Calls on the start_accept function
  /// @param io_context passes by reference a io_context to be used later
  /// acceptor_ takens in the acceptor and creates and acceptor based on the io context.
  tcp_server(asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

private:
  /// @brief creates a new connection. Assigns a seperate instance of the same io context
  /// (basically making a thread) using a shared pointer.
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);
    /// derefences the new_connection socket and uses it for the async accept operations.
    /// sets the completion handler to call on the start() function in the tcp_connection once the async_accept
    /// makes a connection.
    acceptor_.async_accept(new_connection->socket(),
        std::bind(&tcp_server::handle_accept, this, new_connection, 
          asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const std::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }
    /// once the old new_connection is dealt with the cycle restarts for a new connection. (allowing multiple)
    start_accept();
  }

  asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}