#include <functional>
#include <iostream>
#include <thread>
#include <asio.hpp>

class printer
{
    public:
        printer(asio::io_context& io) 
        : strand_(asio::make_strand(io)),
        timer1_(io, asio::chrono::seconds(1)),
        timer2_(io, asio::chrono::seconds(1)),
        count_(0)
        {
            ///calls to bind both timer objects as soon as they are made.
            ///calls onto the respective print funct while accessing the same count_
            timer1_.async_wait(asio::bind_executor(strand_,
                std::bind(&printer::print1, this)));
            timer2_.async_wait(asio::bind_executor(strand_,
                std::bind(&printer::print2, this)));
        };

        ~printer()
        {
            std::cout << "\rFinal Count is: " << count_;
        }

        void print1()
        {
            ///Checks if count_ is less than then, adds some and proceeds to add
            ///seconds to the asynchronous wait. once it completes it once again calls a completion handler
            ///to pass on the timer1_ and it's values to.
            if (count_ < 10)
            {
                std::cout << "Timer 1: " << count_ << std::endl;
                ++count_;

                timer1_.expires_at(timer1_.expiry() + asio::chrono::seconds(1));

                timer1_.async_wait(asio::bind_executor(strand_,
                     std::bind(&printer::print1, this)));
            }
        }

        void print2()
        {
            if(count_ < 10)
            {
                std::cout << "Timer 2: " << count_ << std::endl;
                ++count_;

                timer2_.expires_at(timer2_.expiry() + asio::chrono::seconds(1));

                timer2_.async_wait(asio::bind_executor(strand_,
                     std::bind(&printer::print2, this)));
            }
        }
    private:
        asio::strand<asio::io_context::executor_type> strand_;
        asio::steady_timer timer1_;
        asio::steady_timer timer2_;
        int count_;
};

int main(){
    asio::io_context io;

    //regular function call, prepares the work.
    printer p(io);
    //lambda function for the thread
    std::thread t([&]{io.run(); });
    //second run() call;
    io.run();

    //ensures the thread synchronize before terminating.
    t.join();
    return EXIT_SUCCESS;
}
