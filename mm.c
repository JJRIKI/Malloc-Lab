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
    "",
    /* Second member's full name (leave blank if none) */
    "James Bek",
    /* Second member's email address (leave blank if none) */
    "Emily Peterson"
    "emilpeterson@westmont.edu"
    "Ben Thomas"
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
	//TODO cast ptr to unsigned long *
        // set header indicator bit to 0
        unsigned long x = GET(ptr);
        PUT(ptr,(x & 0x0));
        // set footer indicator bit to 0
        x = GET(ptr + (GET_SIZE(ptr) - 1));
        PUT(ptr + (GET_SIZE(ptr)-1),(x & 0x0));

	//TODO cast ptr to void *
	coalesce(ptr);
	
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

/*
 * mm_coalesce itterates through the linked list and merges 	
 * neighboring free blocks
 */
int mm_coalesce(void *ptr)
{
	int ptrSize;
	//TODO cast ptr to unsigned long *
	
	// point to adjacent left block footer
	unsigned long *left = ptr - 1;//TODO check if off heap
	if ((left) & 0x1){
		// get block size of free adjacent free block
		int leftSize = GET_SIZE(left);
		// set left pointer to block header 
		left = leftSize - 1;
		// get block size of ptr
		ptrSize = GET_SIZE(ptr);
		// set left header size to left + ptr size header		
		PUT(left,leftSize + ptrSize);
		// set ptr footer size to left + ptr size
		PUT((ptr + (ptrSize - 1)), (leftSize + ptrSize);

		// create pointers to left's next and prev blocks
		// TODO if this doesnt work, add another level of value transfer i.e. longs x,y = value of left next/prev then *next and *prev = adress of x,y
		unsigned long *next = *(left + 1);
		unsigned long *prev = *(left + 2);
		
		// bridge left's next/prev blocks
		mm_bridge(next,prev);
		ptr = left;

	ptrSize = GET_SIZE(ptr);
	unsigned long *right = ptr + ptrSize;//TODO check if off heap
	if ((right) & 0x1){
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

// mm_bridge bridges the gap between block next and block prev in the linked list
void mm_bridge(unsigned long* next, unsigned long *prev){
	// set next block's previouse field to prev
	PUT(next + 2, prev);
	// set prev block's next field to next
	PUT(prev + 1, next);	



}






