#ifndef _HW_DATABUF_H
#define _HW_DATABUF_H

#include "hashpipe_databuf.h"

/*  Hello World for hashpipe!
 *  Author: Richard Black
 *  Date: Jan. 11, 2017
 *  
 *  Processing structure
 *  ======================================
 *  hw_thread1 -> hw_buffer1 -> hw_thread2
 *  ======================================
 *
 *  thread1 will put the letters "HELLO WORLD!" into buffer1.
 *  thread2 will read buffer1 and print out contents
 *
 *  This header file explains how the buffer is to be structured.
 */


// Macros to maintain cache alignment
#define CACHE_ALIGNMENT (128)
typedef uint8_t hashpipe_databuf_cache_alignment[
    CACHE_ALIGNMENT - (sizeof(hashpipe_databuf_t)%CACHE_ALIGNMENT)
];

// Number of blocks in the first buffer
#define N_BLOCKS1 5

// Create block header struct
typedef struct hw_buffer1_header {
    int block_number; // We'll keep track of how many blocks we've processed
} hw_buffer1_header_t;

// Create dummy structure to make header size a multiple of CACHE_ALIGNMENT
typedef uint8_t hw_buffer1_cache_alignment[
    CACHE_ALIGNMENT - (sizeof(hw_buffer1_header_t)%CACHE_ALIGNMENT)
];

// Create the actual block struct
typedef struct hw_buffer1_block {
    hw_buffer1_header_t header; // Put the header in the block
    hw_buffer1_cache_alignment padding; // Force data to be aligned
    char data[4]; // Three characters per block
} hw_buffer1_block_t;

// Create the full buffer
typedef struct hw_buffer1_databuf {
    hashpipe_databuf_t header; // All hashpipe buffers must have this header
    hashpipe_databuf_cache_alignment padding; // Force buffer to be aligned
    hw_buffer1_block_t block[N_BLOCKS1];
} hw_buffer1_databuf_t;

/**********************************************
 *  Create buffer control method prototypes
 *  (Definitions in hw_databuf.c)
 **********************************************/

/*
 *  hw_buffer1_databuf_create
 *  Creates an hw_buffer1_databuf_t hashpipe-compatible buffer
 *
 *  @arg    int instance_id
 *          The hashpipe instance id (set by -I on the command line)
 *  @arg    int databuf_id
 *          The buffer's id -- typically set by hashpipe during startup
 *  @return hashpipe_databuf_t *
 *          A pointer to the newly created hashpipe-compatible buffer
 */
hashpipe_databuf_t * hw_buffer1_databuf_create(int instance_id, int databuf_id);


/*
 *  hw_buffer1_databuf_wait_free
 *  Blocking function to wait for a block in the buffer to be marked as processed
 *  @arg hw_buffer1_databuf_t * d
 *       A pointer to the buffer
 *  @arg int block_id
 *       The block index to wait for
 */
int hw_buffer1_databuf_wait_free(hw_buffer1_databuf_t * d, int block_id);

/*
 *  hw_buffer1_databuf_wait_filled
 *  Blocking function to wait for block to be marked as ready for processing
 *  @arg hw_buffer1_databuf_t * d
 *       A pointer to the buffer
 *  @arg int block_id
 *       The block index to wait for
 */
int hw_buffer1_databuf_wait_filled(hw_buffer1_databuf_t * d, int block_id);

/*
 *  hw_buffer1_databuf_set_free
 *  Function to mark block as having been processed
 *  @arg hw_buffer1_databuf_t * d
 *       A pointer to the buffer
 *  @arg int block_id
 *       The block index to free
 */
int hw_buffer1_databuf_set_free(hw_buffer1_databuf_t * d, int block_id);

/*
 *  hw_buffer1_databuf_set_filled
 *  Function to mark block as ready for processing
 *  @arg hw_buffer1_databuf_t * d
 *       A pointer to the buffer
 *  @arg int block_id
 *       The block index to mark as filled
 */
int hw_buffer1_databuf_set_filled(hw_buffer1_databuf_t * d, int block_id);

#endif
