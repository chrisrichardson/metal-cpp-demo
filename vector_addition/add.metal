//
//  add.metal
//  try
//
//  Created by Moritz Hof on 30.12.21.
//

#include <metal_stdlib>
using namespace metal;

kernel void add_vector(device const float* a, device const float* b, device float* c, uint index [[thread_position_in_grid]]){
    
    c[index] = a[index]+b[index];
}
