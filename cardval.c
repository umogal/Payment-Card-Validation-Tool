#include "cardval.h"
#include <stdint.h>
#include <stdbool.h>

/* Lookup table for Luhn values to avoid branching.
 * Index 0: Even position from right (unchanged).
 * Index 1: Odd position from right (doubled, sum of digits if > 9).
 */
static const uint8_t LUHN_TABLE[2][10] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {0, 2, 4, 6, 8, 1, 3, 5, 7, 9}
};

cardval_status_t cardval_validate_buf(const char* pan, size_t len) {
    if (!pan) {
        return CARDVAL_ERR_NULL_PTR;
    }
    
    if (len == 0) {
        return CARDVAL_ERR_INVALID_LENGTH;
    }

    uint32_t sum = 0;
    uint32_t digits_counted = 0;
    size_t is_odd_pos = 0; /* 0 for false, 1 for true */

    /* Process backwards in a single pass to handle length variations and Luhn parity dynamically */
    for (size_t i = len; i > 0; --i) {
        char c = pan[i - 1];

        if (c == ' ' || c == '-') {
            continue;
        }

        if (c < '0' || c > '9') {
            return CARDVAL_ERR_INVALID_CHAR;
        }

        uint8_t digit = (uint8_t)(c - '0');
        sum += LUHN_TABLE[is_odd_pos][digit];
        is_odd_pos ^= 1;
        digits_counted++;
    }

    if (digits_counted < CARDVAL_MIN_DIGITS || digits_counted > CARDVAL_MAX_DIGITS) {
        return CARDVAL_ERR_INVALID_LENGTH;
    }

    if ((sum % 10) != 0) {
        return CARDVAL_ERR_LUHN_MISMATCH;
    }

    return CARDVAL_OK;
}

cardval_status_t cardval_validate_str(const char* pan) {
    if (!pan) {
        return CARDVAL_ERR_NULL_PTR;
    }

    size_t len = 0;
    while (pan[len] != '\0') {
        len++;
    }

    return cardval_validate_buf(pan, len);
}

cardval_status_t cardval_normalize_buf(const char* input, size_t input_len, char* output, size_t* output_len) {
    if (!input || !output || !output_len) {
        return CARDVAL_ERR_NULL_PTR;
    }

    size_t out_idx = 0;
    size_t max_out = *output_len;

    for (size_t i = 0; i < input_len; ++i) {
        char c = input[i];

        if (c == ' ' || c == '-') {
            continue;
        }

        if (c < '0' || c > '9') {
            return CARDVAL_ERR_INVALID_CHAR;
        }

        if (out_idx >= max_out) {
            return CARDVAL_ERR_BUFFER_TOO_SMALL;
        }

        output[out_idx++] = c;
    }

    if (out_idx < CARDVAL_MIN_DIGITS || out_idx > CARDVAL_MAX_DIGITS) {
        return CARDVAL_ERR_INVALID_LENGTH;
    }

    *output_len = out_idx;
    return CARDVAL_OK;
}
