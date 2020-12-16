#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "base.cpp"
#include "Timer.cpp"

#define CL_TARGET_OPENCL_VERSION 220

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.cpp>
#else

#include <CL/cl.hpp>

#endif


template<int height, int width>
struct openCL_version {
    size_t iterations;
    u_char map[height][width];
    
    std::string sourceCode;

    openCL_version(cell (&m)[height][width], int iterations) : iterations(iterations) {
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                map[i][j] = (u_char) m[i][j];
            }
        }
        
        std::ifstream sourceFile("kernel.cl");
        sourceCode = std::string(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
    }

    // https://sites.google.com/site/csc8820/opencl-basics/a-simple-opencl-example
    // https://www.coursera.org/lecture/opencl-fpga-introduction/ndrange-and-single-work-item-kernels-1J01b
    void operator()() {

        const int N_ELEMENTS = height * width;
        u_int platform_id = 0, device_id = 0;

        try {
	    std::vector<cl::Platform> platforms;
            cl::Platform::get(&platforms);

            std::vector<cl::Device> devices;
            platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU, &devices);

            cl::Context context(devices);
            cl::CommandQueue queue = cl::CommandQueue(context, devices[device_id]);

            constexpr size_t data_size = N_ELEMENTS * sizeof(u_char);

            cl::Buffer current_gen = cl::Buffer(context, CL_MEM_READ_ONLY, data_size);
            cl::Buffer next_gen = cl::Buffer(context, CL_MEM_WRITE_ONLY, data_size);

            queue.enqueueWriteBuffer(current_gen, CL_FALSE, 0, data_size, map);

            u_char empty[height][width] = {{}};
            queue.enqueueWriteBuffer(next_gen, CL_FALSE, 0, data_size, empty);

            //std::cout << sourceCode << std::endl;
            cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

            cl::Program program = cl::Program(context, source);
            program.build(devices);

            cl::Kernel kernel(program, "octagonal");

            kernel.setArg(0, current_gen);
            kernel.setArg(1, height);
            kernel.setArg(2, width);
            kernel.setArg(3, next_gen);

            cl::NDRange global(height, width);
            cl::NDRange local(16, 16);

            for (size_t i = 0; i < iterations; i++) {
                queue.enqueueNDRangeKernel(kernel, 0, global, local);
                queue.enqueueCopyBuffer(next_gen, current_gen, 0, 0, data_size);
            }

            queue.enqueueReadBuffer(current_gen, CL_TRUE, 0, data_size, map);

        }
        catch (const cl::Error& err) {
            std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
        }
    }

    void print() {
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                // if ((int) map[i][j] == alive) std::cout << '.' << ' ';
                // if ((int) map[i][j] == dead) std::cout << 'X' << ' ';
                std::cout << (int) map[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

};
