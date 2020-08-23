#ifndef _PAGERANK_CUH
#define _PAGERANK_CUH

#include "util.cuh"


#define checkCudaErrors(val) check( (val), #val, __FILE__, __LINE__)
template<typename T>
void check(T err, const char* const func, const char* const file, const int line) {
  if (err != cudaSuccess) {
    std::cerr << "CUDA error at: " << file << ":" << line << std::endl;
    std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
    exit(1);
  }
}

static const uint MAX_GRID_DIM = 65535;
/* 
 * Each kernel handles the update of one pagerank score. In other
 * words, each kernel handles one row of the update:
 *
 *      pi(t+1) = (1 / 2) A pi(t) + (1 / (2N))
 *
 * You may assume that num_nodes <= blockDim.x * 65535
 *
 */
__global__ void device_graph_propagate(
    const uint *graph_indices,
    const uint *graph_edges,
    const float *graph_nodes_in,
    float *graph_nodes_out,
    const float *inv_edges_per_node,
    int num_nodes
) {
    // TODO: fill in the kernel code here
    uint bid = blockIdx.y * gridDim.x + blockIdx.x;
    uint tid = bid*blockDim.x + threadIdx.x;
    if (tid < num_nodes){
       float sum = 0.f;
       for(uint j = graph_indices[tid]; j < graph_indices[tid+1]; ++j){
          sum += graph_nodes_in[graph_edges[j]]*inv_edges_per_node[graph_edges[j]]; 
       }
       graph_nodes_out[tid] = 0.5f/(float)num_nodes + 0.5f*sum; 
    }
}

void swapIO(float **in, float **out){
     float *temp;
     temp = *in;
     *in = *out;
     *out = temp;
}

/* 
 * This function executes a specified number of iterations of the
 * pagerank algorithm. The variables are:
 *
 * h_graph_indices, h_graph_edges:
 *     These arrays describe the indices of the neighbors of node i.
 *     Specifically, node i is adjacent to all nodes in the range
 *     h_graph_edges[h_graph_indices[i] ... h_graph_indices[i+1]].
 *
 * h_node_values_input:
 *     An initial guess of pi(0).
 *
 * h_gpu_node_values_output:
 *     Output array for the pagerank vector.
 *
 * h_inv_edges_per_node:
 *     The i'th element in this array is the reciprocal of the
 *     out degree of the i'th node.
 *
 * nr_iterations:
 *     The number of iterations to run the pagerank algorithm for.
 *
 * num_nodes:
 *     The number of nodes in the whole graph (ie N).
 *
 * avg_edges:
 *     The average number of edges in the graph. You are guaranteed
 *     that the whole graph has num_nodes * avg_edges edges.
 */

double device_graph_iterate(
    const uint *h_graph_indices,
    const uint *h_graph_edges,
    const float *h_node_values_input,
    float  *h_gpu_node_values_output,
    const float *h_inv_edges_per_node,
    int nr_iterations,
    int num_nodes,
    int avg_edges
) {
    // TODO: allocate GPU memory
    const uint num_indices = num_nodes + 1;
    const uint num_edges = num_nodes * avg_edges;
    
    uint *d_graph_indices, *d_graph_edges;
    float *d_input, *d_output, *d_inv_edges_per_node;
    
    cudaMalloc((void **)&d_graph_indices, num_indices*sizeof(uint));
    cudaMalloc((void **)&d_graph_edges, num_edges*sizeof(uint));
    cudaMalloc((void **)&d_input, num_nodes*sizeof(uint));
    cudaMalloc((void **)&d_output, num_nodes*sizeof(uint));
    cudaMalloc((void **)&d_inv_edges_per_node, num_nodes*sizeof(uint));

    // TODO: check for allocation failure
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());
    // TODO: copy data to the GPU
    cudaMemcpy(d_graph_indices, h_graph_indices, num_indices*sizeof(uint), cudaMemcpyHostToDevice);
    cudaMemcpy(d_graph_edges, h_graph_edges,  num_edges*sizeof(uint), cudaMemcpyHostToDevice);
    cudaMemcpy(d_input, h_node_values_input, num_nodes*sizeof(uint), cudaMemcpyHostToDevice);
    cudaMemcpy(d_inv_edges_per_node, h_inv_edges_per_node, num_nodes*sizeof(uint), cudaMemcpyHostToDevice);

    event_pair timer;
    start_timer(&timer);

    const int block_size = 192;
    // compute grid dimensions
    dim3 blocks_per_grid(1,1);
    dim3 threads_per_blocks(block_size);
    // compute number of blocks needed
    uint num_blocks = ceil((float)num_nodes / (float)block_size);
    if (num_blocks > MAX_GRID_DIM){
       blocks_per_grid.x = MAX_GRID_DIM;
       blocks_per_grid.y = ceil((float)num_blocks/(float)MAX_GRID_DIM);
    }
    else{
       blocks_per_grid.x = num_blocks;
    }
    // TODO: launch your kernels the appropriate number of iterations
    
    check_launch("gpu graph propagate");
    double gpu_elapsed_time = stop_timer(&timer);

    // TODO: copy final data back to the host for correctness checking
    for(int i=0;i<nr_iterations;++i){
       device_graph_propagate<<<blocks_per_grid, threads_per_blocks>>>(d_graph_indices,d_graph_edges,d_input,d_output,d_inv_edges_per_node,num_nodes);
       swapIO(&d_input,&d_output);
    }
    // copy device to host
    cudaMemcpy(h_gpu_node_values_output, d_input, num_nodes*sizeof(float),cudaMemcpyDeviceToHost);
    // TODO: free the memory you allocated!
    cudaFree(d_graph_indices);
    cudaFree(d_graph_edges);
    cudaFree(d_input);
    cudaFree(d_output);
    cudaFree(d_inv_edges_per_node);
    //error checking
    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());
    return gpu_elapsed_time;
}

/**
 * This function computes the number of bytes read from and written to
 * global memory by the pagerank algorithm.
 * 
 * nodes:
 *      The number of nodes in the graph
 *
 * edges: 
 *      The average number of edges in the graph
 *
 * iterations:
 *      The number of iterations the pagerank algorithm was run
 */
uint get_total_bytes(uint nodes, uint edges, uint iterations)
{
    // TODO
    size_t index_read = 2*(nodes)*sizeof(uint);
    size_t graph_edge_read = (nodes)*(edges)*sizeof(uint);
    size_t graph_node_read = (nodes)*(edges)*sizeof(float);
    size_t inv_edges_read = (nodes)*(edges)*sizeof(float);
    size_t read_bytes = index_read+graph_edge_read+graph_node_read+inv_edges_read;
    return read_bytes;
}

#endif
