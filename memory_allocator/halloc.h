#pragma once

#include <cstdlib>
#include <cassert>
#include <vector>


class HeapAlloc
{
	const int memory_block_increment = 16384;

	struct MemoryBlock
	{
		void* pointer;
		int64_t start;
		int64_t bytes;
		int64_t prior_block_index;
		int64_t next_block_index;
	};

	int64_t heap_size = 0;
	char* heap_pointer = nullptr;
	int64_t heap_index = 0;

	std::vector<MemoryBlock> memory_blocks;
	int64_t memory_block_count = 0;

	void add_memory_blocks()
	{
		std::vector<MemoryBlock> new_memory_blocks(memory_blocks.size() + memory_block_increment);

		for (int i = 0; i < memory_blocks.size(); ++i)
		{
			new_memory_blocks[i] = memory_blocks[i];
		}

		memory_blocks = new_memory_blocks;
	}

public:
	HeapAlloc(int64_t heap_bytes)
	{
		add_memory_blocks();

		heap_size = heap_bytes;

		heap_pointer = (char*)malloc(heap_size);
	}

	~HeapAlloc()
	{
		free(heap_pointer);
	}

	void* alloc(int64_t bytes)
	{
		int64_t new_block_start;

		void* new_pointer;

		if (heap_index + bytes <= heap_size)
		{
			new_block_start = heap_index;

			new_pointer = heap_pointer + new_block_start;


			memory_blocks[memory_block_count] = { new_pointer, new_block_start, bytes, memory_block_count - 1, memory_block_count + 1 };

			heap_index += bytes;
		}
		else
		{
			bool found_space = false;

			int64_t current_block_index = 0;
			
			for (int i = 0; i < memory_block_count; ++i)
			{
				int64_t next_block_index = memory_blocks[current_block_index].next_block_index;

				if (memory_blocks[next_block_index].start + bytes < memory_blocks[current_block_index].start)
				{
					found_space = true;

					break;
				}

				current_block_index = next_block_index;
			}


			assert(found_space);

			MemoryBlock current_block = memory_blocks[current_block_index];

			new_block_start = current_block.start + current_block.bytes;

			new_pointer = heap_pointer + new_block_start;

			
			memory_blocks[memory_block_count] = { new_pointer, new_block_start, bytes, current_block_index, current_block.next_block_index };

			memory_blocks[current_block_index] = { current_block.pointer, current_block.start, current_block.bytes, current_block.prior_block_index, memory_block_count };
		}

		++memory_block_count;

		if (memory_block_count == memory_blocks.size())
		{
			add_memory_blocks();
		}

		return new_pointer;
	}

	void dealloc(void* pointer)
	{
		int64_t block_index = -1;

		for (int i = 0; i < memory_block_count; ++i)
		{
			if (pointer == memory_blocks[i].pointer)
			{
				block_index = i;

				break;
			}
		}


		assert(block_index != -1);

		{
			MemoryBlock memory_block = memory_blocks[block_index];

			if (memory_block.prior_block_index >= 0)
			{
				memory_blocks[memory_block.prior_block_index].next_block_index = memory_block.next_block_index;
			}
			
			if (memory_block.next_block_index < memory_block_count)
			{
				memory_blocks[memory_block.next_block_index].prior_block_index = memory_block.prior_block_index;
			}
		}

		memory_blocks[block_index] = memory_blocks[memory_block_count - 1];

		{
			MemoryBlock memory_block = memory_blocks[block_index];

			if (memory_block.prior_block_index >= 0)
			{
				memory_blocks[memory_block.prior_block_index].next_block_index = memory_block.next_block_index;
			}

			if (memory_block.next_block_index < memory_block_count)
			{
				memory_blocks[memory_block.next_block_index].prior_block_index = memory_block.prior_block_index;
			}
		}

		--memory_block_count;
	}
};