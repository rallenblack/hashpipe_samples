#include <pthread.h>
#include <string.h>

#include "hashpipe.h"
#include "hw_databuf.h"

/*****************************************************
 * hw_thread1.c
 * Author: Richard Black
 * Date: January 12, 2017
 *
 * This thread creates the string "HELLO WORLD!" and
 * passes that string 3 characters at a time to
 * hw_thread2 through the hw_buffer1_databuf_t buffer.
 *****************************************************/


// Run method for the thread
static void * run(hashpipe_thread_args_t * args) {

    // Create a local pointer to the output buffer
    hw_buffer1_databuf_t * db_out = (hw_buffer1_databuf_t *)args->obuf;

    // Get access to status shared memory key/values
    hashpipe_status_t st = args->st;

    // Create string to print in the other thread
    const char test_word[13] = "HELLO WORLD!\0";

    // Create string counter
    int str_idx = 0;

    // Create block index tracker
    int block_idx = 0;

    // Create iteration counter
    int iter_count = 0;

    // Execute the main loop of the thread
    int rv;
    while (run_threads()) {
        // Wait for the buffer block to be available for writing
        while ((rv=hw_buffer1_databuf_wait_free(db_out, block_idx)) != HASHPIPE_OK) {
            // If we time out, print "waiting" to status keyword "THREAD1"
            hashpipe_status_lock_safe(&st); // Gives us exclusive access to shared memory
            hputs(st.buf, "THREAD1", "waiting"); // Put string "waiting" with key "THREAD1"
            hashpipe_status_unlock_safe(&st); // Releases our exclusive access
        }

        // Once we get here, we have a ready-for-writing block in the buffer

        // Copy part of the string to the buffer
        strncpy(db_out->block[block_idx].data, test_word + (str_idx % 12), 3);

        // Update the string pointer
        str_idx += 3;

        // Write the loop counter
        db_out->block[block_idx].header.block_number = iter_count;

        // Only do this 20 times
        if (iter_count < 20) {
            // Update the loop counter
            iter_count++;

            // Mark block as filled so next thread can process the data
            hw_buffer1_databuf_set_filled(db_out, block_idx);
        
            // Update the block index
            // We use the % operator to make the block_idx circle back
            block_idx = (block_idx + 1) % N_BLOCKS1;
        }
        else {
            hashpipe_status_lock_safe(&st);
            hputs(st.buf, "THREAD1", "Done!");
            hashpipe_status_unlock_safe(&st);
        }

        // Check to see if hashpipe is closing
        pthread_testcancel();
    }

    return NULL;
}


// Thread description for hashpipe
static hashpipe_thread_desc_t hw_thread1 = {
    name: "hw_thread1", // The name of the thread for the command line
    skey: "STAT1", // A shared memory keyword for thread status msgs
    init: NULL, // The name of the initialization function (NULL if none)
    run: run, // The name of the main loop function
    ibuf_desc: {NULL}, // The buffer creation method name for the input buffer (NULL if none)
    obuf_desc: {hw_buffer1_databuf_create} // The buffer creation name for the output buffer (NULL if none)
};

// Hashpipe calls this method to enable this thread
static __attribute__((constructor)) void ctor() {
    register_hashpipe_thread(&hw_thread1);
}

