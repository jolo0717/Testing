/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 *
 * Kernel functions
 */
#include <spede/flames.h>   // for breakpoint()
#include <spede/stdarg.h>   // for variable argument functions (va_*)
#include <spede/stdio.h>    // for printf
#include <spede/string.h>   // string handling

#include "kernel.h"
#include "vga.h"
#include "keyboard.h"

#ifndef KERNEL_LOG_LEVEL_DEFAULT
#define KERNEL_LOG_LEVEL_DEFAULT KERNEL_LOG_LEVEL_TRACE
#endif

// Current log level
int kernel_log_level = KERNEL_LOG_LEVEL_DEFAULT;

/**
 * Initializes any kernel internal data structures and variables
 */
void kernel_init(void) {
    // Display a welcome message on the host
    kernel_log_info("Welcome to %s!", OS_NAME);

    kernel_log_info("Initializing kernel...");
}

/**
 * Prints a kernel log message to the host with an error log level
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_log_error(char *msg, ...) {
    // Return if our log level is less than error
    if (kernel_log_level < KERNEL_LOG_LEVEL_ERROR) {
        return;
    }

    // Obtain the list of variable arguments
    va_list args;

    // Indicate this is an 'error' type of message
    printf("error: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

/**
 * Prints a kernel log message to the host with a warning log level
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_log_warn(char *msg, ...) {
    // Return if our log level is less than warn
    if (kernel_log_level < KERNEL_LOG_LEVEL_WARN) {
        return;
    }

    // Obtain the list of variable arguments
    va_list args;

    // Indicate this is a 'warn' type of message
    printf("warn: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

/**
 * Prints a kernel log message to the host with an info log level
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_log_info(char *msg, ...) {
    // Return if our log level is less than info
    if (kernel_log_level < KERNEL_LOG_LEVEL_INFO) {
        return;
    }

    // Obtain the list of variable arguments
    va_list args;

    // Indicate this is an 'info' type of message
    printf("info: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

/**
 * Prints a kernel log message to the host with a debug log level
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_log_debug(char *msg, ...) {
    // Return if our log level is less than debug
    if (kernel_log_level < KERNEL_LOG_LEVEL_DEBUG) {
        return;
    }

    // Obtain the list of variable arguments
    va_list args;

    // Indicate this is a 'debug' type of message
    printf("debug: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

/**
 * Prints a kernel log message to the host with a trace log level
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_log_trace(char *msg, ...) {
    // Return if our log level is less than trace
    if (kernel_log_level < KERNEL_LOG_LEVEL_TRACE) {
        return;
    }

    // Obtain the list of variable arguments
    va_list args;

    // Indicate this is a 'trace' type of message
    printf("trace: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");
}

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message on the host console
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits the operating system program
 *
 * @param msg - string format for the message to be displayed
 * @param ... - variable arguments to pass in to the string format
 */
void kernel_panic(char *msg, ...) {
    // Obtain the list of variable arguments
    va_list args;

    //Message to be sent over VGA
    char vgaString[100];

    // Indicate this is a 'panic' type of message
    printf("PANIC: ");

    // Pass the message variable arguments to vprintf
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\n");

    //Pass the message variable arguments to vsnprintf
    va_start(args, msg);
    vsnprintf(vgaString, sizeof(vgaString), msg, args);
    va_end(args);

    //Write to VGA
    vga_puts(vgaString);
    vga_puts("\n");

    // Trigger a breakpoint to inspect what caused the panic
    kernel_break();

    // Exit since this is fatal
    exit(1);
}

/**
 * Returns the current log level
 * @return the kernel log level
 */
int kernel_get_log_level(void) {
    return kernel_log_level;
}

/**
 * Sets the new log level and returns the value set
 * @param level - the log level to set
 * @return the kernel log level
 */
int kernel_set_log_level(log_level_t level) {
    int prev_log_level = kernel_log_level;

    //Changes kernel_log_level to declared level
    switch(level) {
        case 0:
            kernel_log_level = KERNEL_LOG_LEVEL_NONE;
            break;
        case 1:
            kernel_log_level = KERNEL_LOG_LEVEL_ERROR;
            break;
        case 2:
            kernel_log_level = KERNEL_LOG_LEVEL_WARN;
            break;
        case 3:
            kernel_log_level = KERNEL_LOG_LEVEL_INFO;
            break;
        case 4:
            kernel_log_level = KERNEL_LOG_LEVEL_DEBUG;
            break;
        case 5:
            kernel_log_level = KERNEL_LOG_LEVEL_TRACE;
            break;
        case 6:
            kernel_log_level = KERNEL_LOG_LEVEL_ALL;
            break;
        default:
            kernel_log_level = KERNEL_LOG_LEVEL_INFO;
    }

    //For any invalid levels
    if(level < KERNEL_LOG_LEVEL_NONE) {
        kernel_log_level = KERNEL_LOG_LEVEL_NONE;
    }else if(level > KERNEL_LOG_LEVEL_ALL) {
        kernel_log_level = KERNEL_LOG_LEVEL_ALL;
    }

    if (prev_log_level != kernel_log_level) {
        printf("<<kernel log level set to %d>>", kernel_log_level);
    }

    return kernel_log_level;
}

/**
 * Triggers a breakpoint (if running under GBD)
 */
void kernel_break(void) {
    breakpoint();
}

/**
 * Triggers a kernel command
 */
void kernel_command(char c) {
    static int kernel_escape = 0;

    switch (c) {
        case 'p':
        case 'P':
            // Test the kernel panic
            kernel_panic("test panic");
            break;

        case 'b':
        case 'B':
            // Test a breakpoint (only valid when running with GDB)
            kernel_break();
            break;

        // Add new commands to:

        // -Toggle VGA text mode cursor
        case 'c':
        case 'C':
            switch(vga_cursor_enabled()) {
                case 0:
                    vga_cursor_enable();
                    break;
                case 1:
                    vga_cursor_disable();
                    break;
                default:
            }
            break;
            
        //  - Clear the screen (k)
        case 'k':
        case 'K':
            vga_clear();
            break;
        
        //  - Increase the kernel log level (+)
        case 'i':
        case 'I':
            kernel_set_log_level(kernel_log_level++);
            break;
        
        //  - Decrease the kernel log level (-)
        case 'd':
        case 'D':
            kernel_set_log_level(kernel_log_level--);
            break;

        case KEY_ESCAPE:
            // Exit the OS if we press escape three times in a row
            kernel_escape++;

            if (kernel_escape >= 3) {
                kernel_exit();
            }
            break;

        case KEY_NULL:
        default:
            kernel_escape = 0;
            // Nothing to do
            break;
    }
}

/**
 * Exits the kernel
 */
void kernel_exit(void) {
    // Print to the terminal
    printf("Exiting %s...\n", OS_NAME);

    // Print to the VGA display
    char *vgaString = "Exiting " OS_NAME "...\n";
    vga_puts(vgaString);
    // Exit
    exit(0);
}