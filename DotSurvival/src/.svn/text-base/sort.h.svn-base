#ifndef SORT_H
#define SORT_H

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdint.h>
#include "Src/Utils.h"


#ifndef SORT_NAME
#error "Must declare SORT_NAME"
#endif

#ifndef SORT_TYPE
#error "Must declare SORT_TYPE"
#endif

//#ifndef CLZ
//#ifdef __GNUC__
//#define CLZ __builtin_clzll
//#else


// adapted from Hacker's Delight
int_t clz32( uint32 x )
{
  if (x == 0) return(32);
  int_t n = 1;
  if (x <= 0x0000FFFFL) {n = n + 16; x = x << 16;}
  if (x <= 0x00FFFFFFL) {n = n + 8; x = x << 8;}
  if (x <= 0x0FFFFFFFL) {n = n + 4; x = x << 4;}
  if (x <= 0x3FFFFFFFL) {n = n + 2; x = x << 2;}
  //if (x <= 0x7FFFFFFFL) {n = n + 1;}
  n = n - ( x >> 31 );
  return n;
}

int_t clz_orig( uint64 x )
{
  if (x == 0) return(64);
  int_t n = 0;
  if (x <= 0x00000000FFFFFFFFL) {n = n + 32; x = x << 32;}
  if (x <= 0x0000FFFFFFFFFFFFL) {n = n + 16; x = x << 16;}
  if (x <= 0x00FFFFFFFFFFFFFFL) {n = n + 8; x = x << 8;}
  if (x <= 0x0FFFFFFFFFFFFFFFL) {n = n + 4; x = x << 4;}
  if (x <= 0x3FFFFFFFFFFFFFFFL) {n = n + 2; x = x << 2;}
  if (x <= 0x7FFFFFFFFFFFFFFFL) {n = n + 1;}
  //n = n - ( x >> 31 );
  return n;
}

int __clz32( uint32 x )
{
	// This uses a binary search (counting down) algorithm from Hacker's Delight.
	int n = 32;
	uint32 y;
	y = x >>16;  if (y != 0) { n = n -16;  x = y; }
	y = x >> 8;  if (y != 0) { n = n - 8;  x = y; }
	y = x >> 4;  if (y != 0) { n = n - 4;  x = y; }
	y = x >> 2;  if (y != 0) { n = n - 2;  x = y; }
	y = x >> 1;  if (y != 0) return n - 2;
	return n - x;
}

int __ctz32( uint32 x )
{
  // This uses a binary search algorithm from Hacker's Delight.
  int n = 1;
  if ((x & 0x0000FFFF) == 0) { n = n +16; x = x >>16; }
  if ((x & 0x000000FF) == 0) { n = n + 8; x = x >> 8; }
  if ((x & 0x0000000F) == 0) { n = n + 4; x = x >> 4; }
  if ((x & 0x00000003) == 0) { n = n + 2; x = x >> 2; }
  return n - (x & 1);
}

//#define CLZ clzll
//#endif
//#endif


#define SORT_SWAP(x,y) { SORT_TYPE __SORT_SWAP_t = (x); (x) = (y); (y) = __SORT_SWAP_t; }
//#define SORT_SWAP(x,y) ( { SORT_TYPE tmp = (x); (x) = (y); (y) = tmp; } )

#define SORT_CONCAT(x, y) x ## _ ## y
#define SORT_MAKE_STR1(x, y) SORT_CONCAT(x,y)
#define SORT_MAKE_STR(x) SORT_MAKE_STR1(SORT_NAME,x)

#ifndef SORT_CMP
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
#endif

#define SHELL_SORT             SORT_MAKE_STR(shell_sort)
#define BINARY_INSERTION_SORT  SORT_MAKE_STR(binary_insertion_sort)
#define HEAP_SORT              SORT_MAKE_STR(heap_sort)
#define QUICK_SORT             SORT_MAKE_STR(quick_sort)
#define MERGE_SORT             SORT_MAKE_STR(merge_sort)
#define BUBBLE_SORT            SORT_MAKE_STR(bubble_sort)
#define TIM_SORT               SORT_MAKE_STR(tim_sort)

#define TIM_SORT_RUN_T         SORT_MAKE_STR(tim_sort_run_t)
#define TEMP_STORAGE_T         SORT_MAKE_STR(temp_storage_t)

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif
#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif

typedef struct {
  int_t start;
  int_t length;  
} TIM_SORT_RUN_T;


void SHELL_SORT(SORT_TYPE *dst, const int_t size);
void BINARY_INSERTION_SORT(SORT_TYPE *dst, const int_t size);
void HEAP_SORT(SORT_TYPE *dst, const int_t size);
void QUICK_SORT(SORT_TYPE *dst, const int_t size);
void MERGE_SORT(SORT_TYPE *dst, const int_t size);
void BUBBLE_SORT(SORT_TYPE *dst, const int_t size);
void TIM_SORT(SORT_TYPE *dst, const int_t size);

/*
// From http://oeis.org/classic/A102549
static const uint64_t shell_gaps[48] = {1, 4, 10, 23, 57, 132, 301, 701, 1750, 4376, 10941, 27353, 68383, 170958, 427396, 1068491, 2671228, 6678071, 16695178, 41737946, 104344866, 260862166, 652155416, 1630388541, 4075971353LL, 10189928383LL, 25474820958LL, 63687052396LL, 159217630991LL, 398044077478LL, 995110193696LL, 2487775484241LL, 6219438710603LL, 15548596776508LL, 38871491941271LL, 97178729853178LL, 242946824632946LL, 607367061582366LL, 1518417653955916LL, 3796044134889791LL, 9490110337224478LL, 23725275843061196LL, 59313189607652991LL, 148282974019132478LL, 370707435047831196LL, 926768587619577991LL, 2316921469048944978LL, 5792303672622362446LL};

// Shell sort implementation based on Wikipedia article
//   http://en.wikipedia.org/wiki/Shell_sort
//
void SHELL_SORT(SORT_TYPE *dst, const int_t size)
{
  // TODO: binary search to find first gap?
  int inci = 47;
  int_t inc = shell_gaps[inci];
  while (inc > (size >> 1))
  {
    inc = shell_gaps[--inci];
  }
  int_t i;
  while (1)
  {
    for (i = inc; i < size; i++)
    {
      SORT_TYPE temp = dst[i];
      int_t j = i;
      while ((j >= inc) && (SORT_CMP(dst[j - inc], temp) > 0))
      {
        dst[j] = dst[j - inc];
        j -= inc;
      }
      dst[j] = temp;
    }
    if (inc == 1) break;
    inc = shell_gaps[--inci];
  }
}
*/

/* Function used to do a binary search for binary insertion sort */
static inline int_t binary_insertion_find(SORT_TYPE *dst, const SORT_TYPE x, const int_t size)
{
  int_t l, c, r;
  l = 0;
  r = size - 1;
  c = r >> 1;
  SORT_TYPE lx;
  SORT_TYPE cx;
  SORT_TYPE rx;
  lx = dst[l];
  
  /* check for beginning conditions */
  if (SORT_CMP(x, lx) < 0)
    return 0;
  else if (SORT_CMP(x, lx) == 0)
  {
    int_t i = 1;
    while (SORT_CMP(x, dst[i]) == 0) i++;
    return i;
  }
  
  rx = dst[r];
  // guaranteed not to be >= rx
  cx = dst[c];
  while (1)
  {
    const int_t val = SORT_CMP(x, cx);
    if (val < 0)
    {
      if (c - l <= 1) return c;
      r = c;
      rx = cx;
    }
    else if (val > 0)
    {
      if (r - c <= 1) return c + 1;
      l = c;
      lx = cx;
    }
    else
    {
      while (cx == x)
      {
        cx = dst[++c];
      }
      return c;
    }
    c = l + ((r - l) >> 1);
    cx = dst[c];
  }
}

/* Binary insertion sort, but knowing that the first "start" entries are sorted.  Used in timsort. */
static inline void binary_insertion_sort_start(SORT_TYPE *dst, const int_t start, const int_t size)
{
  int_t i;
  for (i = start; i < size; i++)
  {
    int_t j;
    /* If this entry is already correct, just move along */
    if (SORT_CMP(dst[i - 1], dst[i]) <= 0) continue;
    
    /* Else we need to find the right place, shift everything over, and squeeze in */
    SORT_TYPE x = dst[i];
    int_t location = binary_insertion_find(dst, x, i);
    for (j = i - 1; j >= location; j--)
    {
      dst[j + 1] = dst[j];
    }
    dst[location] = x;
  }
}

/* Binary insertion sort */
void BINARY_INSERTION_SORT(SORT_TYPE *dst, const int_t size)
{
  binary_insertion_sort_start(dst, 1, size);
}


void BUBBLE_SORT(SORT_TYPE *dst, const int_t size)
{
  for( int_t i = 0; i < size; i++ )
  for( int_t j = i + 1; j < size; j++ )
  {
      if( SORT_CMP( dst[j],dst[i] ) < 0 )
	  {
		  //p2t::Point* tmp = dst[i]; dst[i] = dst[j]; dst[j] = tmp;
        SORT_SWAP( dst[i], dst[j] );
	  }
  }
}

/*
void MERGE_SORT( SORT_TYPE *dst, const int_t size )
{
  if( size < 16 )
  {
    BINARY_INSERTION_SORT( dst, size );
    return;
  }

  const int_t middle = size / 2;
  
  MERGE_SORT( dst, middle );
  MERGE_SORT( &dst[middle], size - middle );
  
  SORT_TYPE newdst[size];
  int_t out = 0;
  int_t i = 0;
  int_t j = middle;
  while (out != size)
  {
    if (i < middle)
    {
      if (j < size)
      {
        if (SORT_CMP(dst[i], dst[j]) <= 0)
          newdst[out] = dst[i++];
        else
          newdst[out] = dst[j++];
      }
      else
        newdst[out] = dst[i++];
    }
    else
      newdst[out] = dst[j++];
    out++;
  }
  memcpy(dst, newdst, size * sizeof(SORT_TYPE));
}
*/

/* quick sort: based on wikipedia */

static inline int_t quick_sort_partition(SORT_TYPE *dst, const int_t left, const int_t right, const int_t pivot)
{
  SORT_TYPE value = dst[pivot];
  SORT_SWAP(dst[pivot], dst[right]);
  int_t index = left;
  int_t i;
  for (i = left; i < right; i++)
  {
    if (dst[i] <= value)
    {
      SORT_SWAP(dst[i], dst[index]);
      index++;
    }
  }
  SORT_SWAP(dst[right], dst[index]);
  return index;
}

static void quick_sort_recursive(SORT_TYPE *dst, const int_t left, const int_t right)
{
  if (right <= left) return;
  if ((right - left + 1) < 16)
  {
    BINARY_INSERTION_SORT(&dst[left], right - left + 1);
    return;
  }
  const int_t pivot = left + ((right - left) >> 1);
  const int_t new_pivot = quick_sort_partition(dst, left, right, pivot);
  quick_sort_recursive(dst, left, new_pivot - 1);
  quick_sort_recursive(dst, new_pivot + 1, right);
}

void QUICK_SORT(SORT_TYPE *dst, const int_t size)
{
  quick_sort_recursive(dst, 0, size - 1);
}


/* timsort implementation, based on timsort.txt */

static inline void reverse_elements(SORT_TYPE *dst, int_t start, int_t end)
{
  while (1)
  {
    if (start >= end) return;
    SORT_SWAP(dst[start], dst[end]);
    start++;
    end--;
  }
}

static inline int_t count_run(SORT_TYPE *dst, const int_t start, const int_t size)
{
  if (size - start == 1) return 1;
  if (start >= size - 2)
  {
    if (SORT_CMP(dst[size - 2], dst[size - 1]) > 0)
      SORT_SWAP(dst[size - 2], dst[size - 1]);
    return 2;
  }
  
  int_t curr = start + 2;
  
  if (SORT_CMP(dst[start], dst[start + 1]) <= 0)
  {
    // increasing run
    while (1)
    {
      if (curr == size - 1) break;
      if (SORT_CMP(dst[curr - 1], dst[curr]) > 0) break;
      curr++;
    }
    return curr - start;    
  }
  else
  {
    // decreasing run
    while (1)
    {
      if (curr == size - 1) break;
      if (SORT_CMP(dst[curr - 1], dst[curr]) <= 0) break;
      curr++;
    }
    // reverse in-place
    reverse_elements(dst, start, curr - 1);
    return curr - start;
  }
}

static inline int_t compute_minrun( const int_t size )
{
	//const int_t top_bit_tmp = 64 - clz_orig( size );

  const int_t top_bit = 32 - clz32( size );
  const int_t shift = MAX( top_bit, 6 ) - 6;
  const int_t minrun = size >> shift;
  const int_t mask = ( 1 << shift ) - 1;
  if( mask & size ) return minrun + 1;
  return minrun;
}

#define PUSH_NEXT() do\
{\
len = count_run(dst, curr, size);\
run = minrun;\
if (run < minrun) run = minrun;\
if (run > size - curr) run = size - curr;\
if (run > len)\
{\
  binary_insertion_sort_start(&dst[curr], len, run);\
  len = run;\
}\
run_stack[stack_curr].start = curr;\
run_stack[stack_curr++].length = len;\
curr += len;\
if (curr == size)\
{\
  /* finish up */ \
  while (stack_curr > 1) \
  { \
    tim_sort_merge(dst, run_stack, stack_curr, store); \
    stack_curr--; \
  } \
  if (store->storage != NULL)\
  {\
  wzx_Free(store->storage);\
  }\
  return;\
}\
} while(0)
  
static inline int_t check_invariant( TIM_SORT_RUN_T *stack, const int_t stack_curr )
{
  if (stack_curr < 2) return 1;
  if (stack_curr == 2)
  {
    const int_t A = stack[stack_curr-2].length;
    const int_t B = stack[stack_curr-1].length;
    if (A <= B) return 0;
    return 1;
  }
  const int_t A = stack[stack_curr - 3].length;
  const int_t B = stack[stack_curr - 2].length;
  const int_t C = stack[stack_curr - 1].length;
  if ((A <= B + C) || (B <= C)) return 0;
  return 1;
}

typedef struct {
  int_t alloc;
  SORT_TYPE *storage;
} TEMP_STORAGE_T;
  

static inline void tim_sort_resize(TEMP_STORAGE_T *store, const int_t new_size)
{
  if (store->alloc < new_size)
  {
	  SORT_TYPE *tempstore = (SORT_TYPE*)wzx_ReAlloc(store->storage, new_size * sizeof(SORT_TYPE));
    if (tempstore == NULL)
    {
		wzx_ASSERT(false);
      //fprintf(stderr, "Error allocating temporary storage for tim sort: need %lu bytes", sizeof(SORT_TYPE) * new_size);
      //exit(1);
    }
    store->storage = tempstore;
    store->alloc = new_size;
  }
}

static inline void tim_sort_merge(SORT_TYPE *dst, const TIM_SORT_RUN_T *stack, const int_t stack_curr, TEMP_STORAGE_T *store)
{
  const int_t A = stack[stack_curr - 2].length;
  const int_t B = stack[stack_curr - 1].length;
  const int_t curr = stack[stack_curr - 2].start;

  tim_sort_resize(store, MIN(A, B));
  SORT_TYPE *storage = store->storage;
  
  int_t i, j, k;
  
  // left merge
  if (A < B)
  {
	wzx_MemCopy( &dst[curr], storage, A * sizeof(SORT_TYPE));
    i = 0;
    j = curr + A;
    
    for (k = curr; k < curr + A + B; k++)
    {
      if ((i < A) && (j < curr + A + B))
      {
        if (SORT_CMP(storage[i], dst[j]) <= 0)
          dst[k] = storage[i++];
        else
          dst[k] = dst[j++];          
      }
      else if (i < A)
      {
        dst[k] = storage[i++];
      }
      else
        dst[k] = dst[j++];
    }
  }
  // right merge
  else
  {    
    wzx_MemCopy(&dst[curr + A],storage, B * sizeof(SORT_TYPE));
    i = B - 1;
    j = curr + A - 1;
    
    for (k = curr + A + B - 1; k >= curr; k--)
    {
      if ((i >= 0) && (j >= curr))
      {
          if (SORT_CMP(dst[j], storage[i]) > 0)
            dst[k] = dst[j--];
          else
            dst[k] = storage[i--];          
      }
      else if (i >= 0)
        dst[k] = storage[i--];
      else
        dst[k] = dst[j--];
    }
  }
}

static inline int_t tim_sort_collapse(SORT_TYPE *dst, TIM_SORT_RUN_T *stack, int stack_curr, TEMP_STORAGE_T *store, const int_t size)
{
  while (1)
  {
    // if the stack only has one thing on it, we are done with the collapse
    if (stack_curr <= 1) break;
    // if this is the last merge, just do it
    if ((stack_curr == 2) && (stack[0].length + stack[1].length == size))
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    // check if the invariant is off for a stack of 2 elements
    else if ((stack_curr == 2) && (stack[0].length <= stack[1].length))
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[0].length += stack[1].length;
      stack_curr--;
      break;
    }
    else if (stack_curr == 2)
      break;
      
    const int_t A = stack[stack_curr-3].length;
    const int_t B = stack[stack_curr-2].length;
    const int_t C = stack[stack_curr-1].length;
    
    // check first invariant
    if (A <= B + C)
    {
      if (A < C)
      {
        tim_sort_merge(dst, stack, stack_curr - 1, store);
        stack[stack_curr - 3].length += stack[stack_curr - 2].length;
        stack[stack_curr - 2] = stack[stack_curr - 1];
        stack_curr--;
      }
      else
      {
        tim_sort_merge(dst, stack, stack_curr, store);
        stack[stack_curr - 2].length += stack[stack_curr - 1].length;
        stack_curr--;
      }
    }
    // check second invariant
    else if (B <= C)
    {
      tim_sort_merge(dst, stack, stack_curr, store);
      stack[stack_curr - 2].length += stack[stack_curr - 1].length;
      stack_curr--;      
    }
    else
      break;
  }
  return stack_curr;
}

void TIM_SORT(SORT_TYPE *dst, const int_t size)
{
  if (size < 64)
  {
    BINARY_INSERTION_SORT(dst, size);
    return;
  }

  // compute the minimum run length
  const int_t minrun = compute_minrun( size );

  // temporary storage for merges
  TEMP_STORAGE_T _store, *store = &_store;
  store->alloc = (size>>1) + 1;
  store->storage = (SORT_TYPE*)wzx_Malloc( store->alloc * sizeof(SORT_TYPE) );

  wzx_ASSERT( store->storage );

  TIM_SORT_RUN_T run_stack[128];
  int_t stack_curr = 0;
  int_t len, run;
  int_t curr = 0;

  PUSH_NEXT();
  PUSH_NEXT();
  PUSH_NEXT();
  
  while (1)
  {
    if (!check_invariant(run_stack, stack_curr))
    {
      stack_curr = tim_sort_collapse(dst, run_stack, stack_curr, store, size);
      continue;
    }
    PUSH_NEXT();
  }
}



/* heap sort: based on wikipedia */

static inline void heap_sift_down(SORT_TYPE *dst, const int_t start, const int_t end)
{
  int_t root = start;
  
  while ((root << 1) <= end)
  {
    int_t child = root << 1;
    if ((child < end) && (SORT_CMP(dst[child], dst[child + 1]) < 0))
      child++;
    if (SORT_CMP(dst[root], dst[child]) < 0)
    {
      SORT_SWAP(dst[root], dst[child]);
      root = child;
    }
    else
      return;
  }
}

static inline void heapify(SORT_TYPE *dst, const int_t size)
{
  int_t start = size >> 1;
  while (start >= 0)
  {
    heap_sift_down(dst, start, size - 1);
    start--;
  }
}

void HEAP_SORT(SORT_TYPE *dst, const int_t size)
{
  heapify(dst, size);
  int_t end = size - 1;
  
  while (end > 0)
  {
    SORT_SWAP(dst[end], dst[0]);
    heap_sift_down(dst, 0, end - 1);
    end--;
  }
}

#undef SORT_CONCAT
#undef SORT_MAKE_STR1
#undef SORT_MAKE_STR
#undef SORT_NAME

#undef TEMP_STORAGE_T
#undef TIM_SORT_RUN_T
#undef PUSH_NEXT

#endif