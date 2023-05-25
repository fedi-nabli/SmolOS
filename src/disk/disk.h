#ifndef DISK_H
#define DISK_H

/**
 * @brief FInds all disks and initializes them
 */
void disk_search_and_init();

int disk_get_details(int bios_disk_id, int* total_heads, int* sectors_per_track);

#endif