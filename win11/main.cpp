#include <iostream>
#include <windows.h>
#include <pdh.h>

// Link the necessary PDH library
#pragma comment(lib, "pdh.lib")

float getCPUUsage() {
    HQUERY hQuery = NULL;
    HCOUNTER hCounter = NULL;
    PDH_STATUS status;

    // Zero initialize the counter value structure
    PDH_FMT_COUNTERVALUE counterValue = {0};

    // Create a query object
    status = PdhOpenQuery(NULL, 0, &hQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to open PDH Query, Error: " << status << std::endl;
        return -1.0f;
    }

    // Add a counter to the query object
    status = PdhAddCounter(hQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &hCounter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to add PDH Counter, Error: " << status << std::endl;
        PdhCloseQuery(hQuery);
        return -1.0f;
    }

    // Initial data collection
    status = PdhCollectQueryData(hQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect PDH Data (initial), Error: " << status << std::endl;
        PdhCloseQuery(hQuery);
        return -1.0f;
    }

    Sleep(1000); // Sleep for a second to get a time sample

    // Collect the second sample
    status = PdhCollectQueryData(hQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect PDH Data (second), Error: " << status << std::endl;
        PdhCloseQuery(hQuery);
        return -1.0f;
    }

    // Get the calculated counter value
    status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterValue);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to get formatted counter value, Error: " << status << std::endl;
        PdhCloseQuery(hQuery);
        return -1.0f;
    }

    PdhCloseQuery(hQuery);
    return static_cast<float>(counterValue.doubleValue);
}

int main() {
    float cpuUsage = getCPUUsage();
    if (cpuUsage >= 0.0f) {
        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    } else {
        std::cerr << "Failed to get CPU usage." << std::endl;
    }
    return 0;
}
