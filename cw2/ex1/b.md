### Exercise 1

**b) Subsections 1.1.3 and 5.3.1 specify a canonical address form for virtual addresses. With 4-level mapping, canonical addresses go from 0x0000000000000000 to 0x________________ and then from 0x________________ to 0xFFFFFFFFFFFFFFFF. Complete the missing values and write the canonical ranges for 5-level mapping.**

----

**4-level mapping**

Tendo em conta a arquitetura 4 level Mapping , temos que o bit mais significativo utilizado é o bit 47( 47-39 é onde está o PML4).
Tendo em conta endereços virtuais de 64 bits, sabemos que estes não utilizam os 64 bits na totalidade , e portanto , no caso do 4 level mapping , 
temos que desde o bit 48 até ao 63 , é apenas extensão do bit mais significativo.  

0x0000000000000000 to 0x00007FFFFFFFFFFF          
and
0xFFFF800000000000 to 0xFFFFFFFFFFFFFFFF          


**5-level mapping**

Tendo em conta a arquitetura 4 level Mapping , temos que o bit mais significativo utilizado é o bit 56( 56-48 é onde está o PML5).
Tendo em conta endereços virtuais de 64 bits, sabemos que estes não utilizam os 64 bits na totalidade , e portanto , no caso do 5 level mapping , 
temos que desde o bit 57 até ao 63 , é apenas extensão do bit mais significativo. 

0x0000000000000000 to 0x00FFFFFFFFFFFFFF
and

0xFF00000000000000 to 0xFFFFFFFFFFFFFFFF       
