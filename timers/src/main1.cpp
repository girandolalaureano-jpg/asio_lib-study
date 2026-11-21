#include <asio.hpp>
#include <iostream>

int main()
{
    asio::io_context timer_1;

    asio::steady_timer timer(timer_1 , asio::chrono::seconds(5));

    std::cout << "Starting wait...";
    timer.wait();
    std::cout << "\rWaiting Finished!.. Hello world!!";

    return EXIT_SUCCESS;
}
