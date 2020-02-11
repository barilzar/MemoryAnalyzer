#include "test.h"

void* operator new(size_t size) throw(...){
    void* ret = malloc(size);

    if(!ret)
		throw std::bad_alloc();

    fprintf(stdout, "allocate: %p %d bytes from ...\n", ret, size);
    return ret;
}

void* operator new[] (size_t size) throw(...){
    void* ret = malloc(size);

    if(!ret)
		throw std::bad_alloc();

    fprintf(stdout, "allocate: %p %d bytes from ...\n", ret, size);
    return ret;
}

void operator delete (void* data){
    free(data);
    fprintf(stdout, "free: %p\n", data);
}

void operator delete [] (void* data){
    free(data);
    fprintf(stdout, "free: %p\n", data);
}
