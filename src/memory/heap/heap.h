#ifndef HEAP_H
#define HEAP_H

#include "config.h"

#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01

// Entries are one byte in length and are bitmasks
typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_block_table
{
  HEAP_BLOCK_TABLE_ENTRY entry[SMOLOS_MAX_HEAP_ALLOCATIONS];
};

struct heap_blocks
{
  // Pointer to the first block entry in the heap_block_table
  HEAP_BLOCK_TABLE_ENTRY* ptr;
  int total;

  // The index of the first block in the heap blocks table
  int sindex;
};

struct heap_entry
{
  struct heap_blocks blocks;
  void* data_ptr;
};

struct heap
{
  struct heap_block_table table;
  struct heap_entry entries[SMOLOS_MAX_HEAP_ALLOCATIONS];

  // Actual dara for our heap
  char data[SMOLOS_MAX_HEAP_ALLOCATIONS * SMOLOS_MEMORY_BLOCK_SIZE];
};

/**
 * @brief Creates a heap at the given "ptr". We require at least 18952 bytes of memory available for the pointer provided.
 * 
 * @param ptr 
 * @return struct heap* 
 */
struct heap* heap_create(void* ptr);
void* heap_malloc(struct heap* heap, int total);
void heap_free(struct heap* heap, void* ptr);

#endif