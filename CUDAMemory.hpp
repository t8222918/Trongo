#ifndef CUDA_MEMORY_HPP
#define CUDA_MEMORY_HPP

#include <cuda_runtime.h>
#include <stdexcept>
#include <cstring>

// CUDA内存管理模板
// 用法类似CLMemory

template<typename T>
class CUDAMemory {
public:
    CUDAMemory(size_t count, bool allocateHost = true)
        : m_count(count), m_size(sizeof(T) * count), m_pHostData(nullptr), m_pDeviceData(nullptr), m_allocateHost(allocateHost) {
        if (allocateHost) {
            m_pHostData = new T[count];
        }
        cudaError_t err = cudaMalloc(&m_pDeviceData, m_size);
        if (err != cudaSuccess) {
            throw std::runtime_error("cudaMalloc failed");
        }
    }
    ~CUDAMemory() {
        if (m_allocateHost && m_pHostData) delete[] m_pHostData;
        if (m_pDeviceData) cudaFree(m_pDeviceData);
    }
    void copyHostToDevice() {
        if (m_allocateHost && m_pHostData) {
            cudaMemcpy(m_pDeviceData, m_pHostData, m_size, cudaMemcpyHostToDevice);
        }
    }
    void copyDeviceToHost() {
        if (m_allocateHost && m_pHostData) {
            cudaMemcpy(m_pHostData, m_pDeviceData, m_size, cudaMemcpyDeviceToHost);
        }
    }
    T* hostData() { return m_pHostData; }
    T* deviceData() { return m_pDeviceData; }
    size_t size() const { return m_size; }
    size_t count() const { return m_count; }
private:
    size_t m_count;
    size_t m_size;
    T* m_pHostData;
    T* m_pDeviceData;
    bool m_allocateHost;
};

#endif // CUDA_MEMORY_HPP