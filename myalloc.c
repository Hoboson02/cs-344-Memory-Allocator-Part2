// Ryan Earl

#include "myalloc.h"

struct block *head = NULL;

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
            //  # vvv splitting code ^^^
            split_space(cur, size);

            // # ^^^ splitting code ^^^
            // mark it in use
            cur->in_use = 1;
            // printf("Found one!\n");

            // return pointer to node data
            return PTR_OFFSET(cur, block_padded_size);
        }
        cur = cur->next;
    }
    return NULL;
}

void split_space (struct block *s, size_t size) {
    int required_space = PADDED_SIZE(size) + PADDED_SIZE(sizeof(struct block)) + 16;
    
    if (s->size >= required_space) {
        struct block *split_node = PTR_OFFSET(s, PADDED_SIZE(size) + PADDED_SIZE(sizeof(struct block))); // The data space requested by the user
        split_node->in_use = 0;
        split_node->size = s->size - (PADDED_SIZE(size) + (PADDED_SIZE(sizeof(struct block)))); // A new struct block (padded)
        split_node->next = NULL;
        s->next = split_node;
        s->size = PADDED_SIZE(size);
    }
}

void myfree (void *p) { 
    struct block *b = p - (PADDED_SIZE(sizeof(struct block)));
    b->in_use = 0;
}

int main(void) {
    void *p;

    myalloc(10);     print_data();
    p = myalloc(20); print_data();
    myalloc(30);     print_data();
    myfree(p);       print_data();
    myalloc(40);     print_data();
    myalloc(10);     print_data();
}