#include "heap.h"
#include "memory/memory.h"
#include "kernel.h"

static void heap_mark_blocks_taken(HEAP_BLOCK_TABLE_ENTRY* ptr, int total)
{
  int i = 0;
  for (i = 0; i < total; i++)
  {
    ptr[i] = ptr[i] | HEAP_BLOCK_TABLE_ENTRY_TAKEN;
  }
}

static int heap_is_entry_free(struct heap_entry* entry)
{
  return entry->blocks.ptr == HEAP_BLOCK_TABLE_ENTRY_FREE;
}

static struct heap_entry* heap_find_free_heap_entry(struct heap* heap)
{
  int i = 0;
  for (i = 0; i < SMOLOS_MAX_HEAP_ALLOCATIONS; i++)
  {
    if (heap_is_entry_free(&heap->entries[i]))
      return &heap->entries[i];
  }

  return 0;
}

/**
 * @brief Return the total free blocks in a row
 * \warning You must ensure that the pointer and total provided will not overflow the buffer, this function will not chack for this.
 * 
 * @param ptr 
 * @param total 
 * @return int 
 */
static int heap_count_free_blocks_in_row(HEAP_BLOCK_TABLE_ENTRY* ptr, int total)
{
  int i = 0;
  int total_free = 0;
  for (i = 0; i < total; i++)
  {
    if (ptr[i] == HEAP_BLOCK_TABLE_ENTRY_FREE)
    {
      total_free++;
      continue;
    }

    break;
  }

  return total_free;
}

/**
 * @brief Gets the data pointer for a given heap entry
 * 
 * @param heap 
 * @param entry 
 * @return void* 
 */
static void* heap_entry_get_data(struct heap* heap, struct heap_entry* entry)
{
  return &heap->data[entry->blocks.sindex * SMOLOS_MEMORY_BLOCK_SIZE];
}

static struct heap_entry* heap_allocate_blocks(struct heap* heap, int total)
{
  int i = 0;
  // Substract total to ensure we never overflow
  for (i = 0; i < SMOLOS_MAX_HEAP_ALLOCATIONS - total; i++)
  {
    HEAP_BLOCK_TABLE_ENTRY* table_entry = &heap->table.entry[i];
    if (heap_count_free_blocks_in_row(table_entry, total) == total)
    {
      // Free block found
      struct heap_entry* entry = heap_find_free_heap_entry(heap);
      if (!entry)
        return 0;

      entry->blocks.ptr = table_entry;
      entry->blocks.total = total;
      entry->blocks.sindex = i;
      entry->data_ptr = heap_entry_get_data(heap, entry);
      heap_mark_blocks_taken(table_entry, total);

      return entry;
    }
  }

  return 0;
}

void heap_mark_blocks_available(struct heap_blocks* blocks)
{
  int i = 0;
  for (i = 0; i < blocks->total; i++)
  {
    blocks->ptr[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
  }
}

static int heap_mark_entry_available(struct heap_entry* entry)
{
  heap_mark_blocks_available(&entry->blocks);
  memset(entry, 0, sizeof(struct heap_entry));
}

static struct heap_entry* heap_get_entry_for_data_ptr(struct heap* heap, void* ptr)
{
  int i = 0;
  for (i = 0; i < SMOLOS_MAX_HEAP_ALLOCATIONS; i++)
  {
    if (heap->entries[i].data_ptr == ptr)
    {
      return &heap->entries[i];
    }
  }

  return 0;
}

struct heap* heap_create(void* ptr)
{
  struct heap* heap = ptr;
  #if SMOLOS_FORCE_MEMORY_ALIGNMENT == 1
  if (((unsigned char)ptr % SMOLOS_MEMORY_BLOCK_SIZE) != 0)
  {
    panic("heap_create(): Expecting pointer to be able to divide into SMOLOS_MAX_HEAP_ALLOCATIONS");
  }
  #endif
  memset(heap, 0, sizeof(struct heap));
  return heap;
}

void* heap_malloc(struct heap* heap, int total)
{
  struct heap_entry* entry;
  int total_blocks = total / SMOLOS_MEMORY_BLOCK_SIZE;
  if (total % SMOLOS_MEMORY_BLOCK_SIZE)
  {
    total_blocks += 1;
  }

  entry = heap_allocate_blocks(heap, total_blocks);
  if (!entry)
  {
    return 0;
  }

  return entry->data_ptr;
}

void heap_free(struct heap* heap, void* ptr)
{
  struct heap_entry* entry = heap_get_entry_for_data_ptr(heap, ptr);
  if (!entry)
  {
    panic("Heap entry not found for given pointer\r\n");
  }
  heap_mark_entry_available(entry);
}