* Understand 
* Plan

// myalloc
//     if this is the first run:
//         do sbrk
//         do first linked list node setup

    // cur = front of the list 
    // while cur is != NULL 
    //     if this node is big enough and not in use:
    //     mark it in use 
    //     return pointer to node data

    // return NULL

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - ((x) - 1) & (ALIGNMENT - 1))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

void print_data(void) {
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

void *myalloc(size_t size) {

	if (head == NULL) { 
    	head = sbrk(1024);
    	head->next = NULL;
    	head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    	head->in_use = 0;
    }

    (void)size;

    int padded_size = PADDED_SIZE(size);
    int block_padded_size = PADDED_SIZE(sizeof(struct block));

    struct block *cur = head;
    while (cur != NULL) {
        // if this node is big enough and not in use:
        if (!cur->in_use && cur->size >= padded_size) {
            // mark it in use
            cur->in_use = 1;
            printf("Found one!\n");

            // return pointer to node data
            return PTR_OFFSET(cur, block_padded_size);
        }
        cur = cur->next;
    }
    return NULL;
}

int main(void) {
    print_data();
    char *p = myalloc(60);
    printf("%p\n", p);
    print_data();
}