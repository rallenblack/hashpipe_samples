#include "hw_databuf.h"

hashpipe_databuf_t * hw_buffer1_databuf_create(int instance_id, int databuf_id) {
    size_t header_size = sizeof(hashpipe_databuf_t) + sizeof(hashpipe_databuf_cache_alignment);
    size_t block_size = sizeof(hw_buffer1_block_t);
    int n_block = N_BLOCKS1;
    return hashpipe_databuf_create(
        instance_id, databuf_id, header_size, block_size, n_block);
}

int hw_buffer1_databuf_wait_free(hw_buffer1_databuf_t * d, int block_id) {
    return hashpipe_databuf_wait_free((hashpipe_databuf_t *)d, block_id);
}

int hw_buffer1_databuf_wait_filled(hw_buffer1_databuf_t * d, int block_id) {
    return hashpipe_databuf_wait_filled((hashpipe_databuf_t *)d, block_id);
}

int hw_buffer1_databuf_set_free(hw_buffer1_databuf_t * d, int block_id) {
    return hashpipe_databuf_set_free((hashpipe_databuf_t *)d, block_id);
}

int hw_buffer1_databuf_set_filled(hw_buffer1_databuf_t * d, int block_id) {
    return hashpipe_databuf_set_filled((hashpipe_databuf_t *)d, block_id);
}
