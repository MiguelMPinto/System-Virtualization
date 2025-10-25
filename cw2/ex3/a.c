#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char byte;


/* Implementação focada no 3.a:
   - Apenas lê 1 byte por página na faixa pedida (bytes_alvo).
   - Usa volatile para impedir otimizações que eliminem as loads.
   - Não faz fork, não mexe noutras regiões. */
void increase_rss(byte *ptr_regiao, int bytes_alvo) {
    // Requires 
    if (ptr_regiao == NULL) {
        fprintf(stderr, "increase_rss: ponteiro NULL.\n");
        return;
    }
    if (bytes_alvo <= 0) {
        fprintf(stderr, "increase_rss: bytes_alvo deve ser > 0.\n");
        return;
    }

    long ps_l = sysconf(_SC_PAGESIZE);
    if (ps_l <= 0) {
        perror("increase_rss: sysconf(_SC_PAGESIZE)");
        return;
    }
    size_t page_size = (size_t)ps_l;
    size_t span = (size_t)bytes_alvo;
    size_t pages = (span + page_size - 1) / page_size;

    volatile byte *v = (volatile byte *)ptr_regiao;

    // Tocar 1 byte no início de cada página: 0, page_size, 2*page_size, ...
    for (size_t i = 0; i < pages; ++i) {
        size_t off = i * page_size;
        ptr_regiao[off] = 0;
    }
}