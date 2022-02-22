#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

#define block_padded_size  PADDED_SIZE(sizeof(struct block))

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};