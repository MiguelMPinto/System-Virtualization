#include <stdio.h>
#include <unistd.h>   // getpid, sysconf
#include <stdlib.h>   // EXIT_SUCCESS
#include <stdint.h>   // uint8_t

typedef unsigned char byte;

void touch_data(byte *ptr_data_region, size_t region_size) {
    // Informação útil
    printf("do_ex3b: PID = %d\n", getpid());
    const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
    printf("do_ex3b: PAGE_SIZE = %ld\n", PAGE_SIZE);

    // Ponto de observação 1: antes do toque disperso
    printf("do_ex3b #1 (pause) - ver /proc/<pid>/smaps, depois press ENTER to continue...\n");
    getchar();

    // Touch disperso: 1 byte em 128 páginas diferentes
    long acc = 0;
    for (int i = 0; i < 128; ++i) {
        size_t offset = (size_t)i * (size_t)PAGE_SIZE;
        if (offset >= region_size) break;
        // Apenas leitura para manter a página "clean"
        acc += ptr_data_region[offset];
    }

    // Mostra soma acumulada para prevenir otimizações e dar feedback
    printf("do_ex3b: sparse data sum = %ld\n", acc);

    // Ponto de observação 2: depois do toque disperso
    printf("do_ex3b #2 (pause) - ver /proc/<pid>/smaps novamente (compare Private_Clean/Rss). Press ENTER to continue...\n");
    getchar();

    // Termina
    return;
}
