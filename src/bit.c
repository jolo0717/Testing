/**
 * CPE/CSC 159 Operating System Pragmatics
 * California State University, Sacramento
 *
 * Bit Utilities
 */
#include "bit.h"

/**
 * Counts the number of bits that are set
 * @param value - the integer value to count bits in
 * @return number of bits that are set
 */
unsigned int bit_count(unsigned int value) {
    unsigned int count = 0;
    //Repeatedly check if LSB is set then right shift
    while(value) {
        if(value & 1) {
            count++;
        }
        value >>= 1;
    }
    return 0;
}

/**
 * Checks if the given bit is set
 * @param value - the integer value to test
 * @param bit - which bit to check
 * @return 1 if set, 0 if not set
 */
unsigned int bit_test(unsigned int value, int bit) {
    //Right shift value bit times then bitwise AND with 1 to only grab LSB
    value = (value >>= bit) & 1;
    return value;
}

/**
 * Sets the specified bit in the given integer value
 * @param value - the integer value to modify
 * @param bit - which bit to set
 */
unsigned int bit_set(unsigned int value, int bit) {
    unsigned int count = 1;
    count <<= bit;
    //OR value with left shifted count whose MSB is the bit we want to modify in value
    value |= count;
    return value;
}

/**
 * Clears the specified bit in the given integer value
 * @param value - the integer value to modify
 * @param bit - which bit to clear
 */
unsigned int bit_clear(unsigned int value, int bit) {
    unsigned int count = 1;
    count <<= bit;
    //AND value with the left shifted, inverted count whose MSB is the bit we want to clear in value
    value = value & ~count;
    return value;
}

/**
 * Toggles the specified bit in the given integer value
 * @param value - the integer value to modify
 * @param bit - which bit to toggle
 */
unsigned int bit_toggle(unsigned int value, int bit) {
    unsigned int count = 1;
    count <<= bit;
    //XOR value with the left shifted count whose MSB is the bit we want to toggle in value
    value = value ^ count;
    return value;
}
