#include <iostream>
#include <mach/mach_host.h>
#include <mach/mach.h>
#include <sys/sysctl.h>
#include <ctime>
#include <stdlib.h>

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

// Function to get memory usage
void printMemoryUsage() {
    vm_size_t page_size;
    vm_statistics_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    if (host_page_size(mach_host_self(), &page_size) == KERN_SUCCESS &&
        host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vm_stats, &count) == KERN_SUCCESS) {
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
        long long used_memory = ((int64_t)vm_stats.active_count + (int64_t)vm_stats.inactive_count + (int64_t)vm_stats.wire_count) * (int64_t)page_size;
        std::cout << "Free memory: " << free_memory / (1024 * 1024) << " MB\n";
        std::cout << "Used memory: " << used_memory / (1024 * 1024) << " MB\n";
    } else {
        std::cerr << "Failed to get memory usage." << std::endl;
    }
}

// Function to get system uptime
void printSystemUptime() {
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};

    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == -1) {
        std::cerr << "Failed to get boot time." << std::endl;
    } else {
        time_t bsec = boottime.tv_sec, csec = time(NULL);
        time_t uptime = difftime(csec, bsec);
        std::cout << "System Uptime: " << uptime / 60 << " minutes\n";
    }
}

// Function to get and display system load averages
void printLoadAverages() {
    double load[3];
    if (getloadavg(load, 3) != -1) {
        std::cout << "Load Averages: " << load[0] << " (1 min), "
                  << load[1] << " (5 min), " << load[2] << " (15 min)\n";
    } else {
        std::cerr << "Failed to get load averages." << std::endl;
    }
}

int main() {
    // Get and print CPU usage
    float cpuUsage = getCPUUsage();
    if (cpuUsage >= 0.0f) {
        std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    } else {
        std::cerr << "Failed to get CPU usage." << std::endl;
    }

    // Print memory usage
    printMemoryUsage();

    // Print system uptime
    printSystemUptime();

    // Print load averages
    printLoadAverages();

    return 0;
}
