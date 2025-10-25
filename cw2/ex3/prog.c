#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>


void increase_rss(unsigned char *ptr_regiao, int bytes_alvo); // implementada em a.c
void touch_data(unsigned char *ptr_data_region, size_t region_size); // implementada em b.c
void add_data_without_rss(void); // implementada em d.c

#define DATA_SIZE (16*1024*1024)
typedef unsigned char byte;

char info[DATA_SIZE];
char data[DATA_SIZE] = {1};

int main() {
    printf("PID: %u\n", getpid());

    const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
    printf("PAGE_SIZE: %ld\n", PAGE_SIZE);

    printf("#1 (press ENTER to continue)"); getchar();

    const int TWO_MB = 2*1024*1024;  // ~2 MiB
    printf("A tocar ~2 MiB na .bss (info)...\n");
    increase_rss((byte*)info, TWO_MB);

    // a
    printf("#2 (press ENTER to continue)"); getchar();

    printf("Chamar touch_data() para executar a alínea 3.b (acesso disperso na .data)...\n");
    touch_data((byte*)data, DATA_SIZE);

    // b
    printf("#3 (press ENTER to continue)"); getchar();

    // c
    do_alinea_c();

    printf("#4 (press ENTER to continuar)"); getchar();

    // d
    add_data_without_rss();

    printf("#5 (press ENTER to continuar)"); getchar();

    // e
    add_data_with_rss();

    printf("#6 (press ENTER to continuar)"); getchar();

    // f
    create_new_regions();

    printf("#7 (press ENTER to continuar)"); getchar();

    // g
    dirty_new_data_region();

    printf("END (press ENTER to continuar)"); getchar();

    return 0;
}