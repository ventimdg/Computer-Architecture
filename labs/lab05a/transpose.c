#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    for (int yBlock = 0; yBlock < n; yBlock += blocksize){
        int yBlockEnd = yBlock + blocksize;
        if (yBlockEnd > n) {
            yBlockEnd = n;
        }
        for (int xBlock = 0; xBlock < n; xBlock += blocksize){
            int xBlockEnd = xBlock + blocksize;
            if (xBlockEnd > n) {
                xBlockEnd = n;
            }
            for (int y = yBlock; y < yBlockEnd; y++){
                for (int x = xBlock; x < xBlockEnd; x++){
                    dst[y + x*n] = src[x + y * n];
                }
            }
        }
    }
}

