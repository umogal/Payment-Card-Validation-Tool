#include "cardval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Deliberately simple reference implementation for differential testing */
static cardval_status_t ref_validate(const char* str, size_t len) {
    char digits[64];
    size_t count = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == ' ' || str[i] == '-') continue;
        if (str[i] < '0' || str[i] > '9') return CARDVAL_ERR_INVALID_CHAR;
        if (count >= 64) return CARDVAL_ERR_INVALID_LENGTH;
        digits[count++] = str[i];
    }

    if (count < CARDVAL_MIN_DIGITS || count > CARDVAL_MAX_DIGITS) {
        return CARDVAL_ERR_INVALID_LENGTH;
    }

    int sum = 0;
    int is_second = 0;
    for (int i = (int)count - 1; i >= 0; i--) {
        int d = digits[i] - '0';
        if (is_second) {
            d = d * 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        is_second = !is_second;
    }

    return (sum % 10 == 0) ? CARDVAL_OK : CARDVAL_ERR_LUHN_MISMATCH;
}

static void assert_differential(const char* input, size_t len) {
    cardval_status_t optimized = cardval_validate_buf(input, len);
    cardval_status_t reference = ref_validate(input, len);
    if (optimized != reference) {
        fprintf(stderr, "Differential mismatch on '%s': opt=%d, ref=%d\n", input, optimized, reference);
        exit(1);
    }
}

static void test_valid_cases(void) {
    const char* valid_pans[] = {
        "49927398716", "49927398717", "1234567812345670",
        "1234-5678-1234-5670", " 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 0 ",
        "79927398713" 
    };
    
    for (size_t i = 0; i < sizeof(valid_pans) / sizeof(valid_pans[0]); i++) {
        assert(cardval_validate_str(valid_pans[i]) == CARDVAL_OK);
        assert_differential(valid_pans[i], strlen(valid_pans[i]));
    }
}

static void test_invalid_cases(void) {
    struct {
        const char* str;
        cardval_status_t expected;
    } invalid[] = {
        {"49927398715", CARDVAL_ERR_LUHN_MISMATCH},
        {"1234567", CARDVAL_ERR_INVALID_LENGTH},
        {"12345678901234567890", CARDVAL_ERR_INVALID_LENGTH},
        {"1234567a12345670", CARDVAL_ERR_INVALID_CHAR},
        {"1234_5678", CARDVAL_ERR_INVALID_CHAR},
        {"", CARDVAL_ERR_INVALID_LENGTH},
        {"- - - -", CARDVAL_ERR_INVALID_LENGTH}
    };

    for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++) {
        assert(cardval_validate_str(invalid[i].str) == invalid[i].expected);
        assert_differential(invalid[i].str, strlen(invalid[i].str));
    }
}

static void test_embedded_nul(void) {
    const char pan_with_nul[] = {'1', '2', '3', '4', '\0', '5', '6', '7', '8', '9', '0', '1', '2'};
    assert(cardval_validate_buf(pan_with_nul, sizeof(pan_with_nul)) == CARDVAL_ERR_INVALID_CHAR);
}

static void test_normalization(void) {
    const char* dirty = " 1234 - 5678 - 1234 - 5670 ";
    char clean[32];
    size_t clean_len = sizeof(clean);
    
    assert(cardval_normalize_buf(dirty, strlen(dirty), clean, &clean_len) == CARDVAL_OK);
    assert(clean_len == 16);
    assert(strncmp(clean, "1234567812345670", 16) == 0);
    
    clean_len = 10; 
    assert(cardval_normalize_buf(dirty, strlen(dirty), clean, &clean_len) == CARDVAL_ERR_BUFFER_TOO_SMALL);
}

int main(void) {
    test_valid_cases();
    test_invalid_cases();
    test_embedded_nul();
    test_normalization();
    
    assert(cardval_validate_buf(NULL, 10) == CARDVAL_ERR_NULL_PTR);
    assert(cardval_validate_str(NULL) == CARDVAL_ERR_NULL_PTR);
    
    printf("All unit tests and differential tests passed.\n");
    return 0;
}
