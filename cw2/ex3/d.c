#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// 512 KB (~0.5 MiB)
#define EXTRA_DATA_SIZE (512 * 1024)

// Variável global inicializada -> entra na secção .data
uint8_t extra_data[EXTRA_DATA_SIZE] = { 7 };

// Função chamada pelo main
void add_data_without_rss(void) {
    printf("=== Alínea d) ===\n");
    printf("#d (press ENTER to continuar) "); getchar();
}
