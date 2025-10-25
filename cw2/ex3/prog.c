#include <stdio.h>
#include <unistd.h>
#include <stdint.h>


#define DATA_SIZE 16*1024*1024
typedef unsigned char byte;

void increase_rss(byte *ptr_regiao, int bytes_alvo);
void touch_data(byte *ptr_data_region, size_t region_size);
void add_data_without_rss(void);


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

	printf("#4 (press ENTER to continue)"); getchar();
	add_data_without_rss();
	// d

	printf("#5 (press ENTER to continue)"); getchar();

	// e

	printf("#6 (press ENTER to continue)"); getchar();

	// f
	
	printf("#7 (press ENTER to continue)"); getchar();

	// g

	printf("END (press ENTER to continue)"); getchar();

	return 0;
}
