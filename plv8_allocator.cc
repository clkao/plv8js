#include "plv8_allocator.h"

#define RECHECK_INCREMENT 1_MB

size_t operator""_MB( unsigned long long const x ) { return 1024L * 1024L * x; }

ArrayAllocator::ArrayAllocator(size_t limit) : heap_limit(limit),
											   heap_size(RECHECK_INCREMENT),
											   next_size(RECHECK_INCREMENT),
											   allocated(0) {}

bool ArrayAllocator::check(const size_t length) {
	if (heap_size + allocated + length > next_size) {
		v8::HeapStatistics heap_statistics;
		plv8_isolate->GetHeapStatistics(&heap_statistics);
		heap_size = heap_statistics.total_heap_size();
		if (heap_size + allocated + length > heap_limit) {
			// wee need to force GC here,
			// otherwise the next allocation will fail even if there is a space for it
			plv8_isolate->LowMemoryNotification();
			heap_size = heap_statistics.total_heap_size();
			if (heap_size + allocated + length > heap_limit) {
				return false;
			}
		}
		next_size = heap_size + allocated + length + RECHECK_INCREMENT;
	}
	allocated += length;
	return true;
}

void* ArrayAllocator::Allocate(size_t length) {
	if (check(length)) {
		return std::calloc(length, 1);
	} else {
		return nullptr;
	}
}

void* ArrayAllocator::AllocateUninitialized(size_t length) {
	if (check(length)) {
		return std::malloc(length);
	} else {
		return nullptr;
	}
}

void ArrayAllocator::Free(void* data, size_t length) {
	allocated -= length;
	next_size -= length;
	std::free(data);
}
