////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2025/26
//
// Coursework Assignment #1
//

#include "somecode.h"

int add(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return a - b;                  
}

size_t strsize(const char * str) {
    const char *p = str;         // mantém const e não escreve na string
    size_t size = 0;
    while (*p != '\0') {         // ✔️ comparação correta
        ++size;
        ++p;
    }
    return size;
}
