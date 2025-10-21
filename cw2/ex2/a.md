### Exercise 2

**a) List and explain the sequence of calls performed by the program in x86-64/prog.s**

A primeira chamada , chama a função openat( int dirfd, const char *path,int flags) que recebe como primeiro argumento , -100 que representa a diretoria atual , a path "/etc/os-release" e como a flag passado está a 0 é read-only. O retorno desta chamada é o novo fd.

A segunda chamada ,  chama a função lseek( int fd, off_t offset , int whence) que recebe como primeiro argumento , o fd resultante da chamada anterior ,offset a 0 e whence a 2. Esta chamada retorna o offset resultante.

A terceira chamada é feita ao mmap(size_t length;void addr[length], size_t length, int prot, int flags, int fd, off_t offset); o primeiro argumento é passado a null e portanto o kernel é que decide o endereço , o segundo argumento lenght é o retorno da chamada anterior , prot é passado a 1 , ou seja , apenas leitura , flags recebe o Map private , recebe de seguida o fd e o offset a 0. O objetivo é criar um novo mapping no espaço de endereços virtuais do processo chamado. Retorna um ponteiro para a area ocupada. 

A quarta chamada é a chamada do write(int fd , char *buff, size_t count) que recebe como parametros 1 , ou seja stdout, o ponteiro para a area mapeada e o tamanho/offset.

A ultima chamada dá exit(0)


