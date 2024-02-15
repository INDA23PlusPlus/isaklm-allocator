#pragma once

#include <cstdlib>
#include <cassert>


class LinearAlloc
{
	size_t heap_size = 0;
	char* heap_pointer = nullptr;
	size_t heap_index = 0;

public:
	LinearAlloc(size_t heap_bytes)
	{
		heap_size = heap_bytes;

		heap_pointer = (char*)malloc(heap_size);
	}

	~LinearAlloc()
	{
		free(heap_pointer);
	}

	void* alloc(size_t bytes)
	{
		assert(heap_index + bytes <= heap_size);

		heap_index += bytes;

		return (void*)(heap_pointer + heap_index);
	}

	void dealloc()
	{
		heap_index = 0;
	}
};