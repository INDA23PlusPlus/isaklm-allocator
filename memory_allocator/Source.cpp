#include <iostream>
#include "halloc.h"
#include "linalloc.h"

int main()
{
	HeapAlloc heap_allocator(1024 * 1024 * 1024);
	LinearAlloc linear_allocator(1024 * 1024 * 1024);

	int* mem1 = (int*)heap_allocator.alloc(1024);
	heap_allocator.dealloc(mem1);

	int* mem2 = (int*)linear_allocator.alloc(1024);
	linear_allocator.dealloc();


	std::cout << "Success!" << '\n';

	while (true);
}