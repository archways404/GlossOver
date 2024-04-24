#include <iostream>
#include <mach/mach_host.h>

// Function to get CPU usage percentage
float getCPUUsage() {
    host_cpu_load_info_data_t cpuInfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuInfo, &count) != KERN_SUCCESS) {
        return -1.0f;
    }

    float totalUsage = 0.0f;
    for (unsigned int i = 0; i < CPU_STATE_MAX; ++i) {
        totalUsage += cpuInfo.cpu_ticks[i];
    }

    // Calculate CPU usage percentage
    float usagePercentage = ((totalUsage - cpuInfo.cpu_ticks[CPU_STATE_IDLE]) / totalUsage) * 100.0f;

    return usagePercentage;
}

int main() {
    // Get and print CPU usage
    float cpuUsage = getCPUUsage();
    if (cpuUsage >= 0.0f) {
        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    } else {
        std::cerr << "Failed to get CPU usage." << std::endl;
    }

    return 0;
}
