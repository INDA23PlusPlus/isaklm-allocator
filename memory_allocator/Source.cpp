#include <iostream>
#include "halloc.h"
#include "linalloc.h"

int main()
{
	HeapAlloc heap_allocator(1024 * 1024 * 1024);
	LinearAlloc linear_allocator(1024 * 1024 * 1024);

	char* mem1 = (char*)heap_allocator.alloc(1024);
	char* mem2 = (char*)heap_allocator.alloc(1024);
	char* mem3 = (char*)heap_allocator.alloc(1024);
	char* mem4 = (char*)heap_allocator.alloc(1024);

	heap_allocator.dealloc(mem1);
	heap_allocator.dealloc(mem4);
	heap_allocator.dealloc(mem2);
	heap_allocator.dealloc(mem3);

	int* mem5 = (int*)linear_allocator.alloc(1024);
	linear_allocator.dealloc();


	std::cout << "Success!" << '\n';

	while (true);
}