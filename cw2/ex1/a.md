### Exercise 1

**a) Compare Figure 5-17 with Figure 5-18. The change from 4-level mapping to 5-level mapping increases the maximum size of virtual address spaces. Calculate the maximum size for each case.**

Fazendo uma comparação entre estas duas arquiteturas , podemos afirmar que a principal diferença reside no espaço de endereçamento virtual que oferecem. Ambas utilizam endereços virtuais de 64 bits sendo  que na arquitetura 4 level mapping , temos 4 indices de 9 bits cada e um offset de physicall Page de 12 bits totalizando portanto 4*9+12 = 48 bits utilizavéis do endereço virtual . Enquanto que na arquitetura 5 level-mapping , temos 5 indices de 9 bits cada e , tal como em 4 level mapping , um offset de physicall page de 12 bits totalizando portanto 5*9+12 =  57 bits utilizaveis do endereço virtual.

**4-level mapping**

4*9 ( 4 indices de 9 bits cada) + 12 ( bits do offset da physicall page) = 48 bits 
Portanto o espçao virtual máximo = 2^48 = 2^20 * 2^20 * 2^8 = 1*10^6 * 1*10^6 * 256 = 256 * 10^12 = 256 Tib 


**5-level mapping**

5*9 ( 5 indices de 9 bits cada) + 12 ( bits do offset da physicall page) = 57 bits 
Portanto o espçao virtual máximo = 2^57 = 2^20 * 2^20 * 2^10 * 2^7 = 1*10^6 * 1*10^6 * 1*10^3 * 128 = 128 * 10^15 = 128 Pib 
