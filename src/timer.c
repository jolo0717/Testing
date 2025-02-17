/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 *
 * Timer Implementation
 */
#include <spede/string.h>

#include "interrupts.h"
#include "kernel.h"
#include "queue.h"
#include "timer.h"

/**
 * Data structures
 */
// Timer data structure
typedef struct timer_t {
    void (*callback)(); // Function to call when the interval occurs
    int interval;       // Interval in which the timer will be called
    int repeat;         // Indicate how many intervals to repeat (-1 should repeat forever)
} timer_t;

/**
 * Variables
 */

// Number of timer ticks that have occured
int timer_ticks;

// Timers table; each item in the array is a timer_t struct
timer_t timers[TIMERS_MAX];

// Timer allocator; used to allocate indexes into the timers table
queue_t timer_allocator;


/**
 * Registers a new callback to be called at the specified interval
 * @param func_ptr - function pointer to be called
 * @param interval - number of ticks before the callback is performed
 * @param repeat   - Indicate how many intervals to repeat (-1 should repeat forever)
 *
 * @return the allocated timer id or -1 for errors
 */
int timer_callback_register(void (*func_ptr)(), int interval, int repeat) {
    int timer_id = -1;

    if (!func_ptr) {
        kernel_log_error("timer: invalid function pointer");
        return -1;
    }

    // Obtain a timer id
    if (queue_out(&timer_allocator, &timer_id) != 0) {
        kernel_log_error("timer: unable to allocate a timer");
        return -1;
    }

    // Set the callback function for the timer
    // Set the interval value for the timer
    // Set the repeat value for the timer

    return timer_id;
}

/**
 * Unregisters the specified callback
 * @param id
 *
 * @return 0 on success, -1 on error
 */
int timer_callback_unregister(int id) {
    timer_t *timer;

    if (id < 0 || id >= TIMERS_MAX) {
        kernel_log_error("timer: callback id out of range: %d", id);
        return -1;
    }

    timer = &timers[id];
    memset(timer, 0, sizeof(timer_t));

    if (queue_in(&timer_allocator, id) != 0) {
        kernel_log_error("timer: unable to queue timer entry back to allocator");
        return -1;
    }

    return 0;
}

/**
 * Returns the number of ticks that have occured since startup
 *
 * @return timer_ticks
 */
int timer_get_ticks() {
    return timer_ticks;
}

/**
 * Timer IRQ Handler
 *
 * Should perform the following:
 *   - Increment the timer ticks every time the timer occurs
 *   - Handle each registered timer
 *     - If the interval is hit, run the callback function
 *     - Handle timer repeats
 */
void timer_irq_handler(void) {
    // Increment the timer_ticks value
    timer_ticks++;

    // Iterate through the timers table
        // If we have a valid callback, check if it needs to be called
            // If the timer interval is hit, run the callback function

            // If the timer repeat is greater than 0, decrement
            // If the timer repeat is equal to 0, unregister the timer
            // If the timer repeat is less than 0, do nothing
}

/**
 * Initializes timer related data structures and variables
 */
void timer_init(void) {
    kernel_log_info("Initializing timer");

    // Set the starting tick value

    // Initialize the timers data structures

    // Initialize the timer callback allocator queue

    // Populate items into the allocator queue

    // Register the Timer IRQ with the isr_entry_timer and timer_irq_handler
}

