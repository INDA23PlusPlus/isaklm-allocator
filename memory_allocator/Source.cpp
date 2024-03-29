#include <iostream>
#include "halloc.h"
#include "linalloc.h"

int main()
{
	HeapAlloc heap_allocator(1024 * 1024 * 1024);
	LinearAlloc linear_allocator(1024 * 1024 * 1024);

	int* mem1 = (int*)heap_allocator.alloc(1024);
	float* mem2 = (float*)heap_allocator.alloc(1024);
	char* mem3 = (char*)heap_allocator.alloc(1024);
	double* mem4 = (double*)heap_allocator.alloc(1024);

	heap_allocator.dealloc(mem1);
	heap_allocator.dealloc(mem4);
	heap_allocator.dealloc(mem2);
	heap_allocator.dealloc(mem3);

	uint16_t* mem5 = (uint16_t*)linear_allocator.alloc(1024);
	linear_allocator.dealloc();


	std::cout << "Success!" << '\n';

	while (true);
}