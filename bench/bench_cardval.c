#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include "cardval.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#define ITERATIONS 10000000

static double get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}

int main(void) {
    const char* pan = "1234-5678-1234-5670";
    size_t len = strlen(pan);
    
    volatile cardval_status_t sink;
    
    printf("Benchmarking validation of '%s' over %d iterations...\n", pan, ITERATIONS);
    
    double start = get_time_ns();
    for (int i = 0; i < ITERATIONS; i++) {
        sink = cardval_validate_buf(pan, len);
    }
    double end = get_time_ns();
    
    /* Use the sink to prevent the loop from being completely optimized away */
    if (sink != CARDVAL_OK) {
        printf("Unexpected benchmark failure.\n");
        return 1;
    }
    
    double total_time_ns = end - start;
    double ns_per_op = total_time_ns / ITERATIONS;
    double ops_per_sec = (ITERATIONS / (total_time_ns / 1e9));
    
    printf("Results (Expected vs Measured dependent on local hardware):\n");
    printf("  Total Time: %.2f ms\n", total_time_ns / 1e6);
    printf("  Latency:    %.2f ns/op\n", ns_per_op);
    printf("  Throughput: %.2f million ops/sec\n", ops_per_sec / 1e6);
    
    return 0;
}
