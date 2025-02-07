/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 *
 * Keyboard Functions
 */
#include "io.h"
#include "kernel.h"
#include "keyboard.h"

// Define constants for special keys
#define KEY_PRESS_BIT 0x80
#define KEY_CODE_MASK 0x7F

// Define the status of the special keys
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;
static int caps_lock_on = 0;
static int num_lock_on = 0;

/**
 * Initializes keyboard data structures and variables
 */
void keyboard_init() {
    kernel_log_info("Initializing keyboard driver");
}

/**
 * Scans for keyboard input and returns the raw character data
 * @return raw character data from the keyboard
 */
unsigned int keyboard_scan(void) {
    unsigned int c = inportb(0x60);
    return c;
}

/**
 * Polls for a keyboard character to be entered.
 *
 * If a keyboard character data is present, will scan and return
 * the decoded keyboard output.
 *
 * @return decoded character or KEY_NULL (0) for any character
 *         that cannot be decoded
 */
unsigned int keyboard_poll(void) {
    unsigned int c = keyboard_scan();

    if (c & KEY_PRESS_BIT) {                        // Check if the key is being pressed (bit 7 set)
        
        unsigned int key_code = c & KEY_CODE_MASK;  // Extract the key code (bits 0-6)
        return keyboard_decode(key_code);           // Decode the key code and return the corresponding character
    }

    return KEY_NULL;                                // Key is not being pressed, return KEY_NULL
}

/**
 * Blocks until a keyboard character has been entered
 * @return decoded character entered by the keyboard or KEY_NULL
 *         for any character that cannot be decoded
 */
unsigned int keyboard_getc(void) {
    unsigned int c = KEY_NULL;
    
    //Loop until valid key is pressed
    while (c == KEY_NULL) {
        c = keyboard_poll();                        //Check if key was pressed
    }
    return c;
}

/**
 * Processes raw keyboard input and decodes it.
 *
 * Should keep track of the keyboard status for the following keys:
 *   SHIFT, CTRL, ALT, CAPS, NUMLOCK
 *
 * For all other characters, they should be decoded/mapped to ASCII
 * or ASCII-friendly characters.
 *
 * For any character that cannot be mapped, KEY_NULL should be returned.
 *
 * If *all* of the status keys defined in KEY_KERNEL_DEBUG are pressed,
 * while another character is entered, the kernel_debug_command()
 * function should be called.
 */
unsigned int keyboard_decode(unsigned int c) {
    // Read the status from the keyboard controller
    char status = inportb(0x64);

    unsigned char keyboard_map_lowercase[] = {
        /* 0x00 - 0x0F */
        KEY_NULL, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
        /* 0x10 - 0x1F */
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'KEY_NULL', 'a', 's',
        /* 0x20 - 0x2F */
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 'KEY_NULL', '\\', 'z', 'x', 'c', 'v',
        /* 0x30 - 0x3F */
        'b', 'n', 'm', ',', '.', '/', 'KEY_NULL', '*', 'KEY_NULL', ' ', 'KEY_NULL', 'KEY_F1', 'KEY_F2', 'KEY_F3', 'KEY_F4', 'KEY_F5',
        /* 0x40 - 0x4F */
        'KEY_F6', 'KEY_F7', 'KEY_F8', 'KEY_F9', 'KEY_F10', 'KEY_NULL', 'KEY_NULL', '7', '8', '9', '-', '4', '5', '6', '+', '1',
        /* 0x50 - 0x58*/
        '2', '3', '0', '.', 'KEY_NULL', 'KEY_NULL', 'KEY_NULL', 'KEY_F11', 'KEY_F12'
    };

    unsigned char keyboard_map_uppercase[] = {
        /* 0x00 - 0x0F */
        KEY_NULL, KEY_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
        /* 0x10 - 0x1F */
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'KEY_NULL', 'A', 'S',
        /* 0x20 - 0x2F */
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 'KEY_NULL', '|', 'Z', 'X', 'C', 'V',
        /* 0x30 - 0x3F */
        'B', 'N', 'M', '<', '>', '?', 'KEY_NULL', '*', 'KEY_NULL', ' ', 'KEY_NULL', 'KEY_F1', 'KEY_F2', 'KEY_F3', 'KEY_F4', 'KEY_F5',
        /* 0x40 - 0x4F */
        'KEY_F6', 'KEY_F7', 'KEY_F8', 'KEY_F9', 'KEY_F10', 'KEY_NULL', 'KEY_NULL', 'KEY_HOME', 'KEY_UP', 'KEY_PAGE_UP', 'KEY_NULL', 'KEY_LEFT', 'KEY_NULL', 'KEY_RIGHT', 'KEY_NULL', 'KEY_END',
        /* 0x50 - 0x58*/
        'KEY_DOWN', 'KEY_PAGE_DOWN', 'KEY_INSERT', 'KEY_DELETE', 'KEY_NULL', 'KEY_NULL', 'KEY_NULL', 'KEY_F11', 'KEY_F12'
    };

    //while (status) {

        // Check if the key is a special key (SHIFT, CTRL, ALT)
        if (c == 0x2A || c == 0x36) {           //Left or Right SHIFT
            shift_pressed = 1;
        } 
    
        else if (c == 0x1D) {                   //CTRL
            ctrl_pressed = 1;
        }

        else if (c == 0x38) {                   //ALT
            alt_pressed = 1;
        } 

        else if (c == 0x3A) {                   //CAPS LOCK
            caps_lock_on = !caps_lock_on; 
        } 

        else if (c == 0x45) {                   //NUM LOCK
            num_lock_on = !num_lock_on; 
        } 

        else {
            unsigned char decoded_char = keyboard_map_lowercase[c];

            if (shift_pressed) {
                decoded_char = keyboard_map_uppercase[c];
            }

            if (caps_lock_on) {
                if (decoded_char == keyboard_map_uppercase[c]) {
                    decoded_char = keyboard_map_lowercase[c];
                }

                else if (decoded_char = keyboard_map_lowercase[c]) {
                    decoded_char = keyboard_map_uppercase[c];
                }
            }

            if (ctrl_pressed) {
        
            }
    
            if (alt_pressed) {

            }

            if (decoded_char == 0) {
                return KEY_NULL;
            }

            return decoded_char; 
        }
    //}
    
    return KEY_NULL;
}
