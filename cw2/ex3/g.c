#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

extern void *new_data_region;
extern size_t new_data_region_size;

void dirty_new_data_region(void) {

    if (new_data_region == NULL || new_data_region_size == 0) {
        fprintf(stderr, "dirty_new_data_region: new_data_region não existe. Executa create_new_regions() primeiro.\n");
        return;
    }

    size_t target = 128 * 1024; // 128 KB

    if (target > new_data_region_size) target = new_data_region_size;

    long ps = sysconf(_SC_PAGESIZE);
    if (ps <= 0) ps = 4096;

    volatile uint8_t *p = (volatile uint8_t *)new_data_region;

    for (size_t off = 0; off < target; off += (size_t)ps) {
        p[off] = (uint8_t)((off / ps) & 0xFF);
    }

    volatile uint8_t acc = 0;
    
    for (size_t off = 0; off < target; off += (size_t)ps) acc += p[off];
    (void)acc;

    printf("dirty_new_data_region: %zu bytes escritos em new_data_region (deve aparecer como Private_Dirty).\n", target);
}