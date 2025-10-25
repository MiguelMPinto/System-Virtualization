#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

/* extra_data definida em d.c */
extern unsigned char extra_data[];
#ifndef EXTRA_DATA_SIZE
#define EXTRA_DATA_SIZE (512 * 1024)
#endif

void add_data_with_rss(void) {
    printf("=== Alínea e ===\n");
    printf("add_data_with_rss: vou tocar ~256KB em extra_data para aumentar Rss dessa região.\n");

    size_t target = 256 * 1024; /* 256 KB */
    if (target > EXTRA_DATA_SIZE) target = EXTRA_DATA_SIZE;

    long ps = sysconf(_SC_PAGESIZE);
    if (ps <= 0) ps = 4096;

    /* Escrever 1 byte por página para trazer as páginas para memória física */
    for (size_t off = 0; off < target; off += (size_t)ps) {
        extra_data[off] = (unsigned char)(extra_data[off] + 1);
    }

    /* Ler/accumular para evitar que o compilador elimine os writes */
    volatile unsigned char acc = 0;
    for (size_t off = 0; off < target; off += (size_t)ps) acc += extra_data[off];
    (void)acc;

    printf("add_data_with_rss: toque completo (%zu bytes). Ver /proc/%d/smaps para verificar aumento de Rss.\n", target, getpid());
}