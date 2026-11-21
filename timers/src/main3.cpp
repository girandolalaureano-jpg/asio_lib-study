#include <functional>
#include <iostream>
#include <asio.hpp>

//recursive print function that counts to five and runs asynchronous to main.
void print (const std::error_code& /*e*/, asio::steady_timer* t, int* c){
    if (*c < 6){
        std::cout << "\r" << std::to_string(*c);
        ++(*c);
        t->expires_at(t->expiry() + asio::chrono::seconds(1)); //adds a second to the T timer.
        t->async_wait(std::bind(print, asio::placeholders::error, t, c));
    }
    std::cout <<"\nCount complete!";
}

int main()
{
    //io context/ executor
    asio::io_context timer_3;
    //counter
    int count = 0;
    //initial timer
    asio::steady_timer t(timer_3, asio::chrono::seconds(1));
    t.async_wait(std::bind(print, asio::placeholders::error, &t, &count));

    timer_3.run();
    return EXIT_SUCCESS;
}