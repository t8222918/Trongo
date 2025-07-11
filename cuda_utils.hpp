#ifndef CUDA_UTILS_HPP
#define CUDA_UTILS_HPP

#include <cuda_runtime.h>
#include <vector>
#include <string>
#include <iostream>

struct CUDADeviceInfo {
    int deviceId;
    std::string name;
    size_t totalGlobalMem;
    int multiProcessorCount;
    int major;
    int minor;
};

inline std::vector<CUDADeviceInfo> getAllCUDADevices() {
    int count = 0;
    cudaGetDeviceCount(&count);
    std::vector<CUDADeviceInfo> devices;
    for (int i = 0; i < count; ++i) {
        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);
        devices.push_back({
            i,
            prop.name,
            prop.totalGlobalMem,
            prop.multiProcessorCount,
            prop.major,
            prop.minor
        });
    }
    return devices;
}

inline void printCUDADevices() {
    auto devices = getAllCUDADevices();
    std::cout << "CUDA Devices:" << std::endl;
    for (const auto& d : devices) {
        std::cout << "  Device " << d.deviceId << ": " << d.name
                  << ", GlobalMem: " << d.totalGlobalMem / (1024*1024) << " MB"
                  << ", SMs: " << d.multiProcessorCount
                  << ", Compute Capability: " << d.major << "." << d.minor << std::endl;
    }
}

#endif // CUDA_UTILS_HPP