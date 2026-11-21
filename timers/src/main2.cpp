#include <asio.hpp>
#include <iostream>

//completion handler!
void print(const std::error_code& /*e*/){
    std::cout << "\rFinished asynchronous waiting!";
};

int main()
{
    asio::io_context timer_2;

    asio::steady_timer asyncWait(timer_2 , asio::chrono::seconds(5));
    std::cout << "Begining asynchronous wait!\n";
    //calls the completion handler
    asyncWait.async_wait(&print);
    /*since the wait is non-synchronous once we set the timer within
    io_context the main function can still work independently.
    */

    std::cout << "Wait a little more..";

    timer_2.run();
    return EXIT_SUCCESS;
}