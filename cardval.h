#ifndef CARDVAL_H
#define CARDVAL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CARDVAL_MIN_DIGITS 8
#define CARDVAL_MAX_DIGITS 19

/**
 * @brief Status codes returned by the library.
 */
typedef enum {
    CARDVAL_OK = 0,
    CARDVAL_ERR_NULL_PTR = -1,
    CARDVAL_ERR_INVALID_LENGTH = -2,
    CARDVAL_ERR_INVALID_CHAR = -3,
    CARDVAL_ERR_LUHN_MISMATCH = -4,
    CARDVAL_ERR_BUFFER_TOO_SMALL = -5
} cardval_status_t;

/**
 * @brief Validates a payment card number buffer.
 *
 * Runs Luhn's algorithm strictly in O(n) time and O(1) space. Ignores spaces
 * and hyphens. Detects malformed inputs and invalid lengths.
 *
 * @param pan Pointer to the buffer.
 * @param len Length of the buffer in bytes.
 * @return CARDVAL_OK on valid, or an error code.
 */
cardval_status_t cardval_validate_buf(const char* pan, size_t len);

/**
 * @brief Validates a NUL-terminated payment card number string.
 *
 * @param pan Pointer to the NUL-terminated string.
 * @return CARDVAL_OK on valid, or an error code.
 */
cardval_status_t cardval_validate_str(const char* pan);

/**
 * @brief Normalizes a payment card number by extracting only its digits.
 *
 * @param input Pointer to the source buffer.
 * @param input_len Length of the source buffer.
 * @param output Pointer to the destination buffer.
 * @param output_len On input, the max size of output buffer. On success, updated to actual length.
 * @return CARDVAL_OK on success, or an error code.
 */
cardval_status_t cardval_normalize_buf(const char* input, size_t input_len, char* output, size_t* output_len);

#ifdef __cplusplus
}
#endif

#endif /* CARDVAL_H */
