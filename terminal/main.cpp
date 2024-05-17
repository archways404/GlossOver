#include <iostream>
#include <thread>
#include <chrono>
#include "WebsiteChecker.hpp"

void display_loading(int steps) {
    std::string loading = "[---------------]";
    for (int i = 1; i <= steps; ++i) {
        loading[i] = 'I';
    }
    std::cout << "\r" << loading << std::flush;
}

void check_and_print_status(const std::string& website, bool& previous_status) {
    bool current_status = WebsiteChecker::check_website(website);
    if (current_status != previous_status) {
        std::cout << website << (current_status ? " is up!" : " is down!") << std::endl;
        previous_status = current_status;
    }
}

void show_loading_for_seconds(int seconds) {
    const int steps = 15; // number of steps in the loading animation
    const int interval_ms = (seconds * 1000) / steps; // interval in milliseconds

    for (int i = 0; i <= steps; ++i) {
        display_loading(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
    std::cout << "\r[---------------]" << std::flush; // Clear loading animation after completion
}

int main() {
    const std::string website1 = "https://software404.org";
    const std::string website2 = "https://archways404.dev";
    const int interval = 5; // seconds

    bool status1 = false;
    bool status2 = false;

    // Initial status check
    check_and_print_status(website1, status1);
    check_and_print_status(website2, status2);

    while (true) {
        // Show loading animation for the sleep timer
        show_loading_for_seconds(interval);

        // Move cursor up to overwrite the loading animation
        std::cout << "\033[F\033[F";

        // Check websites and print results only if status changes
        check_and_print_status(website1, status1);
        check_and_print_status(website2, status2);

        // Additional sleep to ensure the full interval
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    return 0;
}
