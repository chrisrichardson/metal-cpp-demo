//
//  main.cpp
//  try
//
//  Created by Moritz Hof on 30.12.21.
//

#include <iostream>

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include "metal_adder.hpp"

auto main(int argc, const char *argv[]) -> int {
  // insert code here...
  NS::AutoreleasePool *p_pool = NS::AutoreleasePool::alloc()->init();
  MTL::Device *device = MTL::CreateSystemDefaultDevice();

  metal_adder *adder = new metal_adder();
  adder->init_with_device(device);
  adder->prepare_data();
  adder->send_compute_command();

  std::cout << " End of Computation  " << std::endl;
  p_pool->release();
  return 0;
}
