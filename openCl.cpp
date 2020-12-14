#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "base.cpp"

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.cpp>
#else

#include <CL/cl.hpp>
#include <thread>

#endif

void loadKernel(std::string filename, char *&source, std::size_t &length) {
    std::streampos size;

    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        source = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(source, size);
        file.close();

    } else std::cerr << "Unable to open file" << std::endl;

}

cl_int err;

void die(cl_int err, const char *str) {
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error code %d: %s\n", err, str);
        exit(1);
    }
}

template<int height, int width>
struct openCL_version {
    int iterations;
    u_char map[height][width];

    openCL_version(cell (&m)[height][width], int iterations) : iterations(iterations) {
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                map[i][j] = (u_char) m[i][j];
            }
        }
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
            platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_CPU, &devices);

            cl::Context context(devices);
            cl::CommandQueue queue = cl::CommandQueue(context, devices[device_id]);

            cl::Buffer mapBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, N_ELEMENTS * sizeof(u_char));

            queue.enqueueWriteBuffer(mapBuffer, CL_FALSE, 0, N_ELEMENTS * sizeof(u_char), map);

            std::ifstream sourceFile("kernel.cl");
            std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));

            //std::cout << sourceCode << std::endl;
            cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

            cl::Program program = cl::Program(context, source);
            err = program.build(devices);
            die(err, "build");

            cl::Kernel kernel(program, "calculate");

            kernel.setArg(0, mapBuffer);
            kernel.setArg(1, height);
            kernel.setArg(2, width);

            cl::NDRange global(height, width);
            cl::NDRange local(16, 16);

            for (size_t i = 0; i < iterations; i++) {
                queue.enqueueNDRangeKernel(kernel, 0, global, local);
            }

            queue.enqueueReadBuffer(mapBuffer, CL_TRUE, 0, N_ELEMENTS * sizeof(u_char), map);

        }
        catch (cl::Error err) {
            std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
        }
    }

    void print() {
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                if ((int) map[i][j] == alive) std::cout << '.' << ' ';
                if ((int) map[i][j] == dead) std::cout << 'X' << ' ';
                //std::cout << (int) map[i][j] << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

};
