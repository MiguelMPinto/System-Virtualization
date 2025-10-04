////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2025/26
//
// Coursework Assignment #1
//

#include "ftests.h"

#include "chuta.h"
#include "somecode.h"

void test_add() {
	
	int res = add(2, 2);
	
	CH_ASSERT(res == 4);
}

void test_sub() {
	
	int res = sub(5, 2);
	
	CH_ASSERT(res == 3);
}

void test_strsize() {
	
	size_t size = strsize("ISEL");
	
	CH_ASSERT(size == 4);
}

void test_sub_negative() {
	int res = sub(2, 5);
	CH_ASSERT_MSG(res == -3, "subtrair 5 de 2 devia dar -3");
}

void test_strsize_empty() {
	size_t size = strsize("");
	CH_ASSERT_MSG(size == 0, "String vazia devia ter tamanho 0");
}
