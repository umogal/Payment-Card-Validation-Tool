# PCVT


<img width="1672" height="941" alt="image" src="https://github.com/user-attachments/assets/96d00249-8f78-451e-b03d-bc1c5720e82e" />

PCVTAn offline Multi-Layer Card Validation Engine that performs Luhn verification, issuer identification, and heuristic pattern analysis to flag potentially invalid card numbers.Cli[link] | Library[link]Features:Offline & Fast: Sub-microsecond validation using pure C11 without network calls or external libs.Zero Heap Allocations: Runs strictly with $O(n)$ time and $O(1)$ space memory footprint.Safe Input Processing: Direct character processing prevents integer overflows from long input strings.Auto Cleaning: Handles spaces and hyphens automatically while stripping out non-digit junk.Detailed Error Codes: Gives exact status returns instead of a simple pass/fail boolean.Language Flexible: Includes raw C library, standalone CLI tool, and Python ctypes bindings.Note :This project is an independent, open-source academic tool created for educational and research purposes. It is not affiliated with, endorsed by, or associated with Visa, Mastercard, or any other payment network.UsageCliStandalone CLI ExampleBuild the CLI binary with standard GCC/Clang:Bashgcc -O3 -Iinclude src/cardval.c examples/cli.c -o pcvt_cli
Run validation directly from command line arguments:Bash./pcvt_cli "4532-0151-1283-0366"
Or run interactively:Bash./pcvt_cli
# Enter credit card number: 49927398716
LibraryHow to useInclude cardval.h in your project and pass your string into cardval_validate_str():C#include "cardval.h"
#include <stdio.h>

int main(void) {
    const char* card_number = "4992-7398-716";
    
    cardval_status_t status = cardval_validate_str(card_number);

    if (status == CARDVAL_OK) {
        printf("Card number is valid!\n");
    } else {
        printf("Invalid card! Error code: %d\n", status);
    }
    return 0;
}
How to use as backendCompile cardval.c as a static (.a) or dynamic (.so/.dll) library to link into your backend services:Bash# Build static library
gcc -c -O3 -Iinclude src/cardval.c -o cardval.o
ar rcs libcardval.a cardval.o

# Build shared library for API backends
gcc -shared -fPIC -O3 -Iinclude src/cardval.c -o libcardval.so
Link against your backend code:Bashgcc -O3 -Iinclude main_backend.c -L. -lcardval -o backend_app
How to use with pythonBuild the shared library (libcardval.so), then load it using Python's built-in ctypes:Pythonimport ctypes

# Load compiled C library
lib = ctypes.CDLL("./libcardval.so")

# Setup argument and return types
lib.cardval_validate_str.argtypes = [ctypes.c_char_p]
lib.cardval_validate_str.restype = ctypes.c_int

# Validate card number
card_input = b"4532-0151-1283-0366"
status = lib.cardval_validate_str(card_input)

if status == 0:
    print("Valid card number!")
else:
    print(f"Validation failed with error code: {status}")
License:Part of a simple small case study in card validation heuristics and systems programming. MIT License.Legal :The software is intended solely for offline experimentation, learning, and demonstration. It is not designed for production environments or real financial transaction processing.Educational open-source project for offline payment card number validation and experimentation. Not affiliated with Visa, Mastercard, or any payment network. Designed with reference to general PCI-DSS security concepts for learning purposes only.
