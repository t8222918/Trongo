CC=g++
CDEFINES=
SOURCES=Dispatcher.cpp Mode.cpp precomp.cpp profanity.cpp SpeedSample.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=profanity.x64

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-framework OpenCL -lcurl
	CFLAGS=-c -std=c++11 -Wall -mmmx -O2
else
	LDFLAGS=-s -lOpenCL -mcmodel=large
	CFLAGS=-c -std=c++11 -Wall -mmmx -O2 -mcmodel=large 
endif

# CUDA support
NVCC = nvcc
CUDAFLAGS = -arch=sm_60 -O2

# 添加CUDA相关头文件和源文件
CUDA_OBJS = \
	cuda_utils.o \
	
# 例如后续可添加 .cu 文件

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CDEFINES) $< -o $@

clean:
	rm -rf *.o

profanity: profanity.o Dispatcher.o CLMemory.o Mode.o ArgParser.o help.o kernel_profanity.o kernel_sha256.o kernel_keccak.o CUDAMemory.o $(CUDA_OBJS)
	$(CXX) $^ -o $@ -lcurl -lpthread -ldl -lstdc++ -L/usr/local/cuda/lib64 -lcudart

cuda_utils.o: cuda_utils.hpp
	$(NVCC) $(CUDAFLAGS) -c cuda_utils.hpp -o cuda_utils.o

CUDAMemory.o: CUDAMemory.hpp
	$(CXX) -c CUDAMemory.hpp -o CUDAMemory.o

