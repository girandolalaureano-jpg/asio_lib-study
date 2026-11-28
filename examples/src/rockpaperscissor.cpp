#include <string>
#include <memory>
#include <asio.hpp>
#include <iostream>
#include <functional>
#include <ctime>
#include <random>

/// Small test application consisting of a simple rock paper scissors server
/// connects using tcp and asynchronous (mostly.) operation to recieve new users
/// and take in their input and display whether they won against a random user.

std::string get_time(){
    std::time_t now = std::time(0);
    return ctime(&now);
}

enum RPS{
    rock,
    paper,
    scissor
};

int random_pick(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(rock, scissor);
    return distr(gen);
}

class RockPaperScissor
{
    private:
    

    
    bool match_;

    public:
    RockPaperScissor() : num_picked_(random_pick()){};
    int num_picked_;
    bool match(int play){
        if (play == 0 && num_picked_ == 2){
            return match_ = TRUE;
        } else if (play == 2 && num_picked_ == 0){
            return match_ = FALSE;
        } else if (play > num_picked_){
            return match_ = TRUE;
        } else {
            return match_ = FALSE;
        }
    }
};

class player : public std::enable_shared_from_this<player>
{
    
    public:
    typedef std::shared_ptr<player> player_ptr;

    static player_ptr create(asio::io_context& io){
        return player_ptr(new player(io));
    }

    asio::ip::tcp::socket& socket(){
        return socket_;
    }

    void start(){


        message_ = "Hi!\nWelcome to ROCKPAPERSERVERS!\nSend rock[0], paper[1], scissor[2]\n";
        asio::async_write(socket_, asio::buffer(message_),
        std::bind(&player::handle_write, shared_from_this(),
        asio::placeholders::error,
        asio::placeholders::bytes_transferred));
    }

    void response(){
        std::array<char, 1> buf;
        
        asio::read(socket_ , asio::buffer(buf), 1);
        int play = std::stoi(std::string(buf.data(), 1));
        std::string plays[] = {"rock", "paper", "scissor"};
        if(a.match(play)){
            message_ = "You played: " + plays[play] + " Machine played: " + plays[a.num_picked_] + " You won!\n";
        } else {
            message_ = "You played: " + plays[play] + " Machine played: " + plays[a.num_picked_] + " You lost.\n";
        }

        asio::async_write(socket_, asio::buffer(message_),
        std::bind(&player::handle_response, shared_from_this(),
        asio::placeholders::error,
        asio::placeholders::bytes_transferred));

    }
    ~player(){
            std::cerr << "[Player Object destroyed]";
        };
    private:
    player(asio::io_context& io) : socket_(io){
        std::cerr << "[Player Object created]";
    };

    void handle_write(const std::error_code& /**/, size_t /**/){
        response();
    };

    void handle_response(const std::error_code& /**/, size_t /**/){
    };
   
    

    std::string message_;
    RockPaperScissor a;
    asio::ip::tcp::socket socket_;
};

class server
{
    public:
    server(asio::io_context& io) : io_context_(io), acceptor_(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13))
    {
        start_server();
    }

    
    private:
    void start_server(){
        auto new_player = player::create(io_context_);
        acceptor_.async_accept(new_player->socket(), std::bind(&server::handle_accept, this, new_player,
        asio::placeholders::error));
    };

    void handle_accept(player::player_ptr new_player, const std::error_code& error)
    {
        if (!error){
            new_player->start();
        }
        start_server();
    };
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]){
    try
    {
        //asio::streambuf buf;
        asio::io_context context;
        server server(context);
        context.run();
        


    } catch(std::exception& e)
    {
        std::cerr << "Encountered an error: " << e.what() << std::endl;
    }
    return 0;
}