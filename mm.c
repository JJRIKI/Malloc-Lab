
/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Tanner with overhead",
    /* First member's full name */
    "Tanner Leslie",
    /* First member's email address */
    "tleslie@westmont.edu",
    /* Second member's full name (leave blank if none) */
    "James Bek",
    /* Second member's email address (leave blank if none) */
    "jbek@westmont.edu",
    /* Third member's full name (leave blank if none) */
    //"Emily Peterson",
    /* Third member's email address (leave blank if none) */
    //"emilpeterson@westmont.edu",
    /* Fourth member's full name (leave blank if none) */
    //"Ben Thomas",
    /* Fourth member's email address (leave blank if none) */
    //"bethomas@westmont.com"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


#define GET(p) (*(unsigned long*) (p))
#define PUT(p,val) (*(unsigned long*) (p) = (val))

#define GET_NEXT(p) (*(unsigned long*) (p + 1))
#define GET_PREV(p) (*(unsigned long*) (p + 2))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)



void *HEAD;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
        int startSize = 1024;
          HEAD = mem_sbrk(startSize);
          if (mem_heapsize() <  startSize){
                  return -1;
          }

          unsigned long * start = (unsigned long *)  mem_heap_lo();

          *(start) = (unsigned long) startSize; //initializes the header

          *((char *) mem_heap_hi() - 7) = (unsigned long) startSize; //initalizes footer

          *(start + 1) = 0; //sets next free block to null
          *(start + 2) = 0; //sets previous free block to null

          return 0;

}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
        int newsize = (ALIGN(size) + 16); //how many bytes needed
        unsigned long newsizeword = (unsigned long) (newsize/8); //how many words needed
        void *current = HEAD;
        void *best = HEAD;
        while(current != NULL){
                if(GET_SIZE(current) == newsize){ //if exact fit
                        unsigned long *curnext = ((unsigned long *) current) + 1;
                        unsigned long *curprev = ((unsigned long *) current) + 2;

                        *((void **)curnext + 2) = curprev;
                        *((void **)curprev + 1) = curnext;

                        *((unsigned long *) current) =(unsigned long) (newsize + 1); //sets up header
                        *(((unsigned long *) current) + (newsizeword-1)) =(unsigned long) (newsize + 1); //sets up footer
                        return current;
                }
                else if(GET_SIZE(current) > GET_SIZE(best) && GET_SIZE(current) > newsize){
                        best = current;
                }
                current = *(((void **) current) + 1); //moves down the free list
        }
        if(GET_SIZE(best) < newsize){ //sbreak
                void *james = mem_sbrk(newsize/8);
                unsigned long *ben = (*(unsigned long *) (james));
                *ben = (newsize + 1); // set up new block header
                *(ben + (newsize - 1)) = newsize + 1; // set up new block footer

                return (void *)ben;
        }
        else{ //allocate memory
                unsigned long *curnext = ((unsigned long *) current) + 1;
                unsigned long *curprev = ((unsigned long *) current) + 2;
                *(curnext + 2) = *(curnext + 2) - newsize;
                *(curprev + 1) = *(curprev + 1) + newsize;

                unsigned long fsize = (unsigned long) (GET_SIZE(current) - newsize);
                unsigned long *new = ((unsigned long *)current) + newsize;
                *(new) = fsize;

                int n = (fsize - 8) / 8;

                *((void **)new + 1) = curnext;
                *((void **)new + 2) = curprev;
                *(new + n) = fsize;

                *((unsigned long *) current) =(unsigned long) (newsize + 1); //sets up header
                *(((unsigned long *) current) + (newsizeword-1)) =(unsigned long) (newsize + 1); //sets up footer

                unsigned long *ret = ((unsigned long *) current) + 1;
                return (void *) ret;
        }
}


/*
 * mm_coalesce itterates through the linked list and merges
 * neighboring free blocks
 */
int mm_coalesce(unsigned long *ptr) {

        int ptrSize;
        unsigned long one = 0x1;
        // point to adjacent left block footer
        unsigned long *left = ptr - 1;//TODO check if off heap

        if (*(left) & one){
                // get block size of free adjacent free block
                int leftSize = GET_SIZE(left);
                // set left pointer to block header
                left = leftSize - 1;
                // get block size of ptr
                ptrSize = GET_SIZE(ptr);
                // set left header size to left + ptr size header
                PUT(left,leftSize + ptrSize);
                // set ptr footer size to left + ptr size
                PUT((ptr + (ptrSize - 1)), (leftSize + ptrSize));

                // create pointers to left's next and prev blocks
                // TODO if this doesnt work, add another level of value transfer i.e. longs x,y = value of left next/prev then *next and *prev = adress of x,y
                unsigned long *next = *(left + 1);
                unsigned long *prev = *(left + 2);

                // bridge left's next/prev blocks
                mm_bridge(next,prev);
                ptr = left;
        }
        ptrSize = GET_SIZE(ptr);
        unsigned long *right = ptr + ptrSize;//TODO check if off heap
        if (*(right) & one){
                // get block size of free adjacent free block
                int rightSize = GET_SIZE(right);
                // set ptr block header size to ptr + right block size
                PUT(ptr, (ptrSize + rightSize));
                // set right block footer size to ptr + right block size
                PUT((right + (rightSize - 1)), (ptrSize + rightSize));


                // create pointers to right's next and prev blocks
                // TODO if this doesnt work, add another level of value transfer i.e. longs x,y = value of right next/prev then *next and *prev = adress of x,y
                unsigned long *next = *(right + 1);
                unsigned long *prev = *(right + 2);

                // bridge right's next/prev blocks
                mm_bridge(next,prev);

        }

        return 1;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr_)
{
        // cast void pointer to an unsigned long pointer
        unsigned long *ptr = (unsigned long *) (ptr_);
        // set header allocated field bit to 0
        unsigned long x = GET(ptr);
        PUT(ptr,(x & 0x0));
        // set footer allocated feild bit to 0
        x = GET(ptr + (GET_SIZE(ptr) - 1));
        PUT(ptr + (GET_SIZE(ptr)-1),(x & 0x0));

        mm_coalesce(ptr);

        /*TODO add ptr to head of list
        // set next
        PUT(ptr + 1, NEXT_POINTER);
        // set prev
        PUT(ptr + 2, PREV_POINTER);
        */


}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

// mm_bridge bridges the gap between block next and block prev in the linked list
void mm_bridge(unsigned long* next, unsigned long *prev){
        // set next block's previouse field to prev
        PUT(next + 2, prev);
        // set prev block's next field to next
        PUT(prev + 1, next);



}

/*
 * mm_messiah iterates through the linked list and tests equality of
 * free block headers and footers as well as allocated feild bit value
 */
int mm_messiah(void *head_){

        // cast void head pointer to an unsigned long pointer
        unsigned long *head = (*(unsigned long **) (head_));
        int headSize, blockSize;

        // get size of linked list head
        headSize = GET_SIZE(head);
        // make sure head header and footer are identical
        if (headSize != (headSize + (headSize - 1))){
                return -1;
        }
        // check allocated field bit
        if ((headSize & 0x1) || ((headSize + (headSize - 1)) & 0x1)){
                return -1;
        }
        // iterate to second free block
        unsigned long *block = GET_NEXT(head);
        while (block != head){
                // get block size
                blockSize = GET_SIZE(block);
                // make sure block header and footer are identical
                if (blockSize != (blockSize + (blockSize -1 ))){
                        return -1;
                }
                // check allocated field bit
                if ((blockSize & 0x1) || ((blockSize + (blockSize - 1)) & 0x1)){
                        return -1;
                }
                // iterate to next free block
                block = GET_NEXT(block);
        }
        return 0;
}



