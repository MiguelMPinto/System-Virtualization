#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// variáveis expostas para que g.c possa usar
void *new_data_region = NULL;
size_t new_data_region_size = 0;

void *code_region = NULL;
size_t code_region_size = 0;

void create_new_regions(void) {
    printf("=== Alínea f ===\n");
    // 1) criar região de código (~4KB)
    code_region_size = 4096;
    code_region = mmap(NULL, code_region_size, PROT_READ | PROT_WRITE,
                       MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (code_region == MAP_FAILED) {
        perror("create_new_regions: mmap code_region");
        code_region = NULL;
    } else {
        // preencher com alguns bytes (por exemplo 0xC3) para aparecer como mapeamento
        memset(code_region, 0xC3, 16);
        // tornar executável (apenas para aparecer como r-xp no smaps)
        if (mprotect(code_region, code_region_size, PROT_READ | PROT_EXEC) != 0) {
            perror("create_new_regions: mprotect code_region -> PROT_READ|PROT_EXEC");
            // se falhar, deixamos como estava
        }
        printf("create_new_regions: code_region mmaped at %p size %zu\n", code_region, code_region_size);
    }

    // 2) criar região de dados (~256KB)
    new_data_region_size = 256 * 1024;
    new_data_region = mmap(NULL, new_data_region_size, PROT_READ | PROT_WRITE,
                           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (new_data_region == MAP_FAILED) {
        perror("create_new_regions: mmap new_data_region");
        new_data_region = NULL;
        new_data_region_size = 0;
    } else {
        // não tocar agora (será sujada em g)
        printf("create_new_regions: data_region mmaped at %p size %zu\n", new_data_region, new_data_region_size);
    }

    printf("create_new_regions: criaçao completa. Ver /proc/%d/smaps para novas mappings.\n", getpid());
}