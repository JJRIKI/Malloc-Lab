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
    "Emily Peterson",
    /* Third member's email address (leave blank if none) */
    "emilpeterson@westmont.edu",
    /* Fourth member's full name (leave blank if none) */
    "Ben Thomas",
    /* Fourth member's email address (leave blank if none) */
    "bethomas@westmont.com"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define GET(p) (*(unsigned long*) (p))
#define PUT(p,val) (*(unsigned long*) (p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)



void *HEAD;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
        int startSize = 1024;
        mem_sbrk(startSize);
        if (mem_heapsize() <  startSize){
                 return -1;
        }
        HEAD = mem_heap_lo();
	return 0;

}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
        // set header indicator bit to 0
        unsigned long x = GET(ptr);
        PUT(ptr,(x & 0x0));
        // set footer indicator bit to 0
        x = GET(ptr + (GET_SIZE(ptr) - 1));
        PUT(ptr + (GET_SIZE(ptr)-1),(x & 0x0));


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

/*
 * mm_coalesce itterates through the linked list and merges 	
 * neighboring free blocks
 */
int mm_coalesce()
{
	return 1;
}










