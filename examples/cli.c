#include "cardval.h"
#include <stdio.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 128

int main(void) {
    char input[INPUT_BUFFER_SIZE];

    printf("Enter a card number to validate (or Ctrl+D to exit): ");
    
    /* Use fgets safely, avoid %d/atoi and fprintf to stdin */
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    cardval_status_t status = cardval_validate_buf(input, len);

    switch (status) {
        case CARDVAL_OK:
            printf("Result: VALID\n");
            return 0;
        case CARDVAL_ERR_LUHN_MISMATCH:
            fprintf(stderr, "Result: INVALID (Luhn checksum mismatch)\n");
            break;
        case CARDVAL_ERR_INVALID_LENGTH:
            fprintf(stderr, "Result: INVALID (Length out of standard bounds)\n");
            break;
        case CARDVAL_ERR_INVALID_CHAR:
            fprintf(stderr, "Result: INVALID (Contains letters, punctuation, or NULs)\n");
            break;
        case CARDVAL_ERR_NULL_PTR:
        case CARDVAL_ERR_BUFFER_TOO_SMALL:
            fprintf(stderr, "Result: ERROR (Internal API misuse)\n");
            break;
    }

    return 1;
}
