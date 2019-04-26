
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





