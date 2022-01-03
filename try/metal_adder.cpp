//
//  metal_adder.cpp
//  try
//
//  Created by Moritz Hof on 03.01.22.
//

#include <iostream>
#include "metal_adder.hpp"

const unsigned int vector_length = 1 << 10;
auto buffer_size = vector_length*sizeof(float);

void metal_adder::random_number_generator(MTL::Buffer *buffer){
    float* data_ptr = (float*)buffer->contents();
        for (unsigned long index = 0; index < vector_length; index++){
            data_ptr[index] = (float)rand() / (float)(RAND_MAX);
        }
}

void metal_adder::encode_add_command(MTL::ComputeCommandEncoder* compute_encoder){
    compute_encoder->setComputePipelineState(_addFunctionPSO);
    
        compute_encoder->setBuffer(_A, 0, 0);
        compute_encoder->setBuffer(_B, 0, 1);
        compute_encoder->setBuffer(_C, 0, 2);
        
        MTL::Size grid_size = MTL::Size(vector_length, 1, 1);
        
        NS::UInteger _thread_group_size = _addFunctionPSO->maxTotalThreadsPerThreadgroup();
        if(_thread_group_size > vector_length){
            _thread_group_size = vector_length;
        }
        
        MTL::Size thread_group_size = MTL::Size(_thread_group_size, 1, 1);
        
        compute_encoder->dispatchThreads(grid_size, thread_group_size);
}

void metal_adder::init_with_device(MTL::Device *device){
    
    NS::Error* error;
    this->_device = device;
    
    auto lib = _device->newDefaultLibrary();
    if(!lib){
        std::cerr << "Failed to load Default Library\n";
        std::exit(-1);
    }
    
    auto function =NS::String::string("add_vector", NS::ASCIIStringEncoding);
    auto mtl_function = lib->newFunction(function);
    if(!mtl_function){
        std::cerr<<"failed to load kernel\n";
        std::exit(-1);
    }
    
    _addFunctionPSO = _device->newComputePipelineState(mtl_function, &error);
    _CommandQueue   = _device->newCommandQueue();
    
}


void metal_adder::prepare_data(){
    
    _A = _device->newBuffer(buffer_size, MTL::ResourceStorageModeShared);
    _B = _device->newBuffer(buffer_size, MTL::ResourceStorageModeShared);
    _C = _device->newBuffer(buffer_size, MTL::ResourceStorageModeShared);
    
    random_number_generator(_A);
    random_number_generator(_B);
}

void metal_adder::send_compute_command(){
    MTL::CommandBuffer* command_buffer = _CommandQueue->commandBuffer();
    MTL::ComputeCommandEncoder* compute_encoder = command_buffer->computeCommandEncoder();
    
    encode_add_command(compute_encoder);
    compute_encoder->endEncoding();
    command_buffer->commit();
    command_buffer->waitUntilCompleted();
    
    verify();
}

void metal_adder::verify(){
    auto a = (float*)_A->contents();
    auto b = (float*)_B->contents();
    auto c = (float*)_C->contents();
    
    for(unsigned long i = 0; i < vector_length; ++i){
        if( c[i] != (a[i] + b[i]) ){
             std::cout << "\033[1;31m TEST FAILED \033[0m\n" ;}
    }
            std::cout << "\033[1;32m TEST PASSED \033[0m\n" ;
        
}



