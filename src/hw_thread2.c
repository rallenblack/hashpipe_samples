#include <pthread.h>
#include <string.h>

#include "hashpipe.h"
#include "hw_databuf.h"

/*****************************************************
 * hw_thread2.c
 * Author: Richard Black
 * Date: January 12, 2017
 *
 * This threads waits for data to become available in
 * the hw_buffer1_databuf_t buffer. It then prints
 * out the contents of the buffer, which should be
 * three characters of the string "HELLO WORLD!"
 *
 * Expected output:
 * -----------------
 *  0: HEL
 *  1: LO 
 *  2: WOR
 *  3: LD!
 *  4: HEL
 *  5: LO
 *  6: WOR
 *  7: LD!
 *  ...
 *  19: LD!
 * -----------------
 *****************************************************/

// Run method for the thread
static void * run(hashpipe_thread_args_t * args) {

    // Create a local pointer to the input buffer
    hw_buffer1_databuf_t * db_in = (hw_buffer1_databuf_t *)args->ibuf;

    // Get access to status shared memory key/values
    hashpipe_status_t st = args->st;

    // Create placeholder for incoming data
    char new_chars[4];

    // Create iteration count placeholder
    int iter_count = -1;

    // Create block index tracker
    int block_idx = 0;

    // Execute the main loop of the thread
    int rv;
    while (run_threads()) {
        // Wait for the buffer block to be available for writing
        while ((rv=hw_buffer1_databuf_wait_filled(db_in, block_idx)) != HASHPIPE_OK) {
            // If we time out, print "waiting" to status keyword "THREAD1"
            hashpipe_status_lock_safe(&st); // Gives us exclusive access to shared memory
            hputs(st.buf, "THREAD2", "waiting"); // Put string "waiting" with key "THREAD1"
            hashpipe_status_unlock_safe(&st); // Releases our exclusive access

            pthread_testcancel(); // Check if process is ending
        }

        // Once we get here, we have a ready-for-processing block in the buffer

        // Copy buffer contents to local memory
        strncpy(new_chars, db_in->block[block_idx].data, 3);
        new_chars[3] = '\0';

        // Get the iteration count
        iter_count = db_in->block[block_idx].header.block_number;

        // Print the string to the console
        printf("%d: %s\n", iter_count, new_chars);

        // Mark block as free so it can get new data
        hw_buffer1_databuf_set_free(db_in, block_idx);

        // Update the block index
        block_idx = (block_idx + 1) % N_BLOCKS1;

        // Check to see if hashpipe is closing
        pthread_testcancel();
    }

    return NULL;
}


// Thread description for hashpipe
static hashpipe_thread_desc_t hw_thread2 = {
    name: "hw_thread2",
    skey: "STAT2",
    init: NULL,
    run: run,
    ibuf_desc: {hw_buffer1_databuf_create},
    obuf_desc: {NULL}
};

static __attribute__((constructor)) void ctor() {
    register_hashpipe_thread(&hw_thread2);
}

